#include <Wire.h>
#include <SoftwareWire.h>
#include <Adafruit_SSD1306.h>

#include <RB_Debug.h>
#include <Display.h>
#include <State.h>
#include <Registers.h>
#include <I2C.h>
#include <Board.h>
#include <StateLED.h>

// ====== LOOP RATE ======
#define RATE                            10//Hz

// ====== DEBUG ======
#define DEBUG                           true
#define DEBUG_BAUD_RATE                 115200

// ====== STATE ======
#define STATE_LED_PIN                   10
#define STATE_LED_NUM                   4

// ====== SOFTWARE I2C ======
#define SOFTWARE_WIRE_SDA               14
#define SOFTWARE_WIRE_SCL               15

// ====== SCREEN ======
#define SCREEN_ADDRESS                  0x3C
#define SCREEN_WIDTH                    128 // OLED display width, in pixels
#define SCREEN_HEIGHT                   64  // OLED display height, in pixels

// ====== I2C ======
//Define address select pins
#define ADDRESS_BIT_0_PIN               16
#define ADDRESS_BIT_1_PIN               0

// ====== GLOBALS ======
Debug *debug;
State *state;
StateLED *stateLED;
Adafruit_SSD1306 *screen;
Display *display;
I2CDevice *i2cDevice;
SoftwareWire softwareWire(SOFTWARE_WIRE_SDA, SOFTWARE_WIRE_SCL);

// ====== Handler Functions ======
// TODO use stock SSD1306 lib
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

  // heartbeat time
  d->write(0, 10 * 12, 1, "HB" + String(i2cDevice->getMasterTime() % 1000));

  s->display();
}

void stateHandler(State *s) {
  // Mirror the masters state
  s->transitionTo(i2cDevice->getMasterState());
}

void onReceiveHandler(int bytes) {
  WireMessage message = i2cDevice->receiveEvent(bytes);

  switch (message.message_type) {
    // TODO

    default:
      // TODO debug
      break;
  }
}

void onRequestHandler() {
  switch (i2cDevice->getRegister()) {
    // TODO

    // TODO move to I2CDevice
    case Registers::STATE_STATUS:
      i2cDevice->writeByte(state->getCurrentState());
      break;

    default:
      // TODO debug
      break;
  }
}

void setup() {
  Serial.begin(DEBUG_BAUD_RATE);
  while (!Serial && millis() < 500) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  debug = new Debug("Motor Controller", DEBUG);

  // Initialize state machine and LED
  debug->print("Initializing State Machine...");
  state = new State(stateHandler);
  debug->print("OK\n");

  debug->print("Initializing Debug LED...");
  stateLED = new StateLED(state);
  stateLED->init<STATE_LED_PIN>(STATE_LED_NUM);
  debug->print("OK\n");

  // Initialize software wire bus for onboard peripherals
  debug->print("Initializing Peripheral Wire Bus...");
  softwareWire.begin();
  debug->print("OK\n");

  // Init screen
  debug->print("Initializing Screen...");
  screen = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &softwareWire);

  screen->onWireBegin(wireBegin);
  screen->onWireBeginTransmission(wireBeginTransmission);
  screen->onWireEndTransmission(wireEndTransmission);
  screen->onWireWrite(wireWrite);
  screen->setRotation(0);

  if (!screen->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    debug->print("SSD1306 allocation failed");
    state->transitionTo(FAULT);
    stateLED->update();
    for (;;); // Don't proceed, loop forever
  }

  display = new Display("Status Light", screen, SCREEN_WIDTH, SCREEN_HEIGHT, displayHandler);
  display->initializing("Display...", 0.5f);
  delay(500);
  debug->print("OK\n");

  // ====== I2C Communication ======
  //Add offset to base address
  byte wire_address = STATUS_LIGHT_BASE_ADDRESS + I2CDevice::getAddressOffset(ADDRESS_BIT_0_PIN, ADDRESS_BIT_1_PIN);
  debug->print("Initializing Wire Bus at address 0x" + String(wire_address, HEX) + "...");
  display->initializing("I2C (0x" + String(wire_address, HEX) + ")", 0.7f);

  // Join I2C bus as slave with address
  i2cDevice = new I2CDevice(wire_address);
  Wire.begin(i2cDevice->getAddress());
  Wire.onReceive(onReceiveHandler);
  Wire.onRequest(onRequestHandler);
  delay(500);
  debug->print("OK\n");

  // After successful initializaton, set state to idle to indicate its ready
  state->transitionTo(IDLE);
  stateLED->update();
  debug->print("Startup Complete!\n\n");
  display->initializing("Complete", 1.0f);
  delay(500);
  display->clear();
}


/**

*/
uint32_t lastLoopTime = 0;
void loop() {
  if (millis() >= lastLoopTime + (1000 * (1 / RATE))) {
    lastLoopTime = millis();

    state->update(i2cDevice);
    stateLED->update(i2cDevice->getMasterTime());

    // update display
    display->update();

  }
}
