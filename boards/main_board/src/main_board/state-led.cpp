#include <Adafruit_NeoPixel.h>

#include "./state-led.h"
#include "./state-machine.h"

StateLED::StateLED(StateMachine *state)
{
    this->state = state;

    this->pulse = false;
    this->pulse_latch = FADE_DOWN;
    this->previous_remaining_fade_time = 0;
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

void StateLED::update()
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
        // Calculate the time to fade the led in one direction
        uint32_t fade_direction_time = round((1000 * (1 / PULSE_RATE)) / 2);

        // Get the remainder of the current time divided by the fade time
        uint32_t remaining_fade_time = millis() % fade_direction_time;

        // If the remaining fade time has crossed 0 then flip the fade direction
        if (remaining_fade_time < this->previous_remaining_fade_time)
        {
            this->pulse_latch = !this->pulse_latch;
        }
        this->previous_remaining_fade_time = remaining_fade_time;

        // Calculate and set the current brightness using the remaining fade time
        uint8_t current_brightness_level = map(remaining_fade_time, 0, fade_direction_time, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        if (this->pulse_latch == FADE_DOWN)
        {
            // Invert brightness when fading down
            this->setBrightness(MAX_BRIGHTNESS - current_brightness_level);
        }
        else
        {
            this->setBrightness(current_brightness_level);
        }
    }
    else
    {
        // Otherwise set max brightness
        this->setBrightness(MAX_BRIGHTNESS);
    }

    // Push changes to LED
    this->show();
}
