#include <SoftwareWire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Debug.h"
#include "Display.h"
#include "State.h"
#include "StateLED.h"
#include "Loop.h"

#define DEBUG                           true

#define RATE                            20//Hz
#define SERIAL_BAUD_RATE                115200

// ====== STATE ======
#define STATE_LED_PIN                   7

// ====== ESTOP LOOP ======
#define ESTOP_CONTROL_PIN               9
#define ESTOP_LOOP_SENSE_PIN            2
#define ESTOP_LOOP_INDICATOR_PIN        8

// ====== SOFTWARE WIRE ======
#define SOFTWARE_WIRE_SDA               6
#define SOFTWARE_WIRE_SCL               5


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// ====== GLOBALS ======
Debug *debug;

State *state;
StateLED *stateLED;

Adafruit_SSD1306 *screen;
Display *display;

//Create SoftwareWire master
SoftwareWire softwareWire(SOFTWARE_WIRE_SDA, SOFTWARE_WIRE_SCL);

Loop *estop_loop;

void setup() {
  // Initialize serial debugging
  Serial.begin(SERIAL_BAUD_RATE);
  debug = new Debug(DEBUG);


  // Initialize state machine and LED
  debug->print("Initializing State Machine...");
  state = new State();
  debug->addObject(state);
  stateLED = new StateLED(STATE_LED_PIN, state);
  debug->print("OK\n");


  //Initialize software wire bus
  //  softwareWire = new SoftwareWire(SOFTWARE_WIRE_SDA, SOFTWARE_WIRE_SCL);
  // join i2c bus as master
  softwareWire.begin();


  // Initialize display
  debug->print("Initializing display...");
//  screen = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
//  while (!screen->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS && millis() < 1000)) {
//    Serial.println(F("SSD1306 allocation failed"));
//    delay(100);
//  }
//  screen->setRotation(0);
//  display = new Display(screen, SCREEN_WIDTH, SCREEN_HEIGHT);
//  display->setStateMachine(state);
  delay(500);
  //TODO show startup progress on display
//  display->initializing("Starting");
  debug->print("OK\n");


  //Initialize estop loop
//  display->initializing("EStop Loop", 0.1f);
  estop_loop = new Loop(ESTOP_CONTROL_PIN, ESTOP_LOOP_SENSE_PIN, ESTOP_LOOP_INDICATOR_PIN);
  debug->addObject(estop_loop);
//  display->setLoop(estop_loop);


  //TODO REMOVE ME
  for (double i = 0.1f; i < 1.0f; i += 0.01) {
//    display->initializing("EStop Loop", i);
    delay(5);
  }


  // After successful initializaton, set state to idle to indicate its ready
  state->transitionTo(IDLE);


  Serial.print("Initializing Complete!\n");


  // 
  estop_loop->energise();
}

void loop() {
  
  estop_loop->update();
  
  // After updating all inputs, update state machine
  //  state->update();
  stateLED->update();

  // Update serial debug and display
  debug->update();
//  display->update();


  delay(1000 * (1 / RATE));
}
