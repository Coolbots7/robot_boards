#ifndef VALVE_H
#define VALVE_H

#include "Debug.h"
#include "Solenoid.h"

enum ValveState {
  OPEN = 0x01,
  CENTER = 0x02,
  CLOSE = 0x03
};

enum ValveType {
  TWO_POSITION_ONE_SOLENOID = 0x01,
  TWO_POSITION_TWO_SOLENOID = 0x02,
  THREE_POSITION_TWO_SOLENOID = 0x03
};

//TODO setting for log or plotter. propogates to object debug method.

class Valve: public Debuggable {
  private:
    ValveType type;

    Solenoid *solenoid1;
    Solenoid *solenoid2;

    ValveState state;

  public:
    Valve(ValveType type, uint8_t solenoid_1_pin) {
      //TODO check type

      this->type = type;

      solenoid1 = new Solenoid(solenoid_1_pin);
//      solenoid2 = null;

      if (this->type == THREE_POSITION_TWO_SOLENOID) {
        this->state = CENTER;
      }
      else {
        this->state = OPEN;
      }
    }

    Valve(ValveType type, uint8_t solenoid_1_pin, uint8_t solenoid_2_pin) {
      //TODO check type

      this->type = type;

      solenoid1 = new Solenoid(solenoid_1_pin);
      solenoid2 = new Solenoid(solenoid_2_pin);

      if (this->type == THREE_POSITION_TWO_SOLENOID) {
        this->state = CENTER;
      }
      else {
        this->state = OPEN;
      }
    }

    //TODO reverse solenoids

    void setState(ValveState state) {
      this->state = state;

      if (this->type == TWO_POSITION_ONE_SOLENOID) {
        set1SolenoidState(this->state);
      }
      else {
        set2SolenoidState(this->state);
      }
    }

    ValveState getState() {
      return this->state;
    }

    void set1SolenoidState(ValveState state) {
      // Single solenoid valves dont have a center state
      if (state == CENTER) {
        return;
      }

      if (state == OPEN) {
        this->solenoid1->setState(HIGH);
      }
      else if (state == CLOSE) {
        this->solenoid1->setState(LOW);
      }
    }

    void set2SolenoidState(ValveState state) {
      if (state == OPEN) {
        this->solenoid2->setState(LOW);
        this->solenoid1->setState(HIGH);
      }
      //Only 3 position valves have a center state
      else if (state == CENTER && this->type == THREE_POSITION_TWO_SOLENOID) {
        this->solenoid2->setState(LOW);
        this->solenoid1->setState(LOW);
      }
      else if (state == CLOSE) {
        this->solenoid1->setState(LOW);
        this->solenoid2->setState(HIGH);
      }
    }

    String debug() {
      String state_string = "";
      switch (this->getState()) {
        case OPEN:
          state_string = "Open";
          break;
        case CENTER:
          state_string = "Center";
          break;
        case CLOSE:
          state_string = "Close";
          break;
      }

      return "Valve: " + state_string;
    }

};

#endif
