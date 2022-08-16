#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include "./display.h"

Display::Display(Adafruit_SSD1306 *screen, void (*displayHandler)(Display *), double font_scale)
{
    this->screen = screen;
    this->displayHandler = displayHandler;
    this->font_scale = font_scale;

    // Set rotation
    this->rotation(0);

    // Use full 256 char 'Code Page 437' font
    this->screen->cp437(true);

    // Set text color
    this->screen->setTextColor(SSD1306_WHITE);

    this->initializing("Display...", 0.5f);
}

String Display::progressBar(uint8_t length, double progress)
{
    // Create bar with filled length equivalent to progress
    String progress_bar = "[";
    for (int i = 0; i < length; i++)
    {
        progress_bar += (i < round(length * progress) ? "=" : " ");
    }
    progress_bar += "]";

    return progress_bar;
}

void Display::update()
{
    // Call provided function to update the display during the main loop
    this->displayHandler(this);
}

void Display::clear()
{
    // Erase the screen buffer
    this->screen->clearDisplay();
}

void Display::display()
{
    // Update screen with current buffer
    this->screen->display();
}

void Display::rotation(uint8_t rotation)
{
    // Set the screens rotation
    this->screen->setRotation(rotation);
}

uint8_t Display::getWidth()
{
    return this->screen->width();
}

uint8_t Display::getHeight()
{
    return this->screen->height();
}

void Display::setFontScale(double font_scale)
{
    this->font_scale = font_scale;
    this->screen->setTextSize(this->font_scale);
}

double Display::getFontWidth()
{
    return FONT_WIDTH * this->font_scale;
}

double Display::getFontHeight()
{
    return FONT_HEIGHT * this->font_scale;
}

uint8_t Display::getLineY(uint8_t line)
{
    return (line * this->getFontHeight()) + LINE_SPACE;
}

void Display::initializing(String message, double progress = 0.0f)
{
    // Limit progress between 0 and 1
    if (progress < 0.0f)
        progress = 0.0f;
    if (progress > 1.0f)
        progress = 1.0f;

    this->clear();
    this->horizontal(this->getLineY(0), "Initializing...");
    this->horizontal(this->getLineY(1), message);
    this->horizontal(this->getLineY(2), progressBar(PROGRESS_BAR_WIDTH, progress));
    this->display();
}

void Display::write(uint8_t x, uint8_t y, String message)
{
    this->screen->setCursor(x, y);
    this->screen->write(message.c_str());
}

void Display::leftJustify(uint8_t y, String message)
{
    this->write(0, y, message);
}

void Display::rightJustify(uint8_t y, String message)
{
    // Calculate the x position to write the message based on the screen width, message length, and font size
    uint8_t x = this->getWidth() - (this->getFontWidth() * message.length());
    this->write(x, y, message);
}

void Display::horizontal(int y, String message)
{
    // Calculate the x position to write the message based on the screen width, message length, and font size
    int x = (this->getWidth() / 2) - ((message.length() * this->getFontWidth()) / 2);
    this->write(x, y, message);
}
