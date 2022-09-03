#include <cmath>

#include <Adafruit_NeoPixel.h>

#include "./state-led.h"
#include "./state-machine.h"

StateLED::StateLED(StateMachine *state)
{
    this->state = state;

    this->pulse = false;
}

void StateLED::setBrightness(uint8_t brightness)
{
    // Limit the brightness between the defined min and max
    if (brightness < MIN_BRIGHTNESS)
    {
        brightness = MIN_BRIGHTNESS;
    }
    else if (brightness > MAX_BRIGHTNESS)
    {
        brightness = MAX_BRIGHTNESS;
    }

    this->leds->setBrightness(brightness);
}

void StateLED::setColor(uint32_t color)
{
    this->leds->fill(color);
}

void StateLED::show()
{
    this->leds->show();
}

void StateLED::update(int32_t offset)
{
    // Set the LED color and effect based on the current state of the State Machine
    switch (this->state->getCurrentState())
    {
    case STARTING:
        this->pulse = false;
        setColor(COLOR_STARTING);
        break;
    case HALT:
        this->pulse = true;
        setColor(COLOR_HALT);
        break;
    case IDLE:
        this->pulse = false;
        setColor(COLOR_IDLE);
        break;
    case RUNNING:
        this->pulse = true;
        setColor(COLOR_RUNNING);
        break;
    case FAULT:
        this->pulse = true;
        setColor(COLOR_FAULT);
        break;
    default:
        break;
    }

    // Update the current fade level if the pulse effect is enabled
    if (this->pulse)
    {
        // Calculate the time to fade the led based on the pulse rate
        uint32_t fade_time = 1000UL / PULSE_RATE;

        // Get the remaining fade time of the current cycle
        uint32_t remaining_fade_time = (millis() + offset) % fade_time;

        // Get the position in the wave based on the remaining time
        float current_cycle_position = (((float)fade_time - (float)remaining_fade_time) / (float)fade_time) * (2.0 * M_PI);

        // Get the sin of the current cycle position
        float brightness_multiplier = sin(current_cycle_position);

        // Map the offset sin output value to the LED brightness range, and set brightness
        uint8_t current_brightness_level = StateLED::fmap(brightness_multiplier, -1, 1, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        this->setBrightness(current_brightness_level);
    }
    else
    {
        // Otherwise set max brightness
        this->setBrightness(MAX_BRIGHTNESS);
    }

    // Push changes to LED
    this->show();
}
