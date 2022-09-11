#ifndef _MOTOR_CONTROLLER_MOTOR_H_
#define _MOTOR_CONTROLLER_MOTOR_H_

#include <math.h>

#include "Arduino.h"

/**
 * Motor
 *
 * Base class to implement a Motor interface helper
 */
class Motor
{
protected:
    // Enabled state of the motor
    bool enabled = false;

    // Motor driver enabled pin
    uint8_t enable_pin;

    // Reversed state of the motor
    bool reversed = false;

    // Speed of the motor
    float speed = 0.0f;

public:
    /**
     * @brief Construct a new Motor object
     *
     * @param enable_pin Motor driver H bridge enable pin
     */
    Motor(uint8_t enable_pin);

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
     * @brief Set the reversed state of the motor
     *
     * @param reversed Optional reversed state
     */
    void setReversed(bool reversed = true);

    /**
     * @brief Get the reversed state of the motor
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
