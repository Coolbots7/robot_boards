#ifndef _ROBOT_BOARDS_COMMUNICATION_TWO_WIRE_COMMON_H_
#define _ROBOT_BOARDS_COMMUNICATION_TWO_WIRE_COMMON_H_

#include "Arduino.h"

/**
 * @brief Struct to hold a message for TwoWire communication
 */
struct WireMessage
{
    uint8_t message_type;
    uint8_t data[4];
};

/**
 * @brief Class to implement core functionality for I2C communication
 */
class I2CCore
{
public:
    /**
     * @brief Convert a byte array to a value
     *
     * @param buff Byte array containing an encoded value
     * @return T Value decoded from the byte array
     */
    template <typename T>
    static T decodeValue(uint8_t *buff);

    /**
     * @brief Convert a value to a byte array
     *
     * @param value Value to convert to a byte array
     * @param buff Byte array containing the encoded value
     */
    template <typename T>
    static void encodeValue(T value, uint8_t *buff);
};

template <typename T>
T I2CCore::decodeValue(uint8_t *buff)
{
    T value;
    memcpy(&value, buff, sizeof(T));
    return value;
}

template <typename T>
void I2CCore::encodeValue(T value, uint8_t *buff)
{
    memset(buff, 0, sizeof(T));
    memcpy(buff, &value, sizeof(T));
}

#endif
