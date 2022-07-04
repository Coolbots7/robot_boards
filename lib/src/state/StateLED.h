#ifndef STATE_LED_H
#define STATE_LED_H

#include <math.h>

// #include <FastLED.h>
#include <Adafruit_NeoPixel.h>

#include "./State.h"

// #define LED_TYPE WS2811
// #define COLOR_ORDER GRB
#define NUM_LEDS 1

#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 50

#define PULSE_RATE 2 // hz
#define FADE_UP false
#define FADE_DOWN true

#define COLOR_RED 0xFF0000
#define COLOR_BLUE 0x0000FF
#define COLOR_GREEN 0x00FF00
#define COLOR_ORANGE 0xFF7700
#define COLOR_PURPLE 0xFF0077

uint32_t COLOR_FAULT = COLOR_RED;
uint32_t COLOR_RUNNING = COLOR_BLUE;
uint32_t COLOR_IDLE = COLOR_GREEN;
uint32_t COLOR_HALT = COLOR_ORANGE;
uint32_t COLOR_STARTING = COLOR_PURPLE;

class StateLED
{
private:
  uint8_t pin;
  uint8_t num_leds;

  Adafruit_NeoPixel *leds;
  // CRGB *leds;

  State *state;

  bool pulse;
  bool pulse_latch;

  uint8_t current_brightness;
  uint8_t prevRemainingFadeTime;

  void setBrightness(uint8_t brightness)
  {
    if (brightness < MIN_BRIGHTNESS)
    {
      brightness = MIN_BRIGHTNESS;
    }
    else if (brightness > MAX_BRIGHTNESS)
    {
      brightness = MAX_BRIGHTNESS;
    }

    this->current_brightness = brightness;

    // LEDS.setBrightness(this->current_brightness);
    this->leds->setBrightness(this->current_brightness);
  }

  void setColor(uint32_t color)
  {
    for (int i = 0; i < this->num_leds; i++)
    {
      // this->leds[i] = CRGB(color);
      this->leds->setPixelColor(i, color);
    }
  }

  void show()
  {
    // FastLED.show();
    this->leds->show();
  }

public:
  StateLED(State *state)
  {
    this->state = state;

    this->pulse = false;
    this->pulse_latch = FADE_DOWN;
    this->prevRemainingFadeTime = 0;
  }

  template <uint8_t PIN>
  void init(uint8_t num_leds = 1)
  {
    this->pin = PIN;
    this->num_leds = num_leds;

    // Initialize led color array
    //  this->leds = new CRGB[this->num_leds];
    this->leds = new Adafruit_NeoPixel(num_leds, PIN, NEO_GRB + NEO_KHZ800);
    this->leds->begin();
    this->leds->clear();

    // Initialize LEDs
    //  LEDS.addLeds<LED_TYPE, PIN, COLOR_ORDER>(this->leds, this->num_leds);

    this->setBrightness(MAX_BRIGHTNESS);
    this->setColor(COLOR_STARTING);
    this->show();
  }

  void update(uint32_t masterTime = 0)
  {
    switch (this->state->getCurrentState())
    {
    case STARTING:
      this->pulse = true;
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
      this->pulse = false;
      setColor(COLOR_FAULT);
      break;
    default:
      // TODO debug
      break;
    }

    if (this->pulse)
    {
      // TODO heartbeat synchronized pulse

      // TODO time based fade
      // uint32_t fadeDirectionTime = round((1000 * (1/PULSE_RATE)) / 2);
      uint32_t fadeDirectionTime = 250;

      // if time was not provided, use onboard time
      if (masterTime <= 0)
      {
        masterTime = millis();
      }

      uint32_t remainingFadeTime = masterTime % fadeDirectionTime;

      if (remainingFadeTime < this->prevRemainingFadeTime)
      {
        this->pulse_latch = !this->pulse_latch;
      }
      this->prevRemainingFadeTime = remainingFadeTime;

      // set current brightness
      uint8_t currentBrightnessLevel = map(remainingFadeTime, 0, fadeDirectionTime, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
      if (this->pulse_latch == FADE_DOWN)
      {
        this->setBrightness(MAX_BRIGHTNESS - currentBrightnessLevel);
      }
      else
      {
        this->setBrightness(currentBrightnessLevel);
      }
    }
    else
    {
      this->setBrightness(MAX_BRIGHTNESS);
    }

    this->show();
  }
};

#endif