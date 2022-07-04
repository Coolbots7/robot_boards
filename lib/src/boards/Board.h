#ifndef _ROBOT_BOARDS_BOARD_H_
#define _ROBOT_BOARDS_BOARD_H_

#include "../communication/Communication.h"
#include "../communication/I2C.h"
#include "../communication/CanBus.h"

#define BOARD_NOT_CONNECTED -1

#define MAX_STATIC_BOARD_COUNT 4

/**
 * @brief Base class to represent a Robot Board TM to interface with
 *
 */
class Board
{
private:
  CommunicationController communicationController;

  // The last known state of the board
  int8_t state;

  // Time the last message was received from the board
  uint32_t lastMessageReceived;

  // TODO auto discover and update "static" robot boards of the inheriting class' type
  // uint8_t staticBoardAddresses[4];
  // uint8_t staticBoardCount;

  /**
   * @brief Ping a specified address using the configured communication method
   * to detect if a device with that address is connected.
   *
   * @param address - Address to ping
   * @return true - A device with the address is connected
   * @return false - Failed to find a device with that address connected
   */
  static bool pingBoard(uint8_t address);

protected:
  Board();

  // Communication
  /**
   * @brief Begin I2C data layer to communicate with the board
   *
   * @param base_address - Base address for the board type
   * @param address_offset - Address offset configured on board
   */
  void beginI2C(uint8_t base_address, uint8_t address_offset);

  /**
   * @brief Begin CanBus data layer to communicate with the board
   *
   * @param base_address
   * @param address_offset
   */
  void beginCanBus(uint16_t base_address, uint8_t address_offset);

  // Static Boards
  // void adoptStaticBoards(uint8_t base_address, uint8_t address_space);
  // void heartbeatStaticBoards(uint8_t master_state);

  void subscribe(uint16_t reg, void *isr)
  {
    // TODO register that if a message from this board with the specified register is received, call isr with bytes
  }

  bool handleMessage(can_msg msg)
  {
    // TODO check id is from this board

    // TODO return register and value

    // TODO heartbeat
  }

public:
  /**
   * @brief Get the board state
   *
   * @return int8_t
   */
  int8_t getState();

  /**
   * @brief Send a heartbeat message to the board and get its state
   *
   * @param state - State of the controller
   * @return int8_t - State of the board
   */
  int8_t heartbeat(uint8_t state);
};

#endif