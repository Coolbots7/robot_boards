#ifndef DISPLAY_H
#define DISPLAY_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "State.h"
#include "Loop.h"

#define PROGRESS_BAR_WIDTH 10
#define TEXT_SIZE 1      // Normal 1:1 pixel scale
#define FONT_WIDTH 6
#define FONT_HEIGHT 11

#define INITIALIZING_TIMEOUT 100

#define SIGNAL_STRENGTH_INCREMENT 2

#define ESTOP_STATE_RADIUS 10

#define MAX_GRAPH_WIDTH 255

#define BATTERY_RECT_RADIUS 2
#define BATTERY_WIDTH 13
#define BATTERY_HEIGHT 7

//TODO display singleton

class Graph {
  private:
    double buffer[255];

    uint8_t width;
    double scale;
    uint8_t y_offset;

    void shiftBuffer(uint16_t amount = 1) {
      for (uint16_t i = 1; i < this->width; i++) {
        this->buffer[i - amount] = this->buffer[i];
      }
    }

    void clearBuffer() {
      for (uint16_t i = 0; i < this->width; i++) {
        this->buffer[i] = 0;
      }
    }
  public:
    Graph(uint8_t width, double scale, uint8_t y_offset) {
      this->width = width;
      this->scale = scale;
      this->y_offset = y_offset;

      clearBuffer();
    }

    void setScale(double scale) {
      this->scale = scale;
    }

    void setYOffset(uint8_t offset) {
      this->y_offset = offset;
    }

    void clear() {
      this->clearBuffer();
    }

    void push(double value) {
      this->shiftBuffer();
      this->buffer[this->width - 1] = value;
    }

    void draw(Adafruit_SSD1306 *display) {
      for (uint16_t i = 0; i < this->width; i++) {
        double value = this->buffer[i];
        display->drawPixel(i, -1 * round(value * this->scale) + this->y_offset, SSD1306_WHITE);
      }
    }

};

class Display {
  private:
    Adafruit_SSD1306 *display;
    uint8_t width;
    uint8_t height;

    unsigned long initializing_last_update;

    State *state;

    Loop *estop_loop;

    void write(uint8_t x, uint8_t y, double size, String message) {
      this->display->setTextSize(size);
      this->display->setCursor(x, y);
      this->display->write(message.c_str());
    }

    void horizontal(String message, int y) {
      int x = (this->width / 2) - ((message.length() * (TEXT_SIZE * FONT_WIDTH)) / 2);
      this->write(x, y, TEXT_SIZE, message);
    }

    String progressBar(uint8_t length, double progress) {
      String progress_bar = "[";
      for (int i = 0; i < length; i++) {
        if (i < round(length * progress)) {
          progress_bar += "=";
        }
        else {
          progress_bar += " ";
        }
      }
      progress_bar += "]";

      return progress_bar;
    }

    void drawSignalStrength(uint8_t x, uint8_t y, double strength) {
      for (uint8_t i = 0; i < 4; i++) {
        //TODO strength
        this->display->drawFastVLine(x + (i * 2), y + 9 - (SIGNAL_STRENGTH_INCREMENT * (i + 1)), (SIGNAL_STRENGTH_INCREMENT * (i + 1)), SSD1306_WHITE);
      }
    }

    void drawEStopState(uint8_t x, uint8_t y, bool state) {
      if (state) {
        this->display->fillCircle(x, y, ESTOP_STATE_RADIUS, SSD1306_INVERSE);
      }
      else {
        this->display->drawCircle(x, y, ESTOP_STATE_RADIUS, SSD1306_WHITE);
      }
    }

    void drawBattery(uint8_t x, uint8_t y, double percentage) {
      //Limit percentage between 0 and 1
      if (percentage < 0.0f)
        percentage = 0.0f;
      if (percentage > 1.0f)
        percentage = 1.0f;

      this->display->drawRoundRect(x, y, BATTERY_WIDTH, BATTERY_HEIGHT, BATTERY_RECT_RADIUS, SSD1306_WHITE);
      this->display->fillRoundRect(x, y, BATTERY_WIDTH * percentage, BATTERY_HEIGHT, BATTERY_RECT_RADIUS, SSD1306_WHITE);
    }

  public:
    Display(Adafruit_SSD1306 *display, uint8_t width, uint8_t height) {
      this->display = display;
      this->width = width;
      this->height = height;

      this->initializing_last_update = 0;

      // Clear the buffer
      this->display->clearDisplay();

      // Set defaults
      this->display->cp437(true);         // Use full 256 char 'Code Page 437' font
      this->display->setTextColor(SSD1306_WHITE); // Draw white text


      this->horizontal("Wireless EStop", 0 * 11);
      this->horizontal("HW: ", 1 * FONT_HEIGHT);
      this->horizontal("FW: ", 2 * FONT_HEIGHT);
      this->display->display();
    }

    void initializing(String message, double progress = 0.0f) {
      // Only update progress at most every INITIALIZING_TIMEOUTms for performance
      if (millis() - this->initializing_last_update < INITIALIZING_TIMEOUT)
        return;

      //Limit progress between 0 and 1
      if (progress < 0.0f)
        progress = 0.0f;
      if (progress > 1.0f)
        progress = 1.0f;

      this->display->clearDisplay();
      this->horizontal("Initializing...", 0 * FONT_HEIGHT);
      this->horizontal(message, 1 * FONT_HEIGHT);
      this->horizontal(progressBar(PROGRESS_BAR_WIDTH, progress), 2 * FONT_HEIGHT);
      this->display->display();

      this->initializing_last_update = millis();
    }

    void setStateMachine(State *state) {
      this->state = state;
    }

    void setLoop(Loop *estop_loop) {
      this->estop_loop = estop_loop;
    }

    void update() {
      this->display->clearDisplay();

      //TODO total loop state
      //TODO graphical representation
      //      this->horizontal(String(this->estop_loop->getLoopSafe() ? "SAFE" : "ENERGIZED"), this->height - 1 * FONT_HEIGHT);
      this->drawEStopState(this->width / 2, this->height / 2, this->estop_loop->getLoopSafe());

      //TODO connected remotes

      //TODO robot state

      this->display->display();
    }
};

#endif
