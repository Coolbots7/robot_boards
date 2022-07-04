#include "./Board.h"
#include "../communication/Communication.h"
#include "../communication/I2C.h"
#include "../communication/CanBus.h"
#include "../state/State.h"

uint8_t Board::staticBoardAddresses[MAX_STATIC_BOARD_COUNT];
uint8_t Board::staticBoardCount = 0;

Board::Board(uint8_t addr)
{
    this->state = STARTING;
}

// Communication
bool Board::beginI2C(uint8_t base_address, uint8_t address_offset)
{
    // create I2C controller
    this->communicationController = new I2CController(base_address, address_offset);
    return true;
}

bool Board::beginCanBus(uint16_t base_address, uint8_t address_offset)
{
    // create canbus controller
    this->communicationController = new CanBusController(base_address, address_offset);
    return true;
}

int8_t Board::getState()
{
    return this->state;
}

void Board::heartbeat(uint8_t state)
{
    // Check board is found
    // ping using selected communication method
    if (!this->communicationController->ping())
    {
        return BOARD_NOT_CONNECTED;
    }

    // send master status
    this->communicationController->writeRegisterByte(Registers::HEARTBEAT_STATUS, state);
    // send master time
    this->communicationController->writeRegisterULong(Registers::HEARTBEAT_TIME, millis());

    // get board status
    this->state = this->communicationController->readRegisterByte(Registers::STATE_STATUS);

    return this->state;
}

// Static Boards
// void Board::adoptStaticBoards(uint8_t base_address, uint8_t range)
// {
//     // search for status light boards in expected address range
//     for (uint8_t i = 0; i < range; i++)
//     {
//         uint8_t addr = base_address + i;
//         if (Board::pingBoard(addr))
//         {
//             // adopt board
//             uint8_t index = this->staticBoardCount;
//             this->staticBoardAddresses[index] = addr;

//             this->staticBoardCount = this->staticBoardCount++;
//             if (this->staticBoardCount >= MAX_STATIC_BOARD_COUNT)
//             {
//                 break;
//             }
//         }
//     }
// }

// void Board::heartbeatStaticBoards(uint8_t state)
// {
//     // search for status light boards in expected address range
//     for (uint8_t i = 0; i < this->staticBoardCount; i++)
//     {
//         uint8_t addr = this->staticBoardAddresses[i];
//         // heartbeat board
//         Board *staticBoard = new Board(addr);
//         staticBoard->heartbeat(state);
//     }
// }