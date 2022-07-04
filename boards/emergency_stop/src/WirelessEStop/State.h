#ifndef STATE_H
#define STATE_H

#include "Debug.h"

enum SystemState {
  STARTING = 0x00,
  IDLE = 0x01,
  RUNNING = 0x02,
  HALT = 0x04,
  FAULT = 0xFF
};

//TODO state singleton

class State: public Debuggable {
  private:
    SystemState current_state;

  public:
    State() {
      this->current_state = STARTING;
    }

    /*
       Enforce state machine transition logic.
    */
    SystemState transitionTo(SystemState desired_state) {
      SystemState next_state = -1;

      if (this->current_state == STARTING) {
        switch (desired_state) {
          case IDLE:
          case FAULT:
            next_state = desired_state;
            break;
          default:
            break;
        }
      }
      else if (this->current_state == IDLE) {
        switch (desired_state) {
          case RUNNING:
          case HALT:
          case FAULT:
            next_state = desired_state;
            break;
          default:
            break;
        }
      }
      else if (this->current_state == RUNNING) {
        switch (desired_state) {
          case IDLE:
          case HALT:
          case FAULT:
            next_state = desired_state;
            break;
          default:
            break;
        }
      }
      else if (this->current_state == HALT) {
        switch (desired_state) {
          case IDLE:
          case RUNNING:
          case FAULT:
            next_state = desired_state;
            break;
          default:
            break;
        }
      }
      else if (this->current_state == FAULT) {
        switch (desired_state) {
          case STARTING:
            next_state = desired_state;
            break;
          default:
            break;
        }
      }

      if (next_state > 0) {
        this->current_state = next_state;
      }

      return this->current_state;
    }

    uint8_t getState() {
      return (uint8_t)this->current_state;
    }

    String debug() {
      return "State: " + String(this->getState());
    }
};

#endif
