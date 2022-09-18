#include "./dc_motor.h"
#include "./motor_controller.h"

void DCMotorControllerMotorInterface::setBrake(bool brake)
{
    if (this->motor_idx == 1)
    {
        this->i2c_master->setRegister<bool>(this->address, MotorControllerRegisters::MOTOR_1_BRAKE, brake);
    }
    else
    {
        this->i2c_master->setRegister<bool>(this->address, MotorControllerRegisters::MOTOR_2_BRAKE, brake);
    }
}

void DCMotorControllerMotorInterface::setCoast()
{
    this->setBrake(false);
}

bool DCMotorControllerMotorInterface::getBrake()
{
    if (this->motor_idx == 1)
    {
        return this->i2c_master->getRegister<bool>(this->address, MotorControllerRegisters::MOTOR_1_BRAKE);
    }
    else
    {
        return this->i2c_master->getRegister<bool>(this->address, MotorControllerRegisters::MOTOR_2_BRAKE);
    }
}
