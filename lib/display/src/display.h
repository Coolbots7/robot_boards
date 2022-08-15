#ifndef ROBOT_BOARDS_DISPLAY_H
#define ROBOT_BOARDS_DISPLAY_H

#include <SPI.h>
#include <Wire.h>

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

// How wide the progress bar is in characters
#define PROGRESS_BAR_WIDTH 10

// Default font sizes in pixels at font scale 1
#define FONT_WIDTH 6
#define FONT_HEIGHT 10

// Spacing between lines in pixels
#define LINE_SPACE 1

/**
 * @brief Display object to interface with the onboard debug screen
 */
class Display
{
private:
    // SSD1306 display driver instance
    Adafruit_SSD1306 *screen;

    // Multiplier of the default font size
    double font_scale;

    // Provided display handler function called in the main loop
    void (*displayHandler)(Display *);

    /**
     * @brief Create a progress bar
     *
     * @param length How wide in characters the progress bar should be
     * @param progress Floating point progress
     * @return Progress bar string
     */
    static String progressBar(uint8_t length, double progress);

public:
    /**
     * @brief Construct a new Display object
     *
     * @param screen SSD1306 driver instance
     * @param displayHandler Display update function
     * @param font_scale Optional initial font scale
     */
    Display(Adafruit_SSD1306 *screen, void (*displayHandler)(Display *), double font_scale = 1.0f);

    /**
     * @brief Update the screen content
     */
    void update();

    /**
     * @brief Clear the screen buffer
     */
    void clear();

    /**
     * @brief Push screen buffer to the display
     */
    void display();

    /**
     * @brief Set the rotation of the display
     *
     * @param rotation Rotation of the display
     */
    void rotation(uint8_t rotation);

    /**
     * @brief Get width of the display based on rotation
     *
     * @return Width in pixels based on rotation
     */
    uint8_t getWidth();

    /**
     * @brief Get height of the display based on rotation
     *
     * @return Height in pixels based on rotation
     */
    uint8_t getHeight();

    /**
     * @brief Set the font scale of the display
     *
     * @param font_scale Floating point scale
     */
    void setFontScale(double font_scale);

    /**
     * @brief Get the font scale of the display
     *
     * @return Floating point scale
     */
    double getFontScale();

    /**
     * @brief Get the font width in pixels based on the font scale
     *
     * @return Font width in pixels
     */
    double getFontWidth();

    /**
     * @brief Get the font height in pixels based on the font scale
     *
     * @return Font height in pixels
     */
    double getFontHeight();

    /**
     * @brief Get the Y coordinate for a line on the display based on the font scale
     *
     * @param line Line number on the display
     * @return Y coordinate in pixels
     */
    uint8_t getLineY(uint8_t line);

    /**
     * @brief Show the progress of the board setup
     *
     * @param message String to label the current initialization step
     * @param progress Amount of initialization progress to show
     */
    void initializing(String message, double progress);

    /**
     * @brief Write a string to the display at coordinates
     *
     * @param x X coordinate in pixels
     * @param y Y coordinate in pixels
     * @param message String to write to the display
     */
    void write(uint8_t x, uint8_t y, String message);

    /**
     * @brief Write a left justified string to the display
     *
     * @param y Y coordinate in pixels
     * @param message String to write to the display
     */
    void leftJustify(uint8_t y, String message);

    /**
     * @brief Write a right justified string to the display
     *
     * @param y Y coordinate in pixels
     * @param message String to write to the display
     */
    void rightJustify(uint8_t y, String message);

    /**
     * @brief Write a horizontally centered string to the display
     *
     * @param y Y coordinate in pixels
     * @param message String to write to the display
     */
    void horizontal(int y, String message);
};

#endif
