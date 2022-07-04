#include <Wire.h>
#include <Adafruit_SSD1306.h>

#include <communication/RB_Debug.h>
#include <Display.h>
#include <state/State.h>
#include <communication/I2C.h>
#include <boards/Board.h>
#include <boards/MotorControllerBoard.h>
#include <state/StateLED.h>

#include "SBUS.h"

#define RATE                            30//Hz

#define DEBUG                           true
#define DEBUG_BAUD_RATE                 115200

// ====== STATE ======
#define STATE_LED_PIN                   32

// ====== SOFTWARE I2C ======
#define SOFTWARE_WIRE_SDA               25
#define SOFTWARE_WIRE_SCL               27
#define SOFTWARE_WIRE_FREQUENCY         100000

// ====== Screen ======
#define SCREEN_ADDRESS                  0x3C
#define SCREEN_WIDTH                    128 // OLED display width, in pixels
#define SCREEN_HEIGHT                   32 // OLED display height, in pixels

// ====== SBUS ======
#define SBUS_RX_PIN                     16
#define SBUS_TX_PIN                     17

// ====== JOY CHANNEL MAPPINGS ======
#define LINEAR_X_CHANNEL                2
#define ANGULAR_Z_CHANNEL               1
#define ARM_CHANNEL                     7
#define DEAD_MAN_CHANNEL                10
#define THROTTLE_SCALER_CHANNEL         11

// ====== GLOBALS ======
Debug *debug;
State *state;
StateLED *stateLED;
Adafruit_SSD1306 *screen;
Display *display;
SBUSReceiver sbus;

MotorControllerBoard front_motors(MOTOR_CONTROLLER_BASE_ADDRESS + b);
MotorControllerBoard back_motors(MOTOR_CONTROLLER_BASE_ADDRESS);

// ====== Handler Functions ======
void wireBegin() {
  Wire1.begin();
}

void wireBeginTransmission(uint8_t addr) {
  Wire1.beginTransmission(addr);
}

void wireEndTransmission() {
  Wire1.endTransmission();
}

void wireWrite(uint8_t c) {
  Wire1.write(c);
}

void displayHandler(Display *d, Adafruit_SSD1306 *s) {
  s->clearDisplay();

  // state
  d->write(0, 0 * 12, 1, "State\n" + State::getStateDescription(state->getCurrentState()).substring(0, 4));

  // RSSI
  d->write(0, 2 * 12, 1, "RSSI\n" + String(sbus.getRSSI()));

  // lost frames
  d->write(0, 4 * 12, 1, "LF\n" + String(sbus.getLostFrames()));

  // failsafe
  d->write(0, 6 * 12, 1, "FS\n" + String(sbus.getFailsafe()));

  // heartbeat
  d->write(0, 8 * 12, 1, "HB\n" + String(millis() % 1000));

  s->display();
}

void stateHandler(State *s) {
  // TODO HALT / FAULT if N heartbeat messages are missed from a board
  // TODO HALT if board is not connected / or HALT or FAULT state
  //  if(front_motors.getState() != IDLE || front_motors.getState() != RUNNING) {
  //    s->transitionTo(HALT);
  //    return;
  //  }
  //  if(back_motors.getState() != IDLE || back_motors.getState() != RUNNING) {
  //    s->transitionTo(HALT);
  //    return;
  //  }

  // TODO if no SBUS receiver, transition to FAULT

  // Get SBUS data
  SBUSData data = sbus.getData();

  // If failsafe, transition to HALT
  if (data.failsafe) {
    s->transitionTo(HALT);
    return;
  }

  // TODO use SBUS channel as wireless estop / dead man switch
  // TODO HALT if estop loop is open
  if (data.channels[DEAD_MAN_CHANNEL - 1] < 0.0f) {
    s->transitionTo(HALT);
    return;
  }

  // Use SBUS channel as arm / disarm
  if (data.channels[ARM_CHANNEL - 1] < 0.0f) {
    s->transitionTo(IDLE);
    return;
  }

  // If not failsafe, transition to RUNNING
  s->transitionTo(RUNNING);
}

/**

*/
void setup() {
  Serial.begin(DEBUG_BAUD_RATE);
  while (!Serial && millis() < 500) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  debug = new Debug("RC Board", DEBUG);

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
  Wire1.begin(SOFTWARE_WIRE_SDA, SOFTWARE_WIRE_SCL, SOFTWARE_WIRE_FREQUENCY);
  debug->print("OK\n");

  // Init screen
  debug->print("Initializing Screen...");
  screen = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1);

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

  display = new Display("RC Board", screen, SCREEN_WIDTH, SCREEN_HEIGHT, displayHandler);
  delay(500);
  //TODO show startup progress on display
  display->initializing("Display...", 0.5f);
  delay(500);
  debug->print("OK\n");

  // Join I2C bus as slave with address
  debug->print("Initializing Wire Bus as master...");
  display->initializing("I2C", 0.7f);
  Wire.begin();
  Board::adoptStaticBoards();
  delay(500);
  debug->print("OK\n");

  // TODO auto-adopt non-configurable board. status lights, BMSs, e stops

  // Initialize SBUS
  //  sbus = new SBUS();
  debug->print("Initializing SBUS...");
  display->initializing("SBUS", 0.9f);
  sbus.begin(SBUS_RX_PIN, SBUS_TX_PIN);
  delay(500);
  debug->print("OK\n");

  // After successful initializaton, set state to idle to indicate its ready
  state->transitionTo(IDLE);
  stateLED->update();
  debug->print("Startup Complete!\n\n");
  display->initializing("Complete", 1.0f);
  delay(500);
  display->clear();

  // Set screen rotation to vertical
  screen->setRotation(1);
}

/**

*/
uint32_t lastLoopTime = 0;
void loop() {
  if (millis() >= lastLoopTime + (1000 * (1 / RATE))) {
    lastLoopTime = millis();

    // Get SBUS data
    SBUSData data;
    sbus.read(data);

    if (!data.failsafe) {
      // joy to cmd vel
      float linear_x = data.channels[LINEAR_X_CHANNEL - 1];
      if (abs(linear_x) <= 0.1f) {
        linear_x = 0.0f;
      }

      float angular_z = data.channels[ANGULAR_Z_CHANNEL - 1];
      if (abs(angular_z) <= 0.1f) {
        angular_z = 0.0f;
      }

      float throttle_scaler = (data.channels[THROTTLE_SCALER_CHANNEL - 1] + 1.0f) / 2.0f;

      // TODO cmd vel to motor speeds
      float left_speed = (linear_x + angular_z) * throttle_scaler;
      float right_speed = (linear_x - angular_z) * throttle_scaler;

      // Write motor speeds
      front_motors.setSpeed1(left_speed);
      back_motors.setSpeed2(-left_speed);

      front_motors.setSpeed2(right_speed);
      back_motors.setSpeed1(-right_speed);
    }

    // --- Heartbeat ---
    int8_t front_motor_status = front_motors.heartbeat(state->getCurrentState());
    debug->print("Front Motor Status: " + String(front_motor_status) + "\n\n");

    int8_t back_motor_status = back_motors.heartbeat(state->getCurrentState());
    debug->print("Back Motor Status: " + String(back_motor_status) + "\n\n");

    Board::heartbeatStaticBoards(state->getCurrentState());

    // TODO publish heartbeat

    // --- TELEOP ---
    if (state->getCurrentState() == IDLE || state->getCurrentState() == HALT || state->getCurrentState() == FAULT) {
      front_motors.disable1();
      front_motors.disable2();
      back_motors.disable1();
      back_motors.disable2();

      // TODO publish motor controller speeds
      front_motors.setSpeed1(0);
      front_motors.setSpeed2(0);
      back_motors.setSpeed1(0);
      back_motors.setSpeed2(0);
    }
    else {
      front_motors.enable1();
      front_motors.enable2();
      back_motors.enable1();
      back_motors.enable2();
    }


    // --- Update State ---
    // After updating all inputs, update state machine
    state->update(NULL);
    stateLED->update();
    

    // --- Update Display ---
    display->update();
  }
}
