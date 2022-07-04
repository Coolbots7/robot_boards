#ifndef ROBOT_BOARDS_ESTOP_BOARD_H
#define ROBOT_BOARDS_ESTOP_BOARD_H

#include "./Board.h"

/**
 *
 */
class EStopBoard : public Board
{
public:
    enum Registers
    {
        // estop loop status
        ESTOP_LOOP_STATUS = 0x00,

        // TODO wireless estop status
        // TODO wireless estop connected
        // TODO wireless estop rssi
    }

    EStopBoard(uint8_t addr) : Base(addr)
    {
    }

    void adoptBoards()
    {
        // TODO search for e stop boards in expected address range
    }

    bool getLoopStatus()
    {
        return this->readBool(EStopBoard::ESTOP_LOOP_STATUS);
    }
};

#endif