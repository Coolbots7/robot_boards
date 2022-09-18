#include "./motor.h"

Motor::Motor(uint8_t enable_pin)
{
    // assign pins
    this->enable_pin = enable_pin;

    // set pin modes
    pinMode(this->enable_pin, OUTPUT);

    // initialize motor as disabled
    this->setDisabled();
}

void Motor::setEnabled(bool enabled)
{
    this->enabled = enabled;
}

void Motor::setDisabled()
{
    this->setEnabled(false);
}

bool Motor::getEnabled()
{
    return this->enabled;
}

void Motor::setReversed(bool reversed)
{
    this->reversed = reversed;
}

bool Motor::getReversed()
{
    return this->reversed;
}

void Motor::setSpeed(float speed)
{
    // cap speed between -1.0 and 1.0
    if (speed > 1.0)
    {
        speed = 1.0;
    }
    else if (speed < -1.0)
    {
        speed = -1.0;
    }

    this->speed = speed;
}

float Motor::getSpeed()
{
    return this->speed;
}
