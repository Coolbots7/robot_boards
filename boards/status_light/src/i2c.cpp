#include "common/state.h"
#include "state-machine.h"
#include "twowire/common.h"
#include "twowire/slave.h"

#include "./i2c.h"

void StatusLightI2CSlave::onReceive(int num_bytes)
{
    // Make sure at least one full message is available
    if (this->i2c_slave->getAvailable() >= sizeof(WireMessage))
    {
        // Read the message from the TwoWire buffer
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
        case Registers::REGISTER_REQUEST_TYPE:
            this->i2c_slave->setRequestRegister(I2CCore::decodeValue<uint8_t>(message.data));
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
