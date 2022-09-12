#ifndef ROBOT_BOARDS_STATE_LED_H
#define ROBOT_BOARDS_STATE_LED_H

#include <Adafruit_NeoPixel.h>

#include "./state-machine.h"

// Minimum brightness of the LED
#define MIN_BRIGHTNESS 10
// Maximum brightness of the LED
#define MAX_BRIGHTNESS 30

// How fast the LED should blink for the pulse effect
#define PULSE_RATE 2 // Hz

// Define colors used by the LED
#define COLOR_RED 0xFF0000
#define COLOR_BLUE 0x0000FF
#define COLOR_GREEN 0x00FF00
#define COLOR_ORANGE 0xFF7700
#define COLOR_PURPLE 0xFF0077

/**
 * @brief State LED object to interface with the onboard state indicator RGB LED
 */
class StateLED
{
private:
    // NeoPixel instance to interface with the addressable RGB LEDs
    Adafruit_NeoPixel *leds;

    // State Machine instance tracking boards state
    StateMachine *state;

    // Should the LED have the pulse effect?
    bool pulse;

    // Define State colors
    uint32_t COLOR_FAULT = COLOR_RED;
    uint32_t COLOR_RUNNING = COLOR_BLUE;
    uint32_t COLOR_IDLE = COLOR_GREEN;
    uint32_t COLOR_HALT = COLOR_ORANGE;
    uint32_t COLOR_STARTING = COLOR_PURPLE;

    /**
     * @brief Map a float value from one range to another
     *
     * @param val Input value to map
     * @param from_low Low end of the range to map from
     * @param from_high High end of the range to map from
     * @param to_low Low end of the range to map to
     * @param to_high High end of the range to map to
     * @return float Mapped output value
     */
    static float fmap(float val, float from_low, float from_high, float to_low, float to_high)
    {
        return (val - from_low) * ((to_high - to_low) / (from_high - from_low)) + to_low;
    }

    /**
     * @brief Set the brightness of the LED
     *
     * @param brightness Brightness level to set the LEDs
     */
    void setBrightness(uint8_t brightness);

    /**
     * @brief Set the color of the LED
     *
     * @param color Color to set the LEDs
     */
    void setColor(uint32_t color);

    /**
     * @brief Update the LED with the latest brightness and color
     */
    void show();

public:
    /**
     * @brief Construct a new StateLED object
     *
     * @param state Board State Machine instance
     */
    StateLED(StateMachine *state);

    /**
     * @brief Initialize the State LED
     *
     * @tparam PIN Hardware pin the LED is connected to
     * @param num_leds Optional number of LEDs inline
     */
    template <uint8_t PIN>
    void init(uint8_t num_leds = 1);

    /**
     * @brief Update the LED with the latest state and effect
     *
     * @param offset Time offset in ms to sync with the controller
     */
    void update(int32_t offset = 0);
};

template <uint8_t PIN>
void StateLED::init(uint8_t num_leds)
{
    // Initialize NeoPixel addressable RGB LEDs
    this->leds = new Adafruit_NeoPixel(num_leds, PIN, NEO_GRB + NEO_KHZ800);
    this->leds->begin();
    this->leds->clear();

    // Initialize LED with brightness and state
    this->setBrightness(MAX_BRIGHTNESS);
    this->update();
}

#endif
