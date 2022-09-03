#include "twowire/common.h"

#include "./motor_controller.h"

MotorControllerInterface::MotorControllerInterface(uint8_t address_offset, I2CMaster *i2c_master) : BoardInterface(I2C_MOTOR_CONTROLLER_BASE_ADDRESS + address_offset, i2c_master) {}
