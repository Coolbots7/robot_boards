#include "common/state.h"
#include "state-machine.h"
#include "twowire/common.h"
#include "twowire/slave.h"

#include "./i2c.h"
#include "./dc_motor.h"

MotorControllerI2CSlave::MotorControllerI2CSlave(uint8_t address_offset, StateMachine *state, DCMotor *motor_1, DCMotor *motor_2) : I2CDevice(I2C_MOTOR_CONTROLLER_BASE_ADDRESS + address_offset, state)
{
    this->motor_1 = motor_1;
    this->motor_2 = motor_2;
}

void MotorControllerI2CSlave::onReceive(int num_bytes)
{
    // Make sure at least one full message is available
    if (this->i2c_slave->getAvailable() >= sizeof(WireMessage))
    {
        // Read the message from the TwoWire buffer
        WireMessage message = this->i2c_slave->parseMessage();

        switch (message.message_type)
        {
        case MotorControllerRegisters::MOTOR_1_ENABLED:
            this->motor_1->setEnabled(I2CCore::decodeValue<bool>(message.data));
            break;
        case MotorControllerRegisters::MOTOR_1_REVERSED:
            this->motor_1->setReversed(I2CCore::decodeValue<bool>(message.data));
            break;
        case MotorControllerRegisters::MOTOR_1_BRAKE:
            this->motor_1->setBrake(I2CCore::decodeValue<bool>(message.data));
            break;
        case MotorControllerRegisters::MOTOR_1_SPEED:
            this->motor_1->setSpeed(I2CCore::decodeValue<float>(message.data));
            break;

        case MotorControllerRegisters::MOTOR_2_ENABLED:
            this->motor_2->setEnabled(I2CCore::decodeValue<bool>(message.data));
            break;
        case MotorControllerRegisters::MOTOR_2_REVERSED:
            this->motor_2->setReversed(I2CCore::decodeValue<bool>(message.data));
            break;
        case MotorControllerRegisters::MOTOR_2_BRAKE:
            this->motor_2->setBrake(I2CCore::decodeValue<bool>(message.data));
            break;
        case MotorControllerRegisters::MOTOR_2_SPEED:
            this->motor_2->setSpeed(I2CCore::decodeValue<float>(message.data));
            break;

        case StateRegisters::HEARTBEAT_STATE:
            this->master_state = I2CCore::decodeValue<State>(message.data);
            break;
        case StateRegisters::HEARTBEAT_TIME:
            this->master_time = I2CCore::decodeValue<uint32_t>(message.data);
            this->master_offset = this->master_time - millis();
            break;
        case Registers::REGISTER_REQUEST_TYPE:
            this->i2c_slave->setRequestRegister(I2CCore::decodeValue<uint8_t>(message.data));
            break;
        default:
            break;
        };
    }
}

void MotorControllerI2CSlave::onRequest()
{
    switch (this->i2c_slave->getRequestRegister())
    {
    case MotorControllerRegisters::MOTOR_1_ENABLED:
        this->i2c_slave->write<bool>(this->motor_1->getEnabled());
        break;
    case MotorControllerRegisters::MOTOR_1_REVERSED:
        this->i2c_slave->write<bool>(this->motor_1->getReversed());
        break;
    case MotorControllerRegisters::MOTOR_1_BRAKE:
        this->i2c_slave->write<bool>(this->motor_1->getBrake());
        break;
    case MotorControllerRegisters::MOTOR_1_SPEED:
        this->i2c_slave->write<float>(this->motor_1->getSpeed());
        break;

    case MotorControllerRegisters::MOTOR_2_ENABLED:
        this->i2c_slave->write<bool>(this->motor_2->getEnabled());
        break;
    case MotorControllerRegisters::MOTOR_2_REVERSED:
        this->i2c_slave->write<bool>(this->motor_2->getReversed());
        break;
    case MotorControllerRegisters::MOTOR_2_BRAKE:
        this->i2c_slave->write<bool>(this->motor_2->getBrake());
        break;
    case MotorControllerRegisters::MOTOR_2_SPEED:
        this->i2c_slave->write<float>(this->motor_2->getSpeed());
        break;

    case StateRegisters::STATE:
        this->i2c_slave->write<State>(this->state->getCurrentState());
        break;
    default:
        break;
    };
}
