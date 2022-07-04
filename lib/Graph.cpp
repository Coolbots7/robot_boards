#include "Graph.h"

Graph::Graph(uint8_t width, double scale, uint8_t y_offset)
{
    this->width = width;
    this->scale = scale;
    this->y_offset = y_offset;

    clearBuffer();
}

void Graph::shiftBuffer(uint16_t amount = 1)
{
    for (uint16_t i = 1; i < this->width; i++)
    {
        this->buffer[i - amount] = this->buffer[i];
    }
}

void Graph::clearBuffer()
{
    for (uint16_t i = 0; i < this->width; i++)
    {
        this->buffer[i] = 0;
    }
}

void Graph::setScale(double scale)
{
    this->scale = scale;
}

void Graph::setYOffset(uint8_t offset)
{
    this->y_offset = offset;
}

void Graph::clear()
{
    this->clearBuffer();
}

void Graph::push(double value)
{
    this->shiftBuffer();
    this->buffer[this->width - 1] = value;
}

void Graph::draw(Adafruit_SSD1306 *display)
{
    for (uint16_t i = 0; i < this->width; i++)
    {
        double value = this->buffer[i];
        display->drawPixel(i, -1 * round(value * this->scale) + this->y_offset, SSD1306_WHITE);
    }
}