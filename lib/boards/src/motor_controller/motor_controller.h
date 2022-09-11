#ifndef _ROBOT_BOARDS_BOARDS_MOTOR_CONTROLLER_H_
#define _ROBOT_BOARDS_BOARDS_MOTOR_CONTROLLER_H_

#include "../board.h"
#include "./dc_motor.h"

// Motor Controller 0x40 - 0x4F
#define I2C_MOTOR_CONTROLLER_BASE_ADDRESS 0x40
#define I2C_MOTOR_CONTROLLER_ADDRESS_RANGE 16

// Define constants used to select the value to be sent on next I2C request event
enum MotorControllerRegisters
{
    // MOTOR 1 0x00 - 0x0F
    MOTOR_1_ENABLED = 0x00,
    MOTOR_1_SPEED = 0x01,
    MOTOR_1_REVERSED = 0x02,
    MOTOR_1_BRAKE = 0x03,
    MOTOR_1_CURRENT = 0x04,

    // MOTOR 2 0x10 - 0x1F
    MOTOR_2_ENABLED = 0x10,
    MOTOR_2_SPEED = 0x11,
    MOTOR_2_REVERSED = 0x12,
    MOTOR_2_BRAKE = 0x13,
    MOTOR_2_CURRENT = 0x14,
};

/**
 * @brief Helper to interface with a Motor Controller board
 */
class MotorControllerInterface : public BoardInterface
{
public:
    // Instance of the Motor Interface Utility for motor 1
    DCMotorControllerMotorInterface *motor_1;

    // Instance of the Motor Interface Utility for motor 2
    DCMotorControllerMotorInterface *motor_2;

    /**
     * @brief Construct a new Motor Controller Interface object
     *
     * @param address_offset Address offset for the Motor Controller board from the default address
     * @param i2c_master Instance of I2CMaster to interface with the board
     */
    MotorControllerInterface(uint8_t address_offset, I2CMaster *i2c_master);
};

#endif
