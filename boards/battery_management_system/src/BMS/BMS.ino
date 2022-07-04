#include <Wire.h>
#include <SoftwareWire.h>
#include <Adafruit_SSD1306.h>
#include <DallasTemperature.h>

#include <RB_Debug.h>
#include <Display.h>
#include <State.h>
#include <Registers.h>
#include <BMSBoard.h>
#include <I2C.h>
#include <StateLED.h>

#include "CellVoltages.h"

// ====== LOOP RATE ======
#define RATE 10 // Hz

// ====== DEBUG ======
#define DEBUG true
#define DEBUG_BAUD_RATE 115200

// ====== STATE ======
#define STATE_LED_PIN 30

// ====== SOFTWARE I2C ======
// Define pins used for Software I2C master
#define SOFTWARE_WIRE_SDA 23
#define SOFTWARE_WIRE_SCL 22

// ====== Screen ======
#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// ====== I2C Definitions ======
// Define address select pins
#define ADDRESS_BIT_0_PIN 16
#define ADDRESS_BIT_1_PIN 17
// // Define address options
// #define WIRE_ADDRESS_0 0x50
// #define WIRE_ADDRESS_1 0x51
// #define WIRE_ADDRESS_2 0x52
// #define WIRE_ADDRESS_3 0x53

// ====== VOLTAGE DIVIDERS ======
// TODO move num cells to EEPROM
#define NUM_CELLS 4
// C0 R1 15k R2 10k
// C1 R1 47k R2 12k
// C2 R1 68k R2 10k
// C3 R1 82k R2 10k

// #define logicVoltageRegulatorPin A6
// #define logicVoltageRegulatorResistor1 2
// #define logicVoltageRegulatorResistor2 1.127

// ====== GLOBALS ======
Debug *debug;
State *state;
StateLED *stateLED;
Adafruit_SSD1306 *screen;
Display *display;
I2CDevice *i2cDevice;
SoftwareWire softwareWire(SOFTWARE_WIRE_SDA, SOFTWARE_WIRE_SCL);

Adafruit_INA219 ina219(CURRENT_SENSOR_ADDRESS);

// TODO make error code enum

float cellChargedVoltage;
float cellNominalVoltage;
float cellCriticalVoltage;

// // Constant values to represent the current status of the battery
// enum BatteryStatus
// {
//   OK = 0x00,
//   WARNING = 0x01,
//   CRITICAL = 0x02
// };

// // Constant bit flags used to represent battery errors
// enum BatteryError
// {
//   CELL_OVERVOLTAGE_FLAG = 0x01,
//   CELL_LOW_FLAG = 0x02,
//   CELL_CRITICAL_FLAG = 0x04,
//   TEMPERATURE_OVERHEAT_WARNING_FLAG = 0x08,
//   TEMPERATURE_OVERHEAT_CRITICAL_FLAG = 0x10,
//   TEMPERATURE_UNDERHEAT_WARNING_FLAG = 0x20,
//   TEMPERATURE_UNDERHEAT_CRITICAL_FLAG = 0x40,
//   CURRENT_WARNING_FLAG = 0x80,
//   CURRENT_CRITICAL_FLAG = 0x100,
//   BATTERY_NOT_PRESENT = 0x200
// };

// // Pin to flash EEPROM to defaults when held high on boot
// #define EEPROM_FLASH_PIN 3

struct Battery
{
  float cellVoltages[NUM_CELLS];
  float voltage;
  float current;
  float power;
  float temperature;
  byte status;
};
// struct Battery
// {
//   float cell_voltages[4];
//   float voltage;
// float logicVoltage;
//   float current;
//   float power;
//   float temperature;
//   uint16_t errors;
//   uint8_t status;
// };

CellVoltages *cellVoltages;

// #define TEMP_ONE_WIRE_PIN 2
// // TODO move temp resolution to EEPROM
// #define TEMP_RESOLUTION 12
// // TOOD add over temp threshold
// // TODO add under temp threshold

// // TODO add over current threshold
// // Create one wire communication object and temperature sensor object
// OneWire oneWire(TEMP_ONE_WIRE_PIN);
// DallasTemperature tempSensor(&oneWire);

// // Global configuration values initialized from EEPROM on boot
// uint8_t refresh_rate;
// float cell_charged_voltage;
// float cell_nominal_voltage;
// float cell_critical_voltage;
// uint8_t temperature_resolution;
// float temperature_overheat_warning;
// float temperature_overheat_critical;
// float temperature_underheat_warning;
// float temperature_underheat_critical;
// float current_warning;
// float current_critical;
// uint8_t led_brightness;

// ====== Handler Functions ======
void wireBegin()
{
  softwareWire.begin();
}

void wireBeginTransmission(uint8_t addr)
{
  softwareWire.beginTransmission(addr);
}

void wireEndTransmission()
{
  softwareWire.endTransmission();
}

void wireWrite(uint8_t c)
{
  softwareWire.write(c);
}

class TW : public TwoWire
{
  void begin()
  {
    softwareWire.begin();
  }

  void beginTransmission(uint8_t addr)
  {
    softwareWire.beginTransmission(addr);
  }

  uint8_t endTransmission()
  {
    return softwareWire.endTransmission();
  }

  size_t write(uint8_t c)
  {
    return softwareWire.write(c);
  }
};
TW tw;

void displayHandler(Display *d, Adafruit_SSD1306 *s)
{
  s->clearDisplay();

  uint8_t y = 0;
  // state
  d->leftJustify(y, 1, "State: " + State::getStateDescription(state->getCurrentState()).substring(0, 4));
  // heartbeat time
  d->rightJustify(y, 1, "HB: " + String(i2cDevice->getMasterTime() % 1000));

  //  y += 15;
  //  d->leftJustify(y, 1, "C0: " + String(cellVoltages->getCellVoltage(0)) + "V");
  //  y += 12;
  //  d->leftJustify(y, 1, "C1: " + String(cellVoltages.getCellVoltage(1)) + "V");
  //  y += 12;
  //  d->leftJustify(y, 1, "C2: " + String(cellVoltages.getCellVoltage(2)) + "V");
  //  y += 12;
  //  d->leftJustify(y, 1, "C3: " + String(cellVoltages.getCellVoltage(3)) + "V");

  s->display();
}

void stateHandler(State *s)
{
  // Otherwise, transition to RUNNING
  s->transitionTo(RUNNING);
}

void onReceiveHandler(int bytes)
{
  WireMessage message = i2cDevice->receiveEvent(bytes);

  switch (message.message_type)
  {
  default:
    // TODO debug
    break;
  }
}

void onRequestHandler()
{
  switch (i2cDevice->getRegister())
  {
    // TODO scaleable cell voltage getter
  case BMSBoard::CELL_0_VOLTAGE:
    //      i2cDevice->writeFloat(cellVoltages.getCellVoltage(0));
    break;
  case BMSBoard::CELL_1_VOLTAGE:
    //      i2cDevice->writeFloat(cellVoltages.getCellVoltage(1));
    break;
  case BMSBoard::CELL_2_VOLTAGE:
    //      i2cDevice->writeFloat(cellVoltages.getCellVoltage(2));
    break;
  case BMSBoard::CELL_3_VOLTAGE:
    //      i2cDevice->writeFloat(cellVoltages.getCellVoltage(3));
    break;

  case BMSBoard::BATTERY_VOLTAGE:
    // TODO load voltage
    // float battery_voltage = battery.voltage;
    // sendData(&battery_voltage, sizeof(battery_voltage));
    break;
  case BMSBoard::BATTERY_CURRENT:
    // TODO load current
    // float battery_current = battery.current;
    // sendData(&battery_current, sizeof(battery_current));
    break;
  case BMSBoard::BATTERY_POWER:
    // TODO load power
    // float battery_power = battery.power;
    // sendData(&battery_power, sizeof(battery_power));
    break;
    // TODO
    // float battery_temperature = battery.temperature;
    // sendData(&battery_temperature, sizeof(battery_temperature));
    // TODO
    // uint16_t battery_errors = battery.errors;
    // sendData(&battery_errors, sizeof(battery_errors));
  case Registers::STATE_STATUS:
    //      i2cDevice->writeByte(state->getCurrentState());
    // uint8_t battery_status = battery.status;
    // sendData(&battery_status, sizeof(battery_status));
    break;

  default:
    // TODO debug
    break;
  }

  // else if (request_type == CELL_CHARGED_VOLTAGE)
  // {
  //   sendData(&cell_charged_voltage, sizeof(cell_charged_voltage));
  // }
  // else if (request_type == CELL_NOMINAL_VOLTAGE)
  // {
  //   sendData(&cell_nominal_voltage, sizeof(cell_nominal_voltage));
  // }
  // else if (request_type == CELL_CRITICAL_VOLTAGE)
  // {
  //   sendData(&cell_critical_voltage, sizeof(cell_critical_voltage));
  // }
  // else if (request_type == TEMPERATURE_RESOLUTION)
  // {
  //   sendData(&temperature_resolution, sizeof(temperature_resolution));
  // }
  // else if (request_type == TEMPERATURE_OVERHEAT_WARNING)
  // {
  //   sendData(&temperature_overheat_warning, sizeof(temperature_overheat_warning));
  // }
  // else if (request_type == TEMPERATURE_OVERHEAT_CRITICAL)
  // {
  //   sendData(&temperature_overheat_critical, sizeof(temperature_overheat_critical));
  // }
  // else if (request_type == TEMPERATURE_UNDERHEAT_WARNING)
  // {
  //   sendData(&temperature_underheat_warning, sizeof(temperature_underheat_warning));
  // }
  // else if (request_type == TEMPERATURE_UNDERHEAT_CRITICAL)
  // {
  //   sendData(&temperature_underheat_critical, sizeof(temperature_underheat_critical));
  // }
  // else if (request_type == CURRENT_WARNING)
  // {
  //   sendData(&current_warning, sizeof(current_warning));
  // }
  // else if (request_type == CURRENT_CRITICAL)
  // {
  //   sendData(&current_critical, sizeof(current_critical));
  // }
  // else if (request_type == CELL_COUNT)
  // {
  //   sendData(&num_cells, sizeof(num_cells));
  // }
  // else if (request_type == LED_BRIGHTNESS)
  // {
  //   sendData(&led_brightness, sizeof(led_brightness));
  // }
  // else if (request_type == REFRESH_RATE)
  // {
  //   sendData(&refresh_rate, sizeof(refresh_rate));
  // }
}

/**

*/
void setup()
{
  Serial.begin(DEBUG_BAUD_RATE);
  debug = new Debug("BMS", DEBUG);

  // // Set EEPROM flash pin as input
  // pinMode(EEPROM_FLASH_PIN, INPUT);

  // // If EEPROM flash pin is high on boot
  // if (digitalRead(EEPROM_FLASH_PIN) == HIGH)
  // {
  //   // Wait to confirm EEPROM flash
  //   for (int i = 0; i < 5; i++)
  //   {
  //     status_LED.fill(status_LED.Color(70, 0, 70));
  //     status_LED.show();
  //     delay(500);
  //     status_LED.fill(status_LED.Color(0, 0, 0));
  //     status_LED.show();
  //     delay(500);
  //   }

  //   // Check if pin is still high
  //   if (digitalRead(EEPROM_FLASH_PIN) == HIGH)
  //   {
  //     // Flash EEPROM to defaults
  //     setCellChargedVoltageEEPROM(4.2f);
  //     setCellNominalVoltageEEPROM(3.7f);
  //     setCellCriticalVoltageEEPROM(3.0f);
  //     setTempResolutionEEPROM(12);
  //     setTempOverheatWarningEEPROM(50);
  //     setTempOverheatCriticalEEPROM(60);
  //     setTempUnderheatWarningEEPROM(10);
  //     setTempUnderheatCriticalEEPROM(0);
  //     setCurrentWarningEEPROM(25000);
  //     setCurrentCriticalEEPROM(32000);
  //     setLEDBrightnessEEPROM(70);
  //     setRefreshRateEEPROM(1);

  //     // Indicate EEPROM flash is complete
  //     for (int i = 0; i < 3; i++)
  //     {
  //       status_LED.fill(status_LED.Color(0, 70, 0));
  //       status_LED.show();
  //       delay(100);
  //       status_LED.fill(status_LED.Color(0, 0, 0));
  //       status_LED.show();
  //       delay(100);
  //     }
  //   }
  //   else
  //   {
  //     // Pin is low, EEPROM flash canceled
  //     // Indicate EEPROM flash is canceled
  //     for (int i = 0; i < 3; i++)
  //     {
  //       status_LED.fill(status_LED.Color(70, 0, 0));
  //       status_LED.show();
  //       delay(100);
  //       status_LED.fill(status_LED.Color(0, 0, 0));
  //       status_LED.show();
  //       delay(100);
  //     }
  //   }
  // }

  // // Load configuration from EERPOM on boot
  // refresh_rate = getRefreshRateEEPROM();
  // temperature_resolution = getTempResolutionEEPROM();
  // temperature_overheat_critical = getTempOverheatCriticalEEPROM();
  // temperature_underheat_critical = getTempUnderheatCriticalEEPROM();
  // temperature_overheat_warning = getTempOverheatWarningEEPROM();
  // temperature_underheat_warning = getTempUnderheatWarningEEPROM();
  // current_warning = getCurrentWarningEEPROM();
  // current_critical = getCurrentCriticalEEPROM();
  // cell_charged_voltage = getCellChargedVoltageEEPROM();
  // cell_nominal_voltage = getCellNominalVoltageEEPROM();
  // cell_critical_voltage = getCellCriticalVoltageEEPROM();
  // led_brightness = getLEDBrightnessEEPROM();

  // // Debug configurations loaded from EEPROM on boot
  // Serial.println("Settings loaded from EEPROM:");
  // Serial.print("Refresh Rate (Hz): ");
  // Serial.println(refresh_rate);
  // Serial.print("Cell Charged (V): ");
  // Serial.println(cell_charged_voltage);
  // Serial.print("Cell Nominal (V): ");
  // Serial.println(cell_nominal_voltage);
  // Serial.print("Cell Critical (V): ");
  // Serial.println(cell_critical_voltage);
  // Serial.print("Temp Resolution: ");
  // Serial.println(temperature_resolution);
  // Serial.print("Temp Overheat Warn (C): ");
  // Serial.println(temperature_overheat_warning);
  // Serial.print("Temp Overheat Crit (C): ");
  // Serial.println(temperature_overheat_critical);
  // Serial.print("Temp Underheat Warn (C): ");
  // Serial.println(temperature_underheat_warning);
  // Serial.print("Temp Underheat Crit (C): ");
  // Serial.println(temperature_underheat_critical);
  // Serial.print("Current Warn (mA): ");
  // Serial.println(current_warning);
  // Serial.print("Current Crit (mA): ");
  // Serial.println(current_critical);
  // Serial.print("LED Brightness: ");
  // Serial.println(led_brightness);
  // Serial.println();

  // Initialize state machine and LED
  debug->print("Initializing State Machine...");
  state = new State(stateHandler);
  debug->print("OK\n");

  debug->print("Initializing Debug LED...");
  stateLED = new StateLED(state);
  stateLED->init<STATE_LED_PIN>();
  debug->print("OK\n");

  // Initialize software wire bus for onboard peripherals
  debug->print("Initializing Peripheral Wire Bus...");
  softwareWire.begin();
  debug->print("OK\n");

  // Init screen
  debug->print("Initializing Screen...");
  screen = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &softwareWire);
  //  screen = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT);

  screen->onWireBegin(wireBegin);
  screen->onWireBeginTransmission(wireBeginTransmission);
  screen->onWireEndTransmission(wireEndTransmission);
  screen->onWireWrite(wireWrite);
  screen->setRotation(0);

  if (!screen->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    debug->print("FAILED!");
    state->transitionTo(FAULT);
    stateLED->update();
  }

  display = new Display("BMS", screen, SCREEN_WIDTH, SCREEN_HEIGHT, displayHandler);
  delay(500);
  display->initializing("Display...", 0.5f);
  delay(500);
  debug->print("OK\n");

  // ====== I2C Communication ======
  // Add offset to base address
  byte wire_address = BMS_BASE_ADDRESS + I2CDevice::getAddressOffset(ADDRESS_BIT_0_PIN, ADDRESS_BIT_1_PIN);
  debug->print("Initializing Wire Bus at address 0x" + String(wire_address, HEX) + "...");
  display->initializing("I2C (0x" + String(wire_address, HEX) + ")", 0.7f);

  // Join I2C bus as slave with address
  i2cDevice = new I2CDevice(wire_address);
  Wire.begin(i2cDevice->getAddress());
  Wire.onReceive(onReceiveHandler);
  Wire.onRequest(onRequestHandler);
  delay(500);
  debug->print("OK\n");

  // ====== Cell Voltage ======
  debug->print("Initializing Cells...");
  display->initializing("Cells...", 0.8f);
  cellVoltages = new CellVoltages(NUM_CELLS);
  if (!cellVoltages->init(&tw))
  {
    debug->print("FAILED!\n");
    //      state->transitionTo(FAULT);
    //      stateLED->update();
  }
  else
  {
    delay(500);
    debug->print("OK\n");
  }

  // After successful initializaton, set state to idle to indicate its ready
  state->transitionTo(IDLE);
  stateLED->update();
  debug->print("Startup Complete!\n\n");
  display->initializing("Complete", 1.0f);
  delay(500);
  display->clear();

  // Set screen rotation
  screen->setRotation(0);

  // tempSensor.begin();
  // tempSensor.setResolution(TEMP_RESOLUTION);
  // tempSensor.setWaitForConversion(false);
  // tempSensor.setCheckForConversion(true);
  // tempSensor.requestTemperatures();

  // cellChargedVoltage = getCellChargedVoltageEEPROM();
  // cellNominalVoltage = getCellNominalVoltageEEPROM();
  // cellCriticalVoltage = getCellCriticalVoltageEEPROM();
}

/**

*/
uint32_t lastLoopTime = 0;
void loop()
{
  // // Update temperature sensors
  // if (tempSensor.isConversionComplete())
  //   tempSensor.requestTemperatures();

  if (millis() >= lastLoopTime + (1000 * (1 / RATE)))
  {
    lastLoopTime = millis();

    // Update current battery values, errors, and status
    Battery battery = updateBattery();

    // // Update status RGB LED
    // if (battery.status == OK)
    // {
    //   status_LED.fill(status_LED.Color(0, led_brightness, 0));
    // }
    // else if (battery.status == WARNING)
    // {
    //   status_LED.fill(status_LED.Color(led_brightness, led_brightness, 0));
    // }
    // else if (battery.status == CRITICAL)
    // {
    //   status_LED.fill(status_LED.Color(led_brightness, 0, 0));
    // }
    // status_LED.show();

    // state->update(i2cDevice);
    // stateLED->update(i2cDevice->getMasterTime());

    // TODO debug output
    // for (int i = 0; i < NUM_CELLS; i++)
    // {
    //   Serial.print("Cell ");
    //   Serial.print(i);
    //   Serial.print(" Voltage: ");
    //   Serial.println(battery.cellVoltages[i], 4);
    // }

    // Serial.print("Battery Voltage: ");
    // Serial.println(battery.voltage, 4);
    // Serial.print("Current (mA): ");
    // Serial.println(battery.current, 4);
    // Serial.print("Power (mW): ");
    // Serial.println(battery.power, 4);
    // Serial.print("Temperature (C): ");
    // Serial.println(battery.temperature, 4);
    // Serial.print("Errors: ");
    // Serial.println(battery.errors, BIN);
    // Serial.print("Status: ");
    // Serial.println(battery.status);
    // Serial.println();

    // update display
    display->update();
  }
}

Battery updateBattery()
{
  Battery battery;

  for (int i = 0; i < NUM_CELLS; i++)
  {
    battery.cellVoltages[i] = GetCellVoltage(i);
  }

  // Get battery voltage, current, and power
  battery.voltage = GetBankVoltageAtIndex(NUM_CELLS - 1);
  // another option: use ina219 to calcuate load voltage
  // battery.voltage = ina219.getBusVoltage_V() + (ina219.getShuntVoltage_mV() / 1000);
  battery.current = ina219.getCurrent_mA();
  battery.power = ina219.getPower_mW();

  // TODO
  // battery.logicVoltage = Voltage(logicVoltageRegulatorPin, logicVoltageRegulatorResistor1, logicVoltageRegulatorResistor2);

  // Get battery temperature
  battery.temperature = tempSensor.getTempCByIndex(0);

  // TODO
  battery.status = 0;

  for (int i = 0; i < NUM_CELLS; i++)
  {
    if (battery.cellVoltages[i] > cellChargedVoltage)
    {
      battery.status = 1;
    }
  }

  for (int i = 0; i < NUM_CELLS; i++)
  {
    if (battery.cellVoltages[i] <= cellNominalVoltage)
    {
      battery.status = 2;
    }
  }

  for (int i = 0; i < NUM_CELLS; i++)
  {
    if (battery.cellVoltages[i] <= cellCriticalVoltage)
    {
      battery.status = 3;
    }
  }

  // // Initialize battery errors
  // battery.errors = 0x00;

  // // Detect if battery is present
  // if (battery.voltage <= 1.0f)
  // {
  //   // Battery is not present
  //   battery.errors |= BATTERY_NOT_PRESENT;
  // }
  // else
  // {
  //   // Battery is present
  //   // Check cell voltages
  //   for (int i = 0; i < num_cells; i++)
  //   {
  //     if (battery.cell_voltages[i] >= cell_charged_voltage)
  //     {
  //       battery.errors |= CELL_OVERVOLTAGE_FLAG;
  //     }

  //     if (battery.cell_voltages[i] <= cell_critical_voltage)
  //     {
  //       battery.errors |= CELL_CRITICAL_FLAG;
  //     }
  //     else if (battery.cell_voltages[i] <= cell_nominal_voltage)
  //     {
  //       battery.errors |= CELL_LOW_FLAG;
  //     }
  //   }
  // }

  // if (battery.temperature <= temperature_underheat_critical)
  // {
  //   battery.errors |= TEMPERATURE_UNDERHEAT_CRITICAL_FLAG;
  // }
  // else if (battery.temperature <= temperature_underheat_warning)
  // {
  //   battery.errors |= TEMPERATURE_UNDERHEAT_WARNING_FLAG;
  // }

  // if (battery.temperature >= temperature_overheat_critical)
  // {
  //   battery.errors |= TEMPERATURE_OVERHEAT_CRITICAL_FLAG;
  // }
  // else if (battery.temperature >= temperature_overheat_warning)
  // {
  //   battery.errors |= TEMPERATURE_OVERHEAT_WARNING_FLAG;
  // }

  // if (battery.current >= current_critical)
  // {
  //   battery.errors |= CURRENT_CRITICAL_FLAG;
  // }
  // else if (battery.current >= current_warning)
  // {
  //   battery.errors |= CURRENT_WARNING_FLAG;
  // }

  // // Initialize battery status
  // battery.status = OK;
  // if (
  //     battery.errors & CELL_OVERVOLTAGE_FLAG ||
  //     battery.errors & CELL_LOW_FLAG ||
  //     battery.errors & TEMPERATURE_UNDERHEAT_WARNING_FLAG ||
  //     battery.errors & TEMPERATURE_OVERHEAT_WARNING_FLAG ||
  //     battery.errors & CURRENT_WARNING_FLAG)
  // {
  //   battery.status = WARNING;
  // }

  // if (
  //     battery.errors & CELL_CRITICAL_FLAG ||
  //     battery.errors & TEMPERATURE_UNDERHEAT_CRITICAL_FLAG ||
  //     battery.errors & TEMPERATURE_OVERHEAT_CRITICAL_FLAG ||
  //     battery.errors & CURRENT_CRITICAL_FLAG ||
  //     battery.errors & BATTERY_NOT_PRESENT)
  // {
  //   battery.status = CRITICAL;
  // }

  return battery;
}
