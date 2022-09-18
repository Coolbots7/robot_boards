#include "./dc_motor.h"

DCMotor::DCMotor(uint8_t a_pin, uint8_t b_pin, uint8_t pwm_pin, uint8_t enable_pin) : Motor(enable_pin)
{
    // assign pins
    this->a_pin = a_pin;
    this->b_pin = b_pin;
    this->pwm_pin = pwm_pin;

    // set pin modes
    // Note: The pwm_pin pin mode is set in the analogWrite function
    pinMode(this->a_pin, OUTPUT);
    pinMode(this->b_pin, OUTPUT);
}

void DCMotor::setBrake(bool brake)
{
    this->brake = brake;
}

void DCMotor::setCoast()
{
    setBrake(false);
}

bool DCMotor::getBrake()
{
    return this->brake;
}

void DCMotor::writeSpeed()
{
    if (!this->enabled || this->speed == 0.0f)
    {
        if (this->enable_pin == this->pwm_pin)
        {
            // Stop pulsing motor PWM pin
            analogWrite(this->pwm_pin, 0);
        }
        else
        {
            // Disable the motor using the dedicated enable pin
            digitalWrite(this->enable_pin, this->enabled);
        }

        // Set motor driver to brake or coast
        if (this->brake)
        {
            // brake
            digitalWrite(this->a_pin, LOW);
            digitalWrite(this->b_pin, LOW);
        }
        else
        {
            // coast
            digitalWrite(this->a_pin, HIGH);
            digitalWrite(this->b_pin, HIGH);
        }
    }
    else
    {
        // Get speed and invert if motor is reversed
        float speed = this->speed;
        if (this->reversed)
        {
            speed = speed * -1;
        }

        // Set H bridge low side and get high side pin for PWM control based on speed direction
        uint8_t pwm_pin = NAN;
        if (speed > 0.0f)
        {
            pwm_pin = this->a_pin;
            digitalWrite(this->b_pin, LOW);
        }
        else if (speed < 0.0f)
        {
            digitalWrite(this->a_pin, LOW);
            pwm_pin = this->b_pin;
        }

        // check if separate PWM pin is defined
        if (!isnan(this->pwm_pin))
        {
            // set the H bridge pin high and pulse the PWM pin instead
            digitalWrite(pwm_pin, HIGH);
            pwm_pin = this->pwm_pin;
        }

        // pulse PWM pin based on the speed
        int pwm = round(255.0f * abs(speed));
        analogWrite(pwm_pin, pwm);
    }
}
