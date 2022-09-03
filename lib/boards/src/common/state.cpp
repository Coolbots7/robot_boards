#include "state-machine.h"

#include "../board.h"
#include "./state.h"

BoardStateInterface::BoardStateInterface(uint8_t address, I2CMaster *i2c_master)
{
    this->address = address;
    this->i2c_master = i2c_master;
}

State BoardStateInterface::getBoardState()
{
    return this->i2c_master->getRegister<State>(this->address, StateRegisters::STATE);
}

void BoardStateInterface::setMasterState(State state)
{
    this->i2c_master->setRegister<State>(this->address, StateRegisters::HEARTBEAT_STATE, state);
}

void BoardStateInterface::setMasterTime(uint32_t time)
{
    this->i2c_master->setRegister<uint32_t>(this->address, StateRegisters::HEARTBEAT_TIME, time);
}
