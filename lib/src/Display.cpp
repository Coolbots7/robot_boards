#include "./Display.h"

Display::Display(String boardName, Adafruit_SSD1306 *screen, uint8_t width, uint8_t height, void (*display)(Display *, Adafruit_SSD1306 *))
{
    this->screen = screen;
    this->width = width;
    this->height = height;
    this->display = display;

    this->initializing_last_update = 0;

    // Clear the buffer
    this->screen->clearDisplay();

    // Set defaults
    this->screen->cp437(true);                 // Use full 256 char 'Code Page 437' font
    this->screen->setTextColor(SSD1306_WHITE); // Draw white text

    this->horizontal(boardName, 0 * 11);
    // TODO hardware version
    this->horizontal("HW: ", 1 * FONT_HEIGHT);
    // TODO firmware version
    this->horizontal("FW: ", 2 * FONT_HEIGHT);
    this->screen->display();
}

String Display::progressBar(uint8_t length, double progress)
{
    String progress_bar = "[";
    for (int i = 0; i < length; i++)
    {
        if (i < round(length * progress))
        {
            progress_bar += "=";
        }
        else
        {
            progress_bar += " ";
        }
    }
    progress_bar += "]";

    return progress_bar;
}

// void Display::drawSignalStrength(uint8_t x, uint8_t y, double strength)
// {
//     for (uint8_t i = 0; i < 4; i++)
//     {
//         //TODO strength
//         this->screen->drawFastVLine(x + (i * 2), y + 9 - (SIGNAL_STRENGTH_INCREMENT * (i + 1)), (SIGNAL_STRENGTH_INCREMENT * (i + 1)), SSD1306_WHITE);
//     }
// }

// void Display::drawEStopState(uint8_t x, uint8_t y, bool state)
// {
//     if (state)
//     {
//         this->screen->fillCircle(x, y, ESTOP_STATE_RADIUS, SSD1306_INVERSE);
//     }
//     else
//     {
//         this->screen->drawCircle(x, y, ESTOP_STATE_RADIUS, SSD1306_WHITE);
//     }
// }

// void Display::drawBattery(uint8_t x, uint8_t y, double percentage)
// {
//     //Limit percentage between 0 and 1
//     if (percentage < 0.0f)
//         percentage = 0.0f;
//     if (percentage > 1.0f)
//         percentage = 1.0f;

//     this->screen->drawRoundRect(x, y, BATTERY_WIDTH, BATTERY_HEIGHT, BATTERY_RECT_RADIUS, SSD1306_WHITE);
//     this->screen->fillRoundRect(x, y, BATTERY_WIDTH * percentage, BATTERY_HEIGHT, BATTERY_RECT_RADIUS, SSD1306_WHITE);
// }

void Display::initializing(String message, double progress = 0.0f)
{
    // Only update progress at most every INITIALIZING_TIMEOUTms for performance
    if (millis() - this->initializing_last_update < INITIALIZING_TIMEOUT)
        return;

    // Limit progress between 0 and 1
    if (progress < 0.0f)
        progress = 0.0f;
    if (progress > 1.0f)
        progress = 1.0f;

    this->screen->clearDisplay();
    this->horizontal("Initializing...", 0 * FONT_HEIGHT);
    this->horizontal(message, 1 * FONT_HEIGHT);
    this->horizontal(progressBar(PROGRESS_BAR_WIDTH, progress), 2 * FONT_HEIGHT);
    this->screen->display();

    this->initializing_last_update = millis();
}

void Display::update()
{
    if (this->display)
    {
        this->display(this, this->screen);
    };
}

void Display::clear()
{
    this->screen->clearDisplay();
    this->screen->display();
}

void Display::write(uint8_t x, uint8_t y, double size, String message)
{
    this->screen->setTextSize(size);
    this->screen->setCursor(x, y);
    this->screen->write(message.c_str());
}

void Display::leftJustify(uint8_t y, float size, String message)
{
    this->write(0, y, size, message);
}

void Display::horizontal(String message, int y)
{
    int x = (this->width / 2) - ((message.length() * (TEXT_SIZE * FONT_WIDTH)) / 2);
    this->write(x, y, TEXT_SIZE, message);
}

// void Display::center(String message)
// {
//     this->screen->clearDisplay();
//     uint8_t y = this->height / (TEXT_SIZE * FONT_HEIGHT) / 2;
//     this->horizontal(message, y);
//     this->screen->display();
// }

void Display::rightJustify(uint8_t y, float size, String message)
{
    // TODO get width / height based on rotation
    uint8_t width = this->width;
    uint8_t x = width - ((FONT_WIDTH * size) * message.length());
    this->write(x, y, size, message);
}