#ifndef _ROBOT_BOARDS_STATUS_LIGHT_I2C_H_
#define _ROBOT_BOARDS_STATUS_LIGHT_I2C_H_

#include "state-machine.h"
#include "status_light/status_light.h"
#include "twowire/device.h"

/**
 * Status Light I2C Slave
 *
 * Helper for the Status Light board to be communicated with over the TwoWire protocol
 */
class StatusLightI2CSlave : public I2CDevice
{
public:
    /**
     * @brief Construct a new Status Light I2C Slave object
     *
     * @param address_offset Address offset for the Status Light board from the default address
     * @param state Instance of the State Machine for the board
     */
    StatusLightI2CSlave(uint8_t address_offset, StateMachine *state) : I2CDevice(I2C_STATUS_LIGHT_BASE_ADDRESS + address_offset, state) {}

    /**
     * @brief Function to process TwoWire on receive events for the Status Light board
     *
     * @param num_bytes Number of bytes received
     */
    void onReceive(int num_bytes);

    /**
     * @brief Function to process TwoWire on request events for the Status Light board
     */
    void onRequest();
};

#endif
