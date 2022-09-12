#ifndef _ROBOT_BOARDS_BOARDS_BOARD_H_
#define _ROBOT_BOARDS_BOARDS_BOARD_H_

#include "twowire/master.h"

#include "./common/state.h"

/**
 * @brief Base class to represent a Robot Board to interface with
 *
 */
class BoardInterface
{
protected:
    /**
     * @brief Construct a new Board Interface object
     *
     * @param address Address of the board
     * @param i2c_master Instance of I2CMaster to communicate with the board
     */
    BoardInterface(uint8_t address, I2CMaster *i2c_master);

public:
    // Public instance of the Board State Interface utility
    BoardStateInterface *state;
};

#endif
