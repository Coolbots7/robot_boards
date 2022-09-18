#include "twowire/common.h"

#include "./dc_motor.h"
#include "./motor_controller.h"

MotorControllerInterface::MotorControllerInterface(uint8_t address_offset, I2CMaster *i2c_master) : BoardInterface(I2C_MOTOR_CONTROLLER_BASE_ADDRESS + address_offset, i2c_master)
{
    // create motor interface helper for motor 1
    this->motor_1 = new DCMotorControllerMotorInterface(I2C_MOTOR_CONTROLLER_BASE_ADDRESS + address_offset, 1, i2c_master);

    // create motor interface helper for motor 2
    this->motor_2 = new DCMotorControllerMotorInterface(I2C_MOTOR_CONTROLLER_BASE_ADDRESS + address_offset, 2, i2c_master);
}
