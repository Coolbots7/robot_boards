#ifndef DC_MOTOR_H
#define DC_MOTOR_H

#include <math.h>
#include "Arduino.h"
#include "Motor.h"

class DCMotor : public Motor
{
  private:
    uint8_t aPin;
    uint8_t bPin;
    uint8_t pwmPin = -1;

    bool coast = true;

  public:
    DCMotor(uint8_t aPin, uint8_t bPin, uint8_t enablePin) : DCMotor(aPin, bPin, enablePin, -1) {}

    DCMotor(uint8_t aPin, uint8_t bPin, uint8_t enablePin, uint8_t currentSensePin) : DCMotor(aPin, bPin, -1, enablePin, currentSensePin) {}

    DCMotor(uint8_t aPin, uint8_t bPin, uint8_t pwmPin, uint8_t enablePin, uint8_t currentSensePin)
    {
      // TODO move to Motor class
      this->enabled = false;
      this->setDisabled();

      this->speed = 0.0f;

      //assign pins
      this->aPin = aPin;
      this->bPin = bPin;
      this->pwmPin = pwmPin;

      this->currentSensePin = currentSensePin;

      //set pin modes
      pinMode(this->aPin, OUTPUT);
      pinMode(this->bPin, OUTPUT);

      //      if (this->pwmPin >= 0)
      //      {
      //        pinMode(this->pwmPin, OUTPUT);
      //      }

      if (enablePin >= 0 && enablePin != this->pwmPin)
      {
        this->enablePin = enablePin;
        pinMode(this->enablePin, OUTPUT);
        this->setDisabled();
      }
    }

    void setCoast(bool c = true) {
      this->coast = c;
    }

    void setBrake() {
      setCoast(false);
    }

    bool getCoast() {
      return this->coast;
    }

    void setSpeed(float speed)
    {
      // TODO reversed

      //Cap speed between -1.0 and 1.0
      if (speed > 1.0)
      {
        speed = 1.0;
      }
      else if (speed < -1.0)
      {
        speed = -1.0;
      }

      this->speed = speed;

      if (!this->enabled || speed == 0.0f) {
        // TODO inverted enable logic
        if (this->enablePin == this->pwmPin) {
          analogWrite(this->enablePin, 0);
        }
        else {
          digitalWrite(this->enablePin, LOW);
        }

        //Set motor driver to coast or brake
        // TODO inverted coast / brake
        if (this->coast)
        {
          digitalWrite(this->aPin, LOW);
          digitalWrite(this->bPin, LOW);
        }
        else
        {
          //Brake
          digitalWrite(this->aPin, HIGH);
          digitalWrite(this->bPin, HIGH);
        }

        return;
      }

      int PWMPin = 0;
      if (speed > 0.0f)
      {
        //set motor pins
        PWMPin = this->aPin;
        digitalWrite(this->bPin, LOW);
      }
      else if (speed < 0.0f)
      {
        //set motor pins
        digitalWrite(this->aPin, LOW);
        PWMPin = this->bPin;
      }

      //check if PWM pin is defined
      if (this->pwmPin >= 0)
      {
        //Use PWM pin instead of motor pin
        digitalWrite(PWMPin, HIGH);
        PWMPin = this->pwmPin;
      }

      //pulse PWM pin
      //TODO use high frequency PWM if possible
      int pwm = round(255.0f * abs(speed));
      analogWrite(this->pwmPin, pwm);
    }
};

#endif
