#include <Wire.h>

#include "./common.h"
#include "./slave.h"

I2CSlave::I2CSlave(uint8_t address, TwoWire *twi)
{
    this->address = address;
    this->twi = twi;

    this->request_register = 0;
}

void I2CSlave::begin(void (*onReceive)(int), void (*onRequest)(void))
{
    // Initialize the TwoWire interface in slave mode with address
    this->twi->begin(this->address);

    // Register the on receive handler for the TwoWire instance
    this->twi->onReceive(onReceive);

    // Register the on request handler for the TwoWire instance
    this->twi->onRequest(onRequest);
}

WireMessage I2CSlave::parseMessage()
{
    // Create a byte array and read the incoming message from the TwoWire interface
    uint8_t buff[sizeof(WireMessage)];
    uint8_t i = 0;
    while (this->twi->available() && i < sizeof(WireMessage))
    {
        buff[i++] = this->twi->read();
    }

    // Decode the byte array into a message
    return I2CCore::decodeValue<WireMessage>(buff);
}

uint8_t I2CSlave::getRequestRegister()
{
    return this->request_register;
}

uint8_t I2CSlave::getAvailable()
{
    return this->twi->available();
}
