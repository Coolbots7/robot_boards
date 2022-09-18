#ifndef _MOTOR_CONTROLLER_DC_MOTOR_H_
#define _MOTOR_CONTROLLER_DC_MOTOR_H_

#include <math.h>

#include "Arduino.h"

#include "./motor.h"

/**
 * DC Motor
 *
 * Helper to interface with DC Brushed Motor drivers
 */
class DCMotor : public Motor
{
private:
    // Motor driver H bridge A side pin
    uint8_t a_pin;

    // Motor driver H bridge B side pin
    uint8_t b_pin;

    // Motor driver PWM pin
    uint8_t pwm_pin = NAN;

    // Motor driver brake or coast mode
    bool brake = true;

public:
    /**
     * @brief Construct a new DCMotor object
     *
     * @param a_pin Motor driver H bridge A side pin
     * @param b_pin Motor driver H bridge B side pin
     * @param enable_pin Motor driver enable pin
     */
    DCMotor(uint8_t a_pin, uint8_t b_pin, uint8_t enable_pin) : DCMotor(a_pin, b_pin, NAN, enable_pin) {}

    /**
     * @brief Construct a new DCMotor object
     *
     * @param a_pin Motor driver H bridge A side pin
     * @param b_pin Motor driver H bridge B side pin
     * @param pwm_pin Motor driver H bridge pulse pin
     * @param enable_pin Motor driver enable pin
     */
    DCMotor(uint8_t a_pin, uint8_t b_pin, uint8_t pwm_pin, uint8_t enable_pin);

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

    /**
     * @brief Update motor controller outputs with the current motor states
     */
    void writeSpeed();
};

#endif
