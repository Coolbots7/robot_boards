#ifndef _ROBOT_BOARDS_I2C_H_
#define _ROBOT_BOARDS_I2C_H_

#include "Arduino.h"
#include <Wire.h>

// Addresses
// Motor Controller 0x40 - 0x4F
#define I2C_MOTOR_CONTROLLER_BASE_ADDRESS 0x40

// Status Light 0x50 - 0x53
#define I2C_STATUS_LIGHT_BASE_ADDRESS 0x50
#define I2C_STATUS_LIGHT_ADDRESS_RANGE 4

// BMS 0x54 - 0x57
#define I2C_BMS_BASE_ADDRESS 0x54

// E Stop 0x58 - 0x5B
#define I2C_E_STOP_BASE_ADDRESS 0x58

/**
 * @brief
 *
 */
enum Registers
{
  HEARTBEAT_STATUS = 0xFC,
  HEARTBEAT_TIME = 0xFD,
  STATE_STATUS = 0xFE,
  REGISTER_REQUEST_TYPE = 0xFF,
};

/**
 * @brief
 *
 */
struct WireMessage
{
  uint8_t message_type;
  // MessageType message_type;
  uint8_t data[4];
};

/**
 * @brief
 *
 */
class I2CPrim
{
public:
  // Message
  static WireMessage decodeMessage(uint8_t *buff);
  static void encodeMessage(WireMessage message, uint8_t *buff);
};

/**
 * Used for I2C communiation to a board
 */

/**
 * @brief Wrapper for boards to communicate over I2C
 *
 * - Receive messages to set registers
 * - Respond to messages for registers
 *
 */
class I2CDevice : public CommunicationDevice
{
public:
  I2CDevice(uint8_t addr);

  /**
   * @brief
   *
   * @param byte_count
   * @return WireMessage
   */
  WireMessage receiveEvent(int byte_count);
};

/**
 * @brief Wrapper to communicate with boards over I2C
 *
 * - Set registers on a board
 * - Read registers on a board
 *
 */
class I2CController : public CommunicationController
{
private:
  /**
   * @brief Helper to write messages to the board
   *
   * @param message - Message to send to the board
   */
  void writeMessage(WireMessage message);

  // Helpers to write messages of a specific type to the board
  void writeByteMessage(uint8_t reg, uint8_t val);
  void writeFloatMessage(uint8_t reg, float val);
  void writeULongMessage(uint8_t reg, uint32_t val);

  /**
   * @brief Set the register on the board
   *
   * @param reg - The value to set the register on the board
   */
  void writeRegister(uint8_t reg);

public:
  I2CController(uint16_t base_address, uint8_t address_offset) : base(base_address, address_offset){};
};

#endif