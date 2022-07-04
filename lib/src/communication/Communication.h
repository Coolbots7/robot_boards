#ifndef _ROBOT_BOARDS_COMMUNICATION_H_
#define _ROBOT_BOARDS_COMMUNICATION_H_

#include "Arduino.h"

// Maximum time between heartbeat messages until a device is considered disconnected
#define HEARTBEAT_TIMEOUT 1000

/**
 * @brief
 *
 */
class CommunicationBase
{
public:
    static uint16_t getAddress(uint16_t base_address, uint8_t address_offset)
    {
        return base_address + address_offset;
    }

    // Byte
    static uint8_t decodeByte(uint8_t *buff, uint8_t size);
    static void encodeByte(uint8_t val, uint8_t *buff, uint8_t size);

    // Bool
    static bool decodeBool(uint8_t *buff, uint8_t size);
    static void encodeBool(bool value, uint8_t *buff, uint8_t size);

    // Float
    static float decodeFloat(uint8_t *buff);
    static void encodeFloat(float val, uint8_t *buff);

    // ULong
    static uint32_t decodeULong(uint8_t *buff);
    static void encodeULong(uint32_t val, uint8_t *buff);
}

/**
 * @brief Base class for a board to handle communications
 *
 */
class CommunicationDevice
{
private:
    // Address of the board receiving communications
    uint32_t address;

    // The current register value of the
    uint8_t request_register;

    //
    bool firstContact;
    uint32_t masterLastHeartbeatTime;
    uint32_t masterTime;
    uint8_t masterState;

protected:
    CommunicationDevice(uint16_t base_address, uint8_t address_offset);

public:
    static uint8_t getAddressOffset(uint8_t b0Pin, uint8_t b1Pin, uint8_t b2Pin = 255, uint8_t b3Pin = 255);

    uint32_t getAddress();

    bool getFirstContact();
    uint8_t getRegisterCursor();

    uint32_t getMasterTime();
    uint32_t getMasterLastHeartbeatTime();
    uint8_t getMasterState();

    virtual void writeByte(uint8_t data);
    virtual void writeBool(bool data);
    virtual void writeFloat(float data);
    virtual void writeULong(uint32_t data);
};

/**
 * @brief Base class for communicating with board
 *
 */
class CommunicationController
{
private:
    // Address of the board being communicated with
    uint32_t address;

protected:
    CommunicationController(uint16_t base_address, uint8_t address_offset);

public:
    uint16_t getAddress();

    virtual bool ping();

    // Byte
    virtual void writeRegisterByte(uint8_t reg, uint8_t data);
    virtual uint8_t readRegisterByte(uint8_t reg);

    // Bool
    virtual void writeRegisterBool(uint8_t reg, bool data);
    virtual bool readRegisterBool(uint8_t reg);

    // Float
    virtual void writeRegisterFloat(uint8_t reg, float data);
    virtual float readRegisterFloat(uint8_t reg);

    // Unsigned long
    virtual void writeRegisterULong(uint8_t reg, uint32_t data);
    virtual uint32_t readRegisterULong(uint8_t reg);
};

#endif