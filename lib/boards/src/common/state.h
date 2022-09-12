#ifndef _ROBOT_BOARDS_BOARD_STATE_H_
#define _ROBOT_BOARDS_BOARD_STATE_H_

#include "state-machine.h"
#include "twowire/master.h"

/**
 * @brief State registers used across all boards
 */
enum StateRegisters
{
    HEARTBEAT_STATE = 0xFC,
    HEARTBEAT_TIME = 0xFD,
    STATE = 0xFE,
};

/**
 * @brief Utility to interface with a boards state machine
 */
class BoardStateInterface
{
private:
    // Address of the board
    uint8_t address;

    // Instance of I2CMaster to communicate with the board
    I2CMaster *i2c_master;

public:
    /**
     * @brief Construct a new Board State Interface object
     *
     * @param address Address of the board
     * @param i2c_master Instance of I2CMaster to communicate with the board
     */
    BoardStateInterface(uint8_t address, I2CMaster *i2c_master);

    /**
     * @brief Get the current state of the board
     *
     * @return State
     */
    State getBoardState();

    /**
     * @brief Set the masters state register on the board
     *
     * @param state Master state to set on the boards register
     */
    void setMasterState(State state);

    /**
     * @brief Set the masters time register on the board
     *
     * @param time Master time to set on the boards register
     */
    void setMasterTime(uint32_t time);
};

#endif
