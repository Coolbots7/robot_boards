#include "twowire/master.h"

#include "./motor.h"
#include "./motor_controller.h"

MotorControllerMotorInterface::MotorControllerMotorInterface(uint8_t address, uint8_t motor_idx, I2CMaster *i2c_master)
{
    this->address = address;
    this->motor_idx = motor_idx;
    this->i2c_master = i2c_master;
}

void MotorControllerMotorInterface::setEnabled(bool enabled)
{
    if (this->motor_idx == 1)
    {
        this->i2c_master->setRegister<bool>(this->address, MotorControllerRegisters::MOTOR_1_ENABLED, enabled);
    }
    else
    {
        this->i2c_master->setRegister<bool>(this->address, MotorControllerRegisters::MOTOR_2_ENABLED, enabled);
    }
}

void MotorControllerMotorInterface::setDisabled()
{
    this->setEnabled(false);
}

bool MotorControllerMotorInterface::getEnabled()
{
    if (this->motor_idx == 1)
    {
        return this->i2c_master->getRegister<bool>(this->address, MotorControllerRegisters::MOTOR_1_ENABLED);
    }
    else
    {
        return this->i2c_master->getRegister<bool>(this->address, MotorControllerRegisters::MOTOR_2_ENABLED);
    }
}

void MotorControllerMotorInterface::setReversed(bool reversed)
{
    if (this->motor_idx == 1)
    {
        this->i2c_master->setRegister<bool>(this->address, MotorControllerRegisters::MOTOR_1_REVERSED, reversed);
    }
    else
    {
        this->i2c_master->setRegister<bool>(this->address, MotorControllerRegisters::MOTOR_2_REVERSED, reversed);
    }
}

bool MotorControllerMotorInterface::getReversed()
{
    if (this->motor_idx == 1)
    {
        return this->i2c_master->getRegister<bool>(this->address, MotorControllerRegisters::MOTOR_1_REVERSED);
    }
    else
    {
        return this->i2c_master->getRegister<bool>(this->address, MotorControllerRegisters::MOTOR_2_REVERSED);
    }
}

void MotorControllerMotorInterface::setSpeed(float speed)
{
    if (this->motor_idx == 1)
    {
        this->i2c_master->setRegister<float>(this->address, MotorControllerRegisters::MOTOR_1_SPEED, speed);
    }
    else
    {
        this->i2c_master->setRegister<float>(this->address, MotorControllerRegisters::MOTOR_2_SPEED, speed);
    }
}

float MotorControllerMotorInterface::getSpeed()
{
    if (this->motor_idx == 1)
    {
        return this->i2c_master->getRegister<float>(this->address, MotorControllerRegisters::MOTOR_1_SPEED);
    }
    else
    {
        return this->i2c_master->getRegister<float>(this->address, MotorControllerRegisters::MOTOR_2_SPEED);
    }
}
