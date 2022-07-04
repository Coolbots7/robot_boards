#ifndef SOLENOID_H
#define SOLENOID_H

#include "Debug.h"

class Solenoid: public Debuggable {
  private:
    uint8_t pin;

    bool reversed;

    bool state;

  public:
    Solenoid(uint8_t pin, bool reversed = false) {
      this->pin = pin;

      this->reversed = reversed;

      this->state = reversed;
    }

    void open() {
      this->setState(true);
    }

    void close() {
      this->setState(false);
    }

    void setState(bool state) {
      //Flip state if solenoid set to reversed
      if (this->reversed) {
        state = !state;
      }

      this->state = state;
      digitalWrite(this->pin, this->state);
    }

    bool getState() {
      return this->state;
    }

    String debug() {
      return "Solenoid:" + String(this->getState());
    }
};

#endif
