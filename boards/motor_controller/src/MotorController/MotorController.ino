#include <Wire.h>
#include <SoftwareWire.h>
#include <Adafruit_SSD1306.h>

#include <RB_Debug.h>
#include <Display.h>
#include <State.h>
#include <Registers.h>
#include <I2C.h>
#include <Board.h>
#include <MotorControllerBoard.h>
#include <StateLED.h>

#include "DCMotor.h"

// ====== LOOP RATE ======
#define RATE                            10//Hz

// ====== DEBUG ======
#define DEBUG                           true
#define DEBUG_BAUD_RATE                 115200

// ====== STATE ======
#define STATE_LED_PIN                   10

// ====== SOFTWARE I2C ======
#define SOFTWARE_WIRE_SDA               14
#define SOFTWARE_WIRE_SCL               15

// ====== Screen ======
#define SCREEN_ADDRESS                  0x3C
#define SCREEN_WIDTH                    128 // OLED display width, in pixels
#define SCREEN_HEIGHT                   32  // OLED display height, in pixels

// ====== Motor Definitions ======
#define MOTOR_1_ENABLE_PIN              5
#define MOTOR_1_A_PIN                   4
#define MOTOR_1_B_PIN                   7
#define MOTOR_1_PWM_PIN                 5
#define MOTOR_1_CURRENT_SENSE_PIN       A4

#define MOTOR_2_ENABLE_PIN              6
#define MOTOR_2_A_PIN                   8
#define MOTOR_2_B_PIN                   9
#define MOTOR_2_PWM_PIN                 6
#define MOTOR_2_CURRENT_SENSE_PIN       A5

#define CURRENT_SENSE_SCALER            0.140

// ====== I2C Definitions ======
//Define address select pins
#define ADDRESS_BIT_0_PIN               16
#define ADDRESS_BIT_1_PIN               0
#define ADDRESS_BIT_2_PIN               0
#define ADDRESS_BIT_3_PIN               0

// ====== GLOBALS ======
//Debug *debug;
State *state;
StateLED *stateLED;
Adafruit_SSD1306 *screen;
Display *display;
I2CDevice *i2cDevice;
SoftwareWire softwareWire(SOFTWARE_WIRE_SDA, SOFTWARE_WIRE_SCL);

DCMotor motor1(MOTOR_1_A_PIN, MOTOR_1_B_PIN, MOTOR_1_PWM_PIN, MOTOR_1_ENABLE_PIN, MOTOR_1_CURRENT_SENSE_PIN);
DCMotor motor2(MOTOR_2_A_PIN, MOTOR_2_B_PIN, MOTOR_2_PWM_PIN, MOTOR_2_ENABLE_PIN, MOTOR_2_CURRENT_SENSE_PIN);

// ====== Handler Functions ======
void wireBegin() {
  softwareWire.begin();
}

void wireBeginTransmission(uint8_t addr) {
  softwareWire.beginTransmission(addr);
}

void wireEndTransmission() {
  softwareWire.endTransmission();
}

void wireWrite(uint8_t c) {
  softwareWire.write(c);
}

void displayHandler(Display *d, Adafruit_SSD1306 *s) {
  s->clearDisplay();

  // state
  d->write(0, 0 * 12, 1, "State\n" + State::getStateDescription(state->getCurrentState()).substring(0, 4));

  //MOTOR 1
  // enabled
  d->write(0, 2 * 12, 1, "M1 EN\n" + String(motor1.getEnabled() ? "1" : "0"));
  // speed / steps mased on mode?
  d->write(0, 4 * 12, 1, "M1 SP\n" + String(motor1.getSpeed()));

  // MOTOR 2
  // enabled
  d->write(0, 6 * 12, 1, "M2 EN\n" + String(motor2.getEnabled() ? "1" : "0"));
  // speed / steps mased on mode?
  d->write(0, 8 * 12, 1, "M2 SP\n" + String(motor2.getSpeed()));

  // heartbeat time
  d->write(0, 10 * 12, 1, "HB" + String(i2cDevice->getMasterTime() % 1000));

  s->display();
}

void stateHandler(State *s) {
  s->transitionTo(RUNNING);
}

void onReceiveHandler(int bytes) {
  WireMessage message = i2cDevice->receiveEvent(bytes);

  // TODO lock out motors if not in RUNNING state

  switch (message.message_type) {
    case MotorControllerBoard::MOTOR_1_ENABLED:
      motor1.setEnabled(I2CPrim::decodeBool(message.data, sizeof(message.data)));
      break;
    case MotorControllerBoard::MOTOR_1_REVERSED:
      motor1.setReversed(I2CPrim::decodeBool(message.data, sizeof(message.data)));
      break;
    case MotorControllerBoard::MOTOR_1_COAST:
      motor1.setCoast(I2CPrim::decodeBool(message.data, sizeof(message.data)));
      break;
    case MotorControllerBoard::MOTOR_1_SPEED:
      motor1.setSpeed(I2CPrim::decodeFloat(message.data));
      break;

    case MotorControllerBoard::MOTOR_2_ENABLED:
      motor2.setEnabled(I2CPrim::decodeBool(message.data, sizeof(message.data)));
      break;
    case MotorControllerBoard::MOTOR_2_REVERSED:
      motor2.setReversed(I2CPrim::decodeBool(message.data, sizeof(message.data)));
      break;
    case MotorControllerBoard::MOTOR_2_COAST:
      motor2.setCoast(I2CPrim::decodeBool(message.data, sizeof(message.data)));
      break;
    case MotorControllerBoard::MOTOR_2_SPEED:
      motor2.setSpeed(I2CPrim::decodeFloat(message.data));
      break;

    default:
      // TODO debug
      break;
  }
}

void onRequestHandler() {
  switch (i2cDevice->getRegister()) {
    case MotorControllerBoard::MOTOR_1_ENABLED:
      i2cDevice->writeBool(motor1.getEnabled());
      break;
    case MotorControllerBoard::MOTOR_1_REVERSED:
      i2cDevice->writeBool(motor1.getReversed());
      break;
    case MotorControllerBoard::MOTOR_1_COAST:
      i2cDevice->writeBool(motor1.getCoast());
      break;
    case MotorControllerBoard::MOTOR_1_SPEED:
      i2cDevice->writeFloat(motor1.getSpeed());
      break;

    case MotorControllerBoard::MOTOR_2_ENABLED:
      i2cDevice->writeBool(motor2.getEnabled());
      break;
    case MotorControllerBoard::MOTOR_2_REVERSED:
      i2cDevice->writeBool(motor2.getReversed());
      break;
    case MotorControllerBoard::MOTOR_2_COAST:
      i2cDevice->writeBool(motor2.getCoast());
      break;
    case MotorControllerBoard::MOTOR_2_SPEED:
      i2cDevice->writeFloat(motor2.getSpeed());
      break;

    // TODO mode to I2CDevice
    case Registers::STATE_STATUS:
      i2cDevice->writeByte(state->getCurrentState());
      break;

    default:
      // TODO debug
      break;
  }
}

void setup() {
  //  Serial.begin(DEBUG_BAUD_RATE);
  //  while (!Serial && millis() < 500) {
  //    ; // wait for serial port to connect. Needed for native USB port only
  //  }
  //  debug = new Debug("Motor Controller", DEBUG);

  // Initialize state machine and LED
  //  debug->print("Initializing State Machine...");
  state = new State(stateHandler);
  //  debug->print("OK\n");

  //  debug->print("Initializing Debug LED...");
  stateLED = new StateLED(state);
  stateLED->init<STATE_LED_PIN>();
  //  debug->print("OK\n");

  // Initialize software wire bus for onboard peripherals
  //  debug->print("Initializing Peripheral Wire Bus...");
  softwareWire.begin();
  //  debug->print("OK\n");

  // Init screen
  //  debug->print("Initializing Screen...");
  screen = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &softwareWire);

  screen->onWireBegin(wireBegin);
  screen->onWireBeginTransmission(wireBeginTransmission);
  screen->onWireEndTransmission(wireEndTransmission);
  screen->onWireWrite(wireWrite);
  screen->setRotation(0);

  if (!screen->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    //    debug->print("SSD1306 allocation failed");
    state->transitionTo(FAULT);
    stateLED->update();
    for (;;); // Don't proceed, loop forever
  }

  display = new Display("RC Board", screen, SCREEN_WIDTH, SCREEN_HEIGHT, displayHandler);
  delay(500);
  display->initializing("Display...", 0.5f);
  delay(500);
  //  debug->print("OK\n");

  // ====== I2C Communication ======
  //Add offset to base address
  byte wire_address = STATUS_LIGHT_BASE_ADDRESS + I2CDevice::getAddressOffset(ADDRESS_BIT_0_PIN, ADDRESS_BIT_1_PIN);
  //debug->print("Initializing Wire Bus at address 0x" + String(wire_address, HEX) + "...");
  display->initializing("I2C (0x" + String(wire_address, HEX) + ")", 0.7f);

  // Join I2C bus as slave with address
  i2cDevice = new I2CDevice(wire_address);
  Wire.begin(i2cDevice->getAddress());
  Wire.onReceive(onReceiveHandler);
  Wire.onRequest(onRequestHandler);
  delay(500);
  //  debug->print("OK\n");

  // ====== Initialize Motor ======
  //  debug->print("Initializing Motors...");
  display->initializing("Motors", 0.85f);
  motor1.setDisabled();
  motor1.setSpeed(0.0f);
  motor1.setCoast();
  motor1.setCurrentSenseScalar(CURRENT_SENSE_SCALER);

  motor2.setDisabled();
  motor2.setSpeed(0.0f);
  motor2.setCoast();
  motor2.setCurrentSenseScalar(CURRENT_SENSE_SCALER);
  delay(500);
  //  debug->print("OK\n");

  // After successful initializaton, set state to idle to indicate its ready
  state->transitionTo(IDLE);
  stateLED->update(i2cDevice->getMasterTime());
  //  debug->print("Startup Complete!\n\n");
  display->initializing("Complete", 1.0f);
  delay(500);
  display->clear();

  // Set screen rotation to vertical
  screen->setRotation(3);
}


/**

*/
uint32_t lastLoopTime = 0;
void loop() {
  if (millis() >= lastLoopTime + (1000 * (1 / RATE))) {
    lastLoopTime = millis();

    // Disable motors if IDLE, HALT, or FAULT
    if (state->getCurrentState() == IDLE || state->getCurrentState() == HALT || state->getCurrentState() == FAULT) {
      motor1.setDisabled();
      motor2.setDisabled();
    }
    else {
      // TODO return motors to previous enabled state if RUNNING
    }

    state->update(i2cDevice);
    stateLED->update(i2cDevice->getMasterTime());

    // update display
    display->update();
  }
}
