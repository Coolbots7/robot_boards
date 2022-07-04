#ifndef ROBOT_BOARDS_MOTOR_CONTROLLER_BOARD_H
#define ROBOT_BOARDS_MOTOR_CONTROLLER_BOARD_H

#include "./Board.h"

/**
 *  This library is used to talk to a motor controller board.
 */
class MotorControllerBoard : public Board
{
public:
  // Define constants used to select the value to be sent on next I2C request event
  enum Registers
  {
    // MOTOR 1 0x00 - 0x0F
    MOTOR_1_ENABLED = 0x00,
    MOTOR_1_SPEED = 0x01,
    MOTOR_1_REVERSED = 0x02,
    MOTOR_1_COAST = 0x03,

    // MOTOR 2 0x10 - 0x1F
    MOTOR_2_ENABLED = 0x10,
    MOTOR_2_SPEED = 0x11,
    MOTOR_2_REVERSED = 0x12,
    MOTOR_2_COAST = 0x13,
  };

  MotorControllerBoard(uint8_t addr) : Board(addr) {}

  // Enabled
  void enable1(bool enabled = true)
  {
    this->writeBool(MotorControllerBoard::MOTOR_1_ENABLED, enabled);
  }

  void disable1()
  {
    this->enable1(false);
  }

  bool getEnabled1()
  {
    return this->readBool(MotorControllerBoard::MOTOR_1_ENABLED);
  }

  void enable2(bool enabled = true)
  {
    this->writeBool(MotorControllerBoard::MOTOR_2_ENABLED, enabled);
  }

  void disable2()
  {
    this->enable2(false);
  }

  bool getEnabled2()
  {
    return this->readBool(MotorControllerBoard::MOTOR_2_ENABLED);
  }

  // Reversed
  void reversed1(bool reversed = true)
  {
    this->writeBool(MotorControllerBoard::MOTOR_1_REVERSED, reversed);
  }

  bool isReversed1()
  {
    return this->readBool(MotorControllerBoard::MOTOR_1_REVERSED);
  }

  void reversed2(bool reversed = true)
  {
    this->writeBool(MotorControllerBoard::MOTOR_2_REVERSED, reversed);
  }

  bool isReversed2()
  {
    return this->readBool(MotorControllerBoard::MOTOR_2_REVERSED);
  }

  // Coast
  void coast1(bool coast = true)
  {
    this->writeBool(MotorControllerBoard::MOTOR_1_COAST, coast);
  }

  void brake1()
  {
    this->coast1(false);
  }

  bool getCoast1()
  {
    return this->readBool(MotorControllerBoard::MOTOR_2_COAST);
  }

  void coast2(bool coast = true)
  {
    this->writeBool(MotorControllerBoard::MOTOR_2_COAST, coast);
  }

  void brake2()
  {
    this->coast2(false);
  }

  bool getCoast2()
  {
    return this->readBool(MotorControllerBoard::MOTOR_1_COAST);
  }

  // Speed
  void setSpeed1(float speed)
  {
    this->writeFloat(MotorControllerBoard::MOTOR_1_SPEED, speed);
  }

  float getSpeed1()
  {
    return this->readFloat(MotorControllerBoard::MOTOR_1_SPEED);
  }

  void setSpeed2(float speed)
  {
    this->writeFloat(MotorControllerBoard::MOTOR_2_SPEED, speed);
  }

  float getSpeed2()
  {
    return this->readFloat(MotorControllerBoard::MOTOR_2_SPEED);
  }
};

#endif