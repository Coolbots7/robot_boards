#ifndef _ROBOT_BOARDS_COMMUNICATION_TWO_WIRE_SLAVE_H_
#define _ROBOT_BOARDS_COMMUNICATION_TWO_WIRE_SLAVE_H_

#include "Arduino.h"

#include <Wire.h>

#include "./common.h"

/**
 * @brief Utility for boards to communicate over I2C
 */
class I2CSlave
{
private:
    // TwoWire instance to use for communication
    TwoWire *twi;

    // Address of the TwoWire slave device
    uint8_t address;

    // Request register for the device to respond to requests
    uint8_t request_register;

public:
    /**
     * @brief Construct a new I2CSlave object
     *
     * @param address Address of the TwoWire slave
     * @param twi TwoWire instance to use for communication
     */
    I2CSlave(uint8_t address, TwoWire *twi = &Wire);

    /**
     * @brief Begin the TwoWire interface and register the handlers
     *
     * @param onReceive Handler for the TwoWire on receive handler
     * @param onRequest Handler fro the TwoWire on request handler
     */
    void begin(void (*onReceive)(int), void (*onRequest)(void));

    /**
     * @brief Parse a message from the TwoWire master
     *
     * @return WireMessage Message parsed from the TwoWire master
     */
    WireMessage parseMessage();

    /**
     * @brief Write a value to the TwoWire master
     *
     * @tparam Type of the value being written to the TwoWire master
     * @param value Value to write to the TwoWire master
     */
    template <typename T>
    void write(T value);

    /**
     * @brief Get the current request register
     *
     * @return uint8_t The request registers value
     */
    uint8_t getRequestRegister();

    /**
     * @brief Set the current request register
     *
     * @param request_register Request register value
     */
    void setRequestRegister(uint8_t request_register);

    /**
     * @brief Get the number of available bytes
     *
     * @return uint8_t The number of bytes available to read in the TwoWire buffer
     */
    uint8_t getAvailable();
};

template <typename T>
void I2CSlave::write(T value)
{
    // Create a buffer and encode the value to write
    uint8_t buff[sizeof(T)];
    I2CCore::encodeValue<T>(value, buff);

    // Write buffer to the TwoWire interface
    this->twi->write(buff, sizeof(T));
}

#endif
