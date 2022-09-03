#include <Wire.h>

#include "./common.h"
#include "./master.h"

I2CMaster::I2CMaster(TwoWire *twi)
{
    this->twi = twi;
}

void I2CMaster::writeMessage(uint8_t address, WireMessage message)
{
    // Encode message into a buffer for transmission
    uint8_t buff[sizeof(WireMessage)];
    I2CCore::encodeValue<WireMessage>(message, buff);

    // Send the message to the TwoWire address
    this->twi->beginTransmission(address);
    this->twi->write(buff, sizeof(WireMessage));
    this->twi->endTransmission();
}

void I2CMaster::begin()
{
    // Initialize the TwoWire interface
    this->twi->begin();
}
