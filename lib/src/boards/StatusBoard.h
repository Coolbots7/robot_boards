#ifndef _ROBOT_BOARDS_STATUS_BOARD_H_
#define _ROBOT_BOARDS_STATUS_BOARD_H_

#include "./Board.h"
#include "../communication/CanBus.h"
#include "../communication/I2C.h"

class StatusBoard : public Board
{
public:
    void beginI2C(uint8_t address_offset)
    {
        this->beginI2C(STATUS_LIGHT_BASE_ADDRESS, address_offset);
    }

    void beginCanBus(uint8_t address_offset)
    {
        this->beginCanBus(, address_offset);
    }
};

#endif