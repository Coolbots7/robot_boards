#ifndef _ROBOT_BOARDS_BOARDS_MOTOR_CONTROLLER_H_
#define _ROBOT_BOARDS_BOARDS_MOTOR_CONTROLLER_H_

#include "../board.h"

// Motor Controller 0x40 - 0x4F
#define I2C_MOTOR_CONTROLLER_BASE_ADDRESS 0x40
#define I2C_MOTOR_CONTROLLER_ADDRESS_RANGE 16

/**
 * @brief Helper to interface with a Motor Controller board
 */
class MotorControllerInterface : public BoardInterface
{
public:
    /**
     * @brief Construct a new Motor Controller Interface object
     *
     * @param address_offset Address offset for the Motor Controller board from the default address
     * @param i2c_master Instance of I2CMaster to interface with the board
     */
    MotorControllerInterface(uint8_t address_offset, I2CMaster *i2c_master);
};

#endif
