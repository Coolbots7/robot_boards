#ifndef SWITCH_H
#define SWITCH_H

#include "Debug.h"

class Switch: public Debuggable {
  private:
    uint8_t pin;

  public:
    Switch(uint8_t pin, uint8_t mode = INPUT) {
      this->pin = pin;

      pinMode(this->pin, mode);
    }

    bool getState() {
      return digitalRead(this->pin);
    }

    String debug() {
      return "Switch: " + String(this->getState());
    }

};

#endif
