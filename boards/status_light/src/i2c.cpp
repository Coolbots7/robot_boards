#include "common/state.h"
#include "state-machine.h"
#include "status_light/status_light.h"
#include "twowire/common.h"
#include "twowire/slave.h"

#include "./i2c.h"

StatusLightI2CSlave::StatusLightI2CSlave(uint8_t address_offset, StateMachine *state)
{
    this->state = state;

    // Create I2CSlave with address using offset
    this->i2c_slave = new I2CSlave(I2C_STATUS_LIGHT_BASE_ADDRESS + address_offset);

    this->master_state = State::NONE;
    this->master_time = 0UL;
    this->master_offset = 0UL;
}

void StatusLightI2CSlave::begin(void (*onReceive)(int), void (*onRequest)(void))
{
    // Initialize the TwoWire interface with handler functions
    this->i2c_slave->begin(onReceive, onRequest);
}

void StatusLightI2CSlave::onReceive(int num_bytes)
{
    // Make sure at least one full message is available
    if (this->i2c_slave->getAvailable() >= sizeof(WireMessage))
    {
        // Read the message from the buffer
        WireMessage message = this->i2c_slave->parseMessage();

        switch (message.message_type)
        {
        case StateRegisters::HEARTBEAT_STATE:
            this->master_state = I2CCore::decodeValue<State>(message.data);
            break;
        case StateRegisters::HEARTBEAT_TIME:
            this->master_time = I2CCore::decodeValue<uint32_t>(message.data);
            this->master_offset = this->master_time - millis();
            break;
        default:
            break;
        };
    }
}

void StatusLightI2CSlave::onRequest()
{
    switch (this->i2c_slave->getRequestRegister())
    {
    case StateRegisters::STATE:
        this->i2c_slave->write<State>(this->state->getCurrentState());
        break;
    };
}

State StatusLightI2CSlave::getMasterState()
{
    return this->master_state;
}

uint32_t StatusLightI2CSlave::getMasterTime()
{
    return this->master_time;
}

int32_t StatusLightI2CSlave::getMasterOffset()
{
    return this->master_offset;
}
