#ifndef ROBOT_BOARDS_STATE_H
#define ROBOT_BOARDS_STATE_H

#include "../communication/I2C.h"

enum SystemState
{
  NONE = -1,
  STARTING = 0x00,
  IDLE = 0x01,
  RUNNING = 0x02,
  HALT = 0x04,
  FAULT = 0xFF
};

/*
    State object to track state of the board, enforce transition rules, and make transitions using inputs.

    Inherit 'Debuggable' from debug library.
*/
class State
{
private:
  void (*stateLogic)(State *);

  SystemState previous_state;
  SystemState current_state;

public:
  State(void (*stateHandler)(State *));

  void update(I2CDevice *i2cDevice);

  /*
     Enforce state machine transition rules.
  */
  SystemState transitionTo(SystemState desired_state);

  SystemState getPreviousState();

  SystemState getCurrentState();

  static String getStateDescription(SystemState state);
};

#endif