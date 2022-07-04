#ifndef LOOP_H
#define LOOP_H

#include "Debug.h"

#define SAFE HIGH
#define HOT LOW

#define OPEN LOW
#define CLOSED HIGH

class Loop: public Debuggable {
  private:
    bool state;

    uint8_t control_pin;
    uint8_t sense_pin;
    uint8_t indicator_pin;

    void loopInterrupt() {
      this->state = digitalRead(this->sense_pin);

      this->update();
    }

  public:
    Loop(uint8_t sense_pin) {
      //Initialize loop sense pin
      this->sense_pin = sense_pin;
      pinMode(sense_pin, INPUT);
      //TODO only if pin has interrupt
      //      attachInterrupt(digitalPinToInterrupt(sense_pin), loopInterrupt, CHANGE);
    }

    Loop(uint8_t control_pin, uint8_t sense_pin, uint8_t indicator_pin) {
      //Initialize loop control pin
      this->control_pin = control_pin;
      pinMode(control_pin, OUTPUT);
      digitalWrite(control_pin, SAFE);

      //Initialize loop sense pin
      this->sense_pin = sense_pin;
      pinMode(sense_pin, INPUT_PULLUP);
      //TODO only if pin has interrupt
      //      attachInterrupt(digitalPinToInterrupt(sense_pin), loopInterrupt, CHANGE);

      //Initialize loop indicator pin
      this->indicator_pin = indicator_pin;
      pinMode(indicator_pin, OUTPUT);
      digitalWrite(indicator_pin, LOW);
    }

    void energise() {
      digitalWrite(this->control_pin, HOT);
    }

    void safe() {
      digitalWrite(this->control_pin, SAFE);
    }

    bool getState() {
      return this->state;
    }

    bool getLoopEnergised() {
      return this->state == HIGH;
    }

    bool getLoopSafe() {
      return this->state == LOW;
    }

    void update() {
      //Update loop state
      this->state = digitalRead(this->sense_pin);

      if (this->state == OPEN) {
        //TODO indicator
      }
      else if (this->state == CLOSED) {
        //TODO indicator
      }
    }

    String debug() {
      String message = "Loop: ";
      
      switch (this->state) {
        case HIGH:
          message += "ENERGISED";
          break;
        case LOW:
          message += "SAFE";
          break;
      }
      return message;
    }
};

#endif
