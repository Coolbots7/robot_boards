#ifndef _ROBOT_BOARDS_COMMUNICATION_TWO_WIRE_MASTER_H_
#define _ROBOT_BOARDS_COMMUNICATION_TWO_WIRE_MASTER_H_

#include "Arduino.h"

#include <Wire.h>

#include "common/registers.h"

#include "./common.h"

/**
 * @brief Utility to communicate with boards over I2C
 */
class I2CMaster
{
private:
    // TwoWire instance to use for communication
    TwoWire *twi;

    /**
     * @brief Write a message to a board
     *
     * @param address Address of the board on the bus
     * @param message Message to send to the board
     */
    void writeMessage(uint8_t address, WireMessage message);

    /**
     * @brief Read a value from a board
     *
     * @param address Address of hte board on the bus
     * @return T Value returned by the board
     */
    template <typename T>
    T read(uint8_t address);

public:
    /**
     * @brief Construct a new I2CMaster object
     *
     * @param twi Optional TwoWire instance to use for communication
     */
    I2CMaster(TwoWire *twi = &Wire);

    /**
     * @brief Initialize the TwoWire master
     */
    void begin();

    /**
     * @brief Set the value of a register
     *
     * @tparam T Type of the value being written to the register
     * @param address Address of the TwoWire device
     * @param reg The register to set the value
     * @param value Value to set to the register
     */
    template <typename T>
    void setRegister(uint8_t address, uint8_t reg, T value);

    /**
     * @brief Get the value of a register
     *
     * @tparam T Type of the value expected from the register
     * @param address Address of the TwoWire device
     * @param reg The register to get the value from
     * @return T Value returned from the register
     */
    template <typename T>
    T getRegister(uint8_t address, uint8_t reg);
};

template <typename T>
T I2CMaster::read(uint8_t address)
{
    // Send request to the TwoWire address
    this->twi->requestFrom(address, sizeof(T));

    // Create a byte array and read the incoming data from the TwoWire interface
    uint8_t buff[sizeof(T)];
    uint8_t i = 0;
    while (this->twi->available() && i < sizeof(T))
    {
        buff[i++] = this->twi->read();
    }

    // Decode the byte array to type
    return I2CCore::decodeValue<T>(buff);
}

template <typename T>
void I2CMaster::setRegister(uint8_t address, uint8_t reg, T value)
{
    // Create a Wire Message
    WireMessage message;

    // Set message register
    message.message_type = reg;

    // Set message data
    I2CCore::encodeValue<T>(value, message.data);

    // Send the message over the TwoWire interface
    this->writeMessage(address, message);
}

template <typename T>
T I2CMaster::getRegister(uint8_t address, uint8_t reg)
{
    // Set the boards read register to the desired register
    this->setRegister<uint8_t>(address, Registers::REGISTER_REQUEST_TYPE, reg);

    // Request the value from the board
    return this->read<T>(address);
}

#endif
