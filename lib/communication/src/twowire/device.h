#ifndef _ROBOT_BOARDS_COMMUNICATION_TWO_WIRE_DEVICE_H_
#define _ROBOT_BOARDS_COMMUNICATION_TWO_WIRE_DEVICE_H_

#include "state-machine.h"
#include "twowire/slave.h"

class I2CDevice
{
protected:
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

    /**
     * @brief Construct a new I2CDevice object
     *
     * @param address Address of the board
     * @param state Instance of the State Machine for the board
     */
    I2CDevice(uint8_t address, StateMachine *state);

public:
    /**
     * @brief Initialize the Device I2CSlave and register the TwoWire handlers
     *
     * @param onReceive Handler for the TwoWire on receive event
     * @param onRequest Handler for the TwoWire on request event
     */
    void begin(void (*onReceive)(int), void (*onRequest)(void));

    /**
     * @brief Handle TwoWire on receive events for the board
     *
     * @param num_bytes Number of bytes received
     */
    virtual void onReceive(int num_bytes) = 0;

    /**
     * @brief Handle TwoWire on request events for the board
     */
    virtual void onRequest() = 0;

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
