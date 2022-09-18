#ifndef _ROBOT_BOARDS_BOARDS_MOTOR_CONTROLLER_MOTOR_H_
#define _ROBOT_BOARDS_BOARDS_MOTOR_CONTROLLER_MOTOR_H_

#include "twowire/master.h"

/**
 * Motor Controller Motor Interface
 *
 * Base class to implement a helper to interface with a Motor connected to a Motor Controller board
 */
class MotorControllerMotorInterface
{
protected:
    // Address of the board
    uint8_t address;

    // Instance of I2CMaster to communicate with the board
    I2CMaster *i2c_master;

    // Index of the motor on the driver
    uint8_t motor_idx;

public:
    /**
     * @brief Construct a new Motor Controller Motor Interface object
     *
     * @param address Address of the board
     * @param motor_idx Motor index on the driver
     * @param i2c_master Instance of I2CMaster to communicate with the board
     */
    MotorControllerMotorInterface(uint8_t address, uint8_t motor_idx, I2CMaster *i2c_master);

    /**
     * @brief Set the Enabled state of the motor
     *
     * @param enabled Optional enabled state
     */
    void setEnabled(bool enabled = true);

    /**
     * @brief Set the Enabled state to disabled
     */
    void setDisabled();

    /**
     * @brief Get the Enabled state of the motor
     *
     * @return true The motor is enabled
     * @return false The motor is disabled
     */
    bool getEnabled();

    /**
     * @brief Set the Reversed state of the motor
     *
     * @param reversed Optional reversed state
     */
    void setReversed(bool reversed = true);

    /**
     * @brief Get the Reversed state of the motor
     *
     * @return true The motor direction is reversed
     * @return false The motor direction is not reversed
     */
    bool getReversed();

    /**
     * @brief Set the Speed of the motor
     *
     * @param speed Speed between -1.0 and 1.0
     */
    void setSpeed(float speed);

    /**
     * @brief Get the Speed of the motor
     *
     * @return float Speed of the motor between -1.0 and 1.0
     */
    float getSpeed();
};

#endif
