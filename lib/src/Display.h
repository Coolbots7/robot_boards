#ifndef ROBOT_BOARDS_DISPLAY_H
#define ROBOT_BOARDS_DISPLAY_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define PROGRESS_BAR_WIDTH 10
// TODO eliminate or use as default param
#define TEXT_SIZE 1 // Normal 1:1 pixel scale
#define FONT_WIDTH 6
#define FONT_HEIGHT 11

#define INITIALIZING_TIMEOUT 100

#define SIGNAL_STRENGTH_INCREMENT 2

#define ESTOP_STATE_RADIUS 10

#define MAX_GRAPH_WIDTH 255

#define BATTERY_RECT_RADIUS 2
#define BATTERY_WIDTH 13
#define BATTERY_HEIGHT 7

class Display
{
private:
  Adafruit_SSD1306 *screen;

  uint8_t width;
  uint8_t height;

  unsigned long initializing_last_update;

  void (*display)(Display *, Adafruit_SSD1306 *);

public:
  Display(String boardName, Adafruit_SSD1306 *screen, uint8_t width, uint8_t height, void (*display)(Display *, Adafruit_SSD1306 *));

  void initializing(String message, double progress);

  void update();

  void clear();
  void write(uint8_t x, uint8_t y, double size, String message);

  void leftJustify(uint8_t y, float size, String message);
  void horizontal(String message, int y);
  // void center(String message);
  void rightJustify(uint8_t y, float size, String message);

  String progressBar(uint8_t length, double progress);
  // void drawSignalStrength(uint8_t x, uint8_t y, double strength);
  // void drawEStopState(uint8_t x, uint8_t y, bool state);
  // void drawBattery(uint8_t x, uint8_t y, double percentage);
};

#endif