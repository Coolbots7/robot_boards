#ifndef _ROBOT_BOARDS_BOARDS_MOTOR_CONTROLLER_DC_MOTOR_H_
#define _ROBOT_BOARDS_BOARDS_MOTOR_CONTROLLER_DC_MOTOR_H_

#include "twowire/master.h"

#include "./motor.h"

/**
 * DC Motor Controller Motor Interface
 *
 * Helper to interface with a DC Motor connected to a Motor Controller board
 */
class DCMotorControllerMotorInterface : public MotorControllerMotorInterface
{
public:
    /**
     * @brief Construct a new DCMotorControllerMotorInterface object
     *
     * @param address Address of the board
     * @param motor_idx Motor index on the driver
     * @param i2c_master Instance of I2CMaster to communicate with the board
     */
    DCMotorControllerMotorInterface(uint8_t address, uint8_t motor_idx, I2CMaster *i2c_master) : MotorControllerMotorInterface(address, motor_idx, i2c_master) {}

    /**
     * @brief Set the Brake state of the motor
     *
     * @param brake Optional brake state
     */
    void setBrake(bool brake = true);

    /**
     * @brief Set the Brake state to Coast
     */
    void setCoast();

    /**
     * @brief Get the Brake state of the motor
     *
     * @return true The motor is in brake state
     * @return false The motor is in coast state
     */
    bool getBrake();
};

#endif
