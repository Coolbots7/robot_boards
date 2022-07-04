#ifndef STATE_LED_H
#define STATE_LED_H

#include <FastLED.h>

#include "State.h"

#define NUM_LEDS  1

#define MIN_BRIGHTNESS 10
#define BRIGHTNESS 100

#define FADE_SPEED 5
#define FADE_UP false
#define FADE_DOWN true

uint32_t COLOR_FAULT = 0xFF0000;
uint32_t COLOR_RUNNING = 0xFF7700;
uint32_t COLOR_IDLE = 0x0000FF;
uint32_t COLOR_STARTING = 0xFF0077;

//TODO state led singleton

class StateLED {
  private:
    uint8_t pin;

    CRGB* leds;

    State *state;

    bool pulse;
    bool pulse_latch;

    uint8_t current_brightness;

    setColor(uint32_t color) {
      for (int i = 0; i < NUM_LEDS; i++) {
        this->leds[i] = CRGB(color);
      }
    }

  public:
    StateLED(uint8_t pin, State *state) {
      this->pin = pin;
      this->state = state;

      this->pulse = false;
      this->pulse_latch = FADE_DOWN;

      this->current_brightness = BRIGHTNESS;

      //Initialize led color array
      leds = new CRGB[NUM_LEDS];

      //Initialize LEDs
      const int pin_data = this->pin;
      LEDS.addLeds<WS2812, 4, GRB>(leds, NUM_LEDS);
      LEDS.setBrightness(BRIGHTNESS);
    }

    void update() {
      switch (this->state->getState()) {
        case STARTING:
          this->pulse = true;
          setColor(COLOR_STARTING);
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
          this->pulse = false;
          setColor(COLOR_FAULT);
          break;
        default:
          //TODO debug
          break;
      }

      if (this->pulse) {
        if (this->current_brightness <= MIN_BRIGHTNESS) {
          this->current_brightness = MIN_BRIGHTNESS;
          this->pulse_latch = FADE_UP;
        }

        if(this->current_brightness >= BRIGHTNESS) {
          this->current_brightness = BRIGHTNESS;
          this->pulse_latch = FADE_DOWN;
        }

        if (this->pulse_latch == FADE_DOWN) {
          this->current_brightness -= FADE_SPEED;
        }
        else {
          this->current_brightness += FADE_SPEED;
        }

        LEDS.setBrightness(this->current_brightness);
      }
      else {
        LEDS.setBrightness(BRIGHTNESS);
      }

      
      FastLED.show();
    }
};

#endif
