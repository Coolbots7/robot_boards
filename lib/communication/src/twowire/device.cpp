#include "state-machine.h"
#include "twowire/slave.h"

#include "./device.h"

I2CDevice::I2CDevice(uint8_t address, StateMachine *state)
{
    this->state = state;

    // Create I2CSlave with address
    this->i2c_slave = new I2CSlave(address);

    this->master_state = State::NONE;
    this->master_time = 0UL;
    this->master_offset = 0UL;
}

void I2CDevice::begin(void (*onReceive)(int), void (*onRequest)(void))
{
    // Initialize the TwoWire interface with handler functions
    this->i2c_slave->begin(onReceive, onRequest);
}

State I2CDevice::getMasterState()
{
    return this->master_state;
}

uint32_t I2CDevice::getMasterTime()
{
    return this->master_time;
}

int32_t I2CDevice::getMasterOffset()
{
    return this->master_offset;
}
