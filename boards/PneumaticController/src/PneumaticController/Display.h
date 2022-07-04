#ifndef DISPLAY_H
#define DISPLAY_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "PressureTransducer.h"
#include "Solenoid.h"
#include "State.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

//TODO display singleton
//TODO decouple hardware from class

class Graph {
  private:
    double buffer[SCREEN_WIDTH];

    double scale;
    uint8_t y_offset;

    void shiftBuffer(uint16_t amount = 1) {
      for (uint16_t i = 1; i < SCREEN_WIDTH; i++) {
        this->buffer[i - amount] = this->buffer[i];
      }
    }

    void clearBuffer() {
      for (uint16_t i = 0; i < SCREEN_WIDTH; i++) {
        this->buffer[i] = 0;
      }
    }
  public:
    Graph(double scale, uint8_t y_offset) {
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
      this->buffer[SCREEN_WIDTH - 1] = value;
    }

    void draw(Adafruit_SSD1306 *display) {
      for (uint16_t i = 0; i < SCREEN_WIDTH; i++) {
        double value = this->buffer[i];
        display->drawPixel(i, -1 * round(value * this->scale) + this->y_offset, SSD1306_WHITE);
      }
    }

};

class Display {
  private:
    Adafruit_SSD1306 *display;

    PressureTransducer *pt;
    State *state;
    Graph *graph;

  public:
    Display() {
      this->display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

      // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
      if (!this->display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        return;
      }

      // Clear the buffer
      this->display->clearDisplay();
    }

    void setStateMachine(State *state) {
      this->state = state;
    }

    void setPressureTransducer(PressureTransducer *pt) {
      this->pt = pt;
    }

    void setGraph(Graph *graph) {
      this->graph = graph;
    }

    void update() {
      // Clear the buffer
      this->display->clearDisplay();

      this->display->setTextSize(1);      // Normal 1:1 pixel scale
      this->display->setTextColor(SSD1306_WHITE); // Draw white text
      this->display->cp437(true);         // Use full 256 char 'Code Page 437' font

      //      this->display->setCursor(0, 0);     // Start at top-left corner
      //      this->display->write("RAW: ");
      //      this->display->write(String(this->pt->getRaw()).c_str());
      //
      //      this->display->setCursor(0, 1 * 11);
      //      this->display->write("VOLT: ");
      //      this->display->write(String(this->pt->getVoltage()).c_str());

      this->display->setCursor(0, 0 * 11);
      this->display->write("State: ");
      this->display->write(String(this->state->getState()).c_str());

      this->display->setCursor(0, 1 * 11);
      this->display->write("PSI: ");
      this->display->write(String(this->pt->getPressurePSI()).c_str());

      this->graph->draw(this->display);

      this->display->display();
    }
};

#endif
