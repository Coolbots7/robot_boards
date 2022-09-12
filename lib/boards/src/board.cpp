#include "./board.h"
#include "./common/state.h"

BoardInterface::BoardInterface(uint8_t address, I2CMaster *i2c_master)
{
    this->state = new BoardStateInterface(address, i2c_master);
}
