#ifndef _ROBOT_BOARDS_STATUS_LIGHT_I2C_H_
#define _ROBOT_BOARDS_STATUS_LIGHT_I2C_H_

#include "state-machine.h"
#include "twowire/common.h"
#include "twowire/slave.h"

/**
 * @brief Helper for the Status Light board to be communicated with over the TwoWire protocol
 */
class StatusLightI2CSlave
{
private:
    // Master state register value
    State master_state;

    // Master time register value
    uint32_t master_time;

    // Master time offset register value
    int32_t master_offset;

    // Instance of the State Machine for the board
    StateMachine *state;

    // Instance of the I2CSlave to use for communication
    I2CSlave *i2c_slave;

public:
    /**
     * @brief Construct a new Status Light I2C Slave object
     *
     * @param address_offset Address offset for the Status Light board from the default address
     * @param state Instance of the State Machine for the board
     */
    StatusLightI2CSlave(uint8_t address_offset, StateMachine *state);

    /**
     * @brief Initialize the Status Light I2CSlave and register the TwoWire handlers
     *
     * @param onReceive Handler for the TwoWire on receive event
     * @param onRequest Handler for the TwoWire on request event
     */
    void begin(void (*onReceive)(int), void (*onRequest)(void));

    /**
     * @brief Handle TwoWire on receive events for the Status Light board
     *
     * @param num_bytes Number of bytes received
     */
    void onReceive(int num_bytes);

    /**
     * @brief Handle TwoWire on request events for the Status Light board
     */
    void onRequest();

    /**
     * @brief Get the Master State register value
     *
     * @return State Current Master State register value
     */
    State getMasterState();

    /**
     * @brief Get the Master Time register value
     *
     * @return uint32_t Current Master Time register value
     */
    uint32_t getMasterTime();

    /**
     * @brief Get the Master Time Offset register value
     *
     * @return int32_t Current Master Time Offset register value
     */
    int32_t getMasterOffset();
};

#endif
