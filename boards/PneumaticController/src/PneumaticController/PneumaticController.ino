#include <Wire.h>

#include "Debug.h"
#include "Display.h"
#include "State.h"
#include "StateLED.h"
#include "I2CController.h"
#include "PressureTransducer.h"
#include "Solenoid.h"
#include "Switch.h"

#define DEBUG                           true

#define RATE                            20//Hz
#define SERIAL_BAUD_RATE                115200

#define BUS_VOLTAGE                     4.766

// ====== STATE ======
#define STATE_LED_PIN                   4

// ====== I2C Definitions ======
#define BASE_WIRE_ADDRESS               0x40

// ====== PRESSURE TRANSDUCERS ======
#define TRANSDUCER_COUNT                4

const uint8_t transducer_pins[TRANSDUCER_COUNT] = {A5, A4, A3, A2};

#define TRANSDUCER_VOLTAGE_MIN          0.5
#define TRANSDUCER_VOLTAGE_MAX          4.5
#define TRANSDUCER_PRESSURE_MIN         0//psi
#define TRANSDUCER_PRESSURE_MAX         100//psi
#define TRANSDUCER_VOLTAGE_MIN_OFFSET   0.05


// ====== SOLENOIDS ======
#define SOLENOID_COUNT                  6

const uint8_t solenoid_pins[SOLENOID_COUNT] = {2, 3, 4, 5, 6, 7};


// ====== SWITCHES ======
#define SWITCH_COUNT                    3

const uint8_t switch_pins[SWITCH_COUNT] = {2, 3, 4};


// ====== GLOBALS ======
Debug *debug;

State *state;
StateLED *stateLED;

I2CController* i2cController;

PressureTransducer *transducers[TRANSDUCER_COUNT];

Solenoid *solenoids[SOLENOID_COUNT];

Switch *switches[SWITCH_COUNT];

Display *display;
Graph *graph;

void setup() {
  // Initialize serial debugging
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.print("Pneumatic Controller\n");
  Serial.print("Initializing...");
  debug = new Debug(DEBUG);


  // Initialize display
  debug->print("Initializing display...");
  display = new Display();
  //TODO show startup progress on display
  debug->print("OK\n");


  // Initialize state machine and LED
  debug->print("Initializing State Machine...");
  state = new State();
  debug->addObject(state);
  display->setStateMachine(state);
  stateLED = new StateLED(STATE_LED_PIN, state);
  debug->print("OK\n");


  // ====== Initialize I2C Communication Layer ======
  debug->print("Initializing I2C...");
  byte wire_address = BASE_WIRE_ADDRESS;
  i2cController = new I2CController(wire_address);
  Wire.begin(i2cController->getAddress());
  Wire.onReceive([] (int howMany) {
    i2cController->receiveEvent(howMany);
  });
  Wire.onRequest([] () {
    i2cController->requestEvent();
  });
  debug->print("OK\n");


  // Initialize pressure transducers
  debug->info("Initializing " + String(TRANSDUCER_COUNT) + " Pressure Transducers...");
  for (uint8_t i = 0; i < TRANSDUCER_COUNT; i++) {
    debug->info("Pressure Transducer " + String(i) + "...");
    transducers[i] = new PressureTransducer(transducer_pins[i], BUS_VOLTAGE, TRANSDUCER_VOLTAGE_MIN, TRANSDUCER_VOLTAGE_MAX, TRANSDUCER_PRESSURE_MIN, TRANSDUCER_PRESSURE_MAX, TRANSDUCER_VOLTAGE_MIN_OFFSET);
    debug->info("OK\n");
  }
  display->setPressureTransducer(transducers[0]);
  debug->info("Initializing Pressure Transducers...OK\n");


  // Initialize solenoids
  debug->info("Initializing " + String(SOLENOID_COUNT) + " Solenoids...");
  for (uint8_t i = 0; i < SOLENOID_COUNT; i++) {
    debug->info("Solenoid " + String(i) + "...");
    solenoids[i] = new Solenoid(solenoid_pins[i]);
    debug->info("OK\n");
  }
  debug->info("Initializing Solenoids...OK\n");


  // Initialize switches
  debug->info("Initializing " + String(SWITCH_COUNT) + " Switches...");
  for (uint8_t i = 0; i < SWITCH_COUNT; i++) {
    debug->info("Switch " + String(i) + "...");
    switches[i] = new Switch(switch_pins[i]);
    debug->info("OK\n");
  }
  debug->info("Initializing Solenoids...OK\n");


  // Create graph for pressure transducer PSI
  graph = new Graph(1.0f, 32 - 4);
  display->setGraph(graph);


  //TODO REMOVE ME
  while (millis() < 5000) {
    //Pretend like were doing stuff to test starting state
  }


  // After successful initializaton, set state to idle to indicate its ready
  state->transitionTo(IDLE);
  
  
  Serial.print("Initializing Complete!\n");
}

void loop() {

  //TODO update sensors

  // After updating all systems, update graphs
  graph->push(transducers[0]->getPressurePSI());

  // After updating all inputs, update state machine
  //  state->update();
  stateLED->update();

  if (state->getState() == IDLE) {
    //TODO deactivate all outputs
  }
  else if (state->getState() == RUNNING) {
    //TODO run
  }
  else if (state->getState() == FAULT) {
    //TODO deactivate all outputs
    //TODO how to transition back to STARTING?
  }

  // Update serial debug and display
  debug->update();
  display->update();

  // Run the main loop at the set rate
  delay(1000 * (1 / RATE));
}
