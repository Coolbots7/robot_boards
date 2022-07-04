#ifndef PRESSURETRANSDUCER_H
#define PRESSURETRANSDUCER_H

#include "Debug.h"

#define BAR_PER_PSI 0.0689476
#define MPA_PER_PSI 0.00689476

class PressureTransducer: public Debuggable {
  private:
    uint8_t pin;

    double bus_voltage;

    double voltage_min;
    double voltage_max;
    double voltage_min_offset;

    double pressure_min;
    double pressure_max;

    double map_f(double value, double from_min, double from_max, double to_min, double to_max) {
      return (value  - from_min) * (to_max - to_min) / (from_max - from_min) + to_min;
    }

  public:
    PressureTransducer(uint8_t pin, double bus_voltage, double voltage_min, double voltage_max, double pressure_min, double pressure_max, double voltage_min_offset = 0.0f) {
      this->pin = pin;

      this->bus_voltage = bus_voltage;

      this->voltage_min = voltage_min;
      this->voltage_max = voltage_max;

      this->pressure_min = pressure_min;
      this->pressure_max = pressure_max;

      this->voltage_min_offset = voltage_min_offset;
    }

    double getRaw() {
      return analogRead(this->pin);
    }

    double getVoltage() {
      return map_f(this->getRaw(), 0.0f, 1024.0f, 0.0f, this->bus_voltage) + this->voltage_min_offset;
    }

    double getPressurePSI() {
      return map_f(this->getVoltage(), this->voltage_min, this->voltage_max, this->pressure_min, this->pressure_max);
    }

    double getPressureBar() {
      return getPressurePSI() * BAR_PER_PSI;
    }

    double getPressureMPA() {
      return getPressurePSI() * MPA_PER_PSI;
    }

    String debug() {
      return "PSI:" + String(this->getPressurePSI());
    }
};

#endif
