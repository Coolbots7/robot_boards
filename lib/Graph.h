#ifndef ROBOT_BOARDS_GRAPH_H
#define ROBOT_BOARDS_GRAPH_H

class Graph
{
private:
    double buffer[255];

    uint8_t width;
    double scale;
    uint8_t y_offset;

    void shiftBuffer(uint16_t amount);

    void clearBuffer();

public:
    Graph(uint8_t width, double scale, uint8_t y_offset);

    void setScale(double scale);

    void setYOffset(uint8_t offset);

    void clear();

    void push(double value);

    void draw(Adafruit_SSD1306 *display);
};

#endif