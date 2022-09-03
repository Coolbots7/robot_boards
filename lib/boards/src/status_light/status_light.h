#ifndef _ROBOT_BOARDS_BOARDS_STATUS_LIGHT_H_
#define _ROBOT_BOARDS_BOARDS_STATUS_LIGHT_H_

#include "../board.h"

// Status Light 0x50 - 0x53
#define I2C_STATUS_LIGHT_BASE_ADDRESS 0x50
#define I2C_STATUS_LIGHT_ADDRESS_RANGE 4

/**
 * @brief Helper to interface with a Status Light board
 */
class StatusLightInterface : public BoardInterface
{
public:
    /**
     * @brief Construct a new Status Light Interface object
     *
     * @param address_offset Address offset for the Status Light board from the default address
     * @param i2c_master Instance of I2CMaster to interface with the board
     */
    StatusLightInterface(uint8_t address_offset, I2CMaster *i2c_master);
};

#endif
