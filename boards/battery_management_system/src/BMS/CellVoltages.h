#ifndef CELL_VOLTAGES_H
#define CELL_VOLTAGES_H

#include <Adafruit_ADS1X15.h>

// C0 R1 15k R2 10k
// C1 R1 47k R2 12k
// C2 R1 68k R2 10k
// C3 R1 82k R2 10k

// #define ADC_GAIN GAIN_TWOTHIRDS
// Address of the ADC for cells 0-3
// #define ADC_0_ADDR 0x48
// #define ADC_1_ADDR 0x49
// Adafruit_ADS1115 ads0(ADC_0_ADDR);
// Adafruit_ADS1115 ads1(ADC_1_ADDR);

#define ADC_ADDRESS 0x48

class CellVoltages
{
private:
  Adafruit_ADS1115 *ads;
  uint8_t num_cells;

  const float r1_values[4] = {15000, 47000, 68000, 82000};
  const float r2_values[4] = {10000, 12000, 10000, 10000};
  // // Resistance values used for R1 in the voltage divider calculation
  // const float VOLTAGE_DIVIDER_R1[] = {
  //     0.0f,
  //     1.764f,
  //     1.468f,
  //     6.650f};

  // // Resistance values used for R@ in the voltage divider calculation
  // const float VOLTAGE_DIVIDER_R2[] = {
  //     1.0f,
  //     3.290f,
  //     0.985f,
  //     3.270f};

  static float vdSolveForInput(float r1, float r2, float out)
  {
    return (out * (r1 + r2)) / r2;
  }

  float calculateChannelVoltage(int16_t raw, uint8_t channel)
  {
    // convert raw to voltage
    float rawVoltage = this->ads->computeVolts(raw);

    // convert voltage to actual voltage w/ calibration
    return CellVoltages::vdSolveForInput(r1_values[channel], r2_values[channel], rawVoltage);
  }

public:
  CellVoltages(uint8_t num_cells)
  {
    this->num_cells = num_cells;
  }

  bool init(TwoWire *twoWire = &Wire)
  {
    // create ads object
    this->ads = new Adafruit_ADS1115();
    if (!ads->begin(ADC_ADDRESS, twoWire))
    {
      return false;
    }

    // default to 2x gain
    this->setGain(GAIN_TWO);

    // TODO load calibrations

    // // Detect the number of cells
    // Serial.println("Detecting cell count... ");
    // num_cells = 0;
    // for (int i = 3; i > 0; i--)
    // {
    //   float voltage = GetCellVoltage(i);
    //   if (voltage >= 2.0f)
    //   {
    //     num_cells = i + 1;
    //     break;
    //   }
    // }
    // Serial.print(num_cells);
    // Serial.println(" cells detected.");

    return true;
  }

  // TODO set channel voltage divider values

  void setGain(uint8_t gain)
  {
    this->ads->setGain((adsGain_t)gain);
  }

  // float GetBankVoltageAtIndex(uint8_t Index)
  // {
  //   uint16_t analogValue = 0;
  //   if (Index >= 0 && Index <= 3)
  //   {
  //     analogValue = ads0.readADC_SingleEnded(Index);
  //   }
  //   else if (Index > 3 && Index <= 5)
  //   {
  //     analogValue = ads1.readADC_SingleEnded(Index - 4);
  //   }
  //   else
  //   {
  //     return -1;
  //   }

  //   return analogValue * ((0.1875f / 1000.0f) * ((VOLTAGE_DIVIDER_R1[Index] + VOLTAGE_DIVIDER_R2[Index]) / VOLTAGE_DIVIDER_R2[Index]));
  // }

  float getCellVoltage(uint8_t cellIdx)
  {
    if (cellIdx > 3)
    {
      return -1.0f;
    }

    // get raw ads value for index
    float raw = this->ads->readADC_SingleEnded(cellIdx);

    // get actual voltage
    return this->calculateChannelVoltage(raw, cellIdx);

    // double CellVoltage = 0;
    // if (Index == 0)
    // {
    //   return GetBankVoltageAtIndex(Index);
    // }
    // else
    // {
    //   return (GetBankVoltageAtIndex(Index) - GetBankVoltageAtIndex(Index - 1));
    // }
    // return -1;
  }

  // TODO calibrate channel

  // TODO store channel calibrations in EEPROM

  // TODO retrieve channel calibrations from EEPROM
};

#endif
