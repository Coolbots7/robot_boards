#ifndef MOTOR_H
#define MOTOR_H

#include <math.h>
#include "Arduino.h"

#ifndef ADC_MAX_VOLTAGE
#define ADC_MAX_VOLTAGE 5
#endif

class Motor
{
  protected:
    bool enabled;
    uint8_t enablePin = -1;

    bool reversed;

    float speed;

    uint8_t currentSensePin;
    float currentSenseScalar;

    static float fmap(float in, float in_min, float in_max, float out_min, float out_max) {
      return (in - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

  public:
    // ====== Enable ======
    void setEnabled(bool enabled = true)
    {
      this->enabled = enabled;
      
      // TODO enable and PWM same pin
      //Check enabled pin defined
      if (this->enablePin >= 0)
      {
        digitalWrite(this->enablePin, this->enabled);
      }
    }

    void setDisabled()
    {
      setEnabled(false);
    }

    bool getEnabled() {
      return this->enabled;
    }

    void setReversed(bool reversed = true) {
      this->reversed = reversed;
    }

    bool getReversed() {
      return this->reversed;
    }

    virtual void setSpeed(float speed);

    float getSpeed() {
      return this->speed;
    }


    // ====== Current Sense ======
    void setCurrentSenseScalar(double scalar) {
      this->currentSenseScalar = scalar;
    }

    float getCurrent() {
      if (isnan(this->currentSensePin) || isnan(this->currentSenseScalar)) {
        return 0.0;
      }

      //Read analog pin
      uint8_t analogValue = analogRead(this->currentSensePin);

      //Map analog read value to voltage
      float senseVoltage = fmap(analogValue, 0, 1023, 0, (double)ADC_MAX_VOLTAGE);

      //Convert voltage to current
      float senseCurrent = senseVoltage / this->currentSenseScalar;

      return senseCurrent;
    }
};

#endif
