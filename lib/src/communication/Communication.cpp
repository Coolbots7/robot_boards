#include "./Communication.h"

/**
 * CommunicationDevice
 */
CommunicationDevice::CommunicationDevice(uint16_t base_address, uint8_t address_offset)
{
    this->address = CommunicationBase::combineAddress(base_address, address_offset);

    this->request_register = 0;

    this->firstContact = false;
    this->masterLastHeartbeatTime = 0;
    this->masterTime = 0;
    this->masterState = -1;
}

static uint8_t CommunicationDevice::getAddressOffset(uint8_t b0Pin, uint8_t b1Pin, uint8_t b2Pin = 255, uint8_t b3Pin = 255)
{
    uint8_t addressOffset = 0;

    if (b3Pin < 255)
    {
        pinMode(b3Pin, INPUT_PULLUP);

        addressOffset = addressOffset | digitalRead(b3Pin);
        addressOffset << 1;
    }

    if (b2Pin < 255)
    {
        pinMode(b2Pin, INPUT_PULLUP);

        addressOffset = addressOffset | digitalRead(b2Pin);
        addressOffset << 1;
    }

    pinMode(b1Pin, INPUT_PULLUP);
    addressOffset = addressOffset | digitalRead(b1Pin);
    addressOffset << 1;

    pinMode(b0Pin, INPUT_PULLUP);
    addressOffset = addressOffset | digitalRead(b0Pin);
    return addressOffset;
}

uint8_t CommunicationDevice::getAddress()
{
    return this->address;
}

bool CommunicationDevice::getFirstContact()
{
    return this->firstContact;
}

uint8_t CommunicationDevice::getRegisterCursor()
{
    return this->request_register;
}

//
uint32_t CommunicationDevice::getMasterTime()
{
    return this->masterTime;
}

uint32_t CommunicationDevice::getMasterLastHeartbeatTime()
{
    return this->masterLastHeartbeatTime;
}

uint8_t CommunicationDevice::getMasterState()
{
    return this->masterState;
}

/**
 * CommunicationController
 */
CommunicationController::CommunicationController(uint16_t base_address, uint8_t address_offset)
{
    this->address = CommunicationBase::combineAddress(base_address, address_offset);
}

uint32_t CommunicationController::getAddress()
{
    return this->address;
}