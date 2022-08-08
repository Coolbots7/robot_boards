#include <cmath>

#include <Adafruit_SSD1306.h>

#include "display.h"
#include "logger.h"
#include "monitoring.h"
#include "state-led.h"
#include "state-machine.h"

// ====== ABOUT ======
#define BOARD_NAME "Main Board"
#define HARDWARE_VERSION "0.1.0"
#define FIRMWARE_VERSION "0.5.0"

// ====== DEBUG ======
#define ENABLE_LOGGING true
#define SERIAL_BAUD_RATE 115200
#define SERIAL_INIT_DELAY 500

// ====== RATE ======
// Maximum execution rate of the main loop
#define RATE 40 // Hz

// ====== State LED ======
// Number of inline addressable LEDs attached to the board
#define NUM_LEDS 1
// Hardware pin the addressable LEDs are attached to
#define STATE_LED_PIN 32

// ====== SOFTWARE I2C ======
// Define the hardware SDA pin for the onboard peripheral Wire bus
#define SOFTWARE_WIRE_SDA 25
// Define the hardware SCL pin for the onboard peripheral Wire bus
#define SOFTWARE_WIRE_SCL 27
// Define the clock speed for the onboard peripheral Wire bus
#define SOFTWARE_WIRE_FREQUENCY 100000

// ====== Display ======
// OLED display address
#define DISPLAY_ADDRESS 0x3C
// OLED display width, in pixels
#define DISPLAY_WIDTH 128
// OLED display height, in pixels
#define DISPLAY_HEIGHT 32

// ====== Wire Handler Functions ======
// Handler for calling begin on the peripheral Wire bus
void wireBegin()
{
    Wire1.begin();
}

// Handler for calling beginTransmission on the peripheral Wire bus
void wireBeginTransmission(uint8_t addr)
{
    Wire1.beginTransmission(addr);
}

// Handler for calling endTransmission on the peripheral Wire bus
void wireEndTransmission()
{
    Wire1.endTransmission();
}

// Handler for calling write on the peripheral Wire bus
void wireWrite(uint8_t c)
{
    Wire1.write(c);
}

// ====== GLOBALS ======
// SSD1306 display instance for the onboard display
Adafruit_SSD1306 *screen;

// Display instance for the board
Display *display;

// Logger instance for the board
Logger *logger;

// Monitoring instance for the board
Monitoring *monitoring;

// State LED instance for the board
StateLED *state_led;

// State Machine instance for the board
StateMachine *state;

// Keep track of the last time the main loop was run
uint32_t last_loop_time = 0;

// Time between main code executions based on the define rate
const uint32_t LOOP_TIME = round(1000 / RATE);

/**
 * @brief State Machine update handler
 *
 * @param s State Machine
 */
void stateUpdateHandler(StateMachine *s)
{
}

/**
 * @brief Update the display in the main loop
 *
 * @param display Display instance to update
 */
void displayHandler(Display *display)
{
    // Clear the display buffer
    display->clear();

    // Show the board state
    display->leftJustify(display->getLineY(0), "S:" + StateMachine::getStateDescription(state->getCurrentState()).substring(0, 4));

    // Show the clock time
    char time_buffer[6];
    sprintf(time_buffer, "T:%03d", millis() % 1000);
    display->rightJustify(display->getLineY(0), String(time_buffer));

    // show monitoring information
    char execution_time_buffer[8];
    sprintf(execution_time_buffer, "E:%.0fms", monitoring->getAverageExecutionTime());
    display->rightJustify(display->getLineY(1), String(execution_time_buffer));
    char cycle_time_buffer[14];
    sprintf(cycle_time_buffer, "C:%.0fms(%.0fhz)", monitoring->getAverageCycleTime(), monitoring->getAverageCycleRate());
    display->rightJustify(display->getLineY(2), String(cycle_time_buffer));

    // Push buffer to display
    display->display();
}

/**
 * @brief Initialization function called a single time when powered on for setting up the board
 */
void setup()
{
    // Create logger for serial debugging
    logger = new Logger(&Serial, ENABLE_LOGGING);

    // Wait for serial port to connect
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial && millis() < SERIAL_INIT_DELAY)
    {
        ;
    }

    if (!Serial)
    {
        // If serial initialization failed, disable logging and continue
        logger->disable();
    }
    else
    {
        // If serial initialization was successful, initialize the logger
        logger->begin(BOARD_NAME, HARDWARE_VERSION, FIRMWARE_VERSION);
    }

    logger->info("Setup Beginning...");

    // Create the Monitoring instance
    monitoring = new Monitoring();

    // Create state machine and initialize
    state = new StateMachine(stateUpdateHandler, logger);

    logger->info("Initializing the State LED...");
    state_led = new StateLED(state);
    state_led->init<STATE_LED_PIN>(NUM_LEDS);
    logger->info("State LED initialized");

    // Initialize second Wire bus for onboard peripherals
    logger->info("Initializing onboard peripheral Wire bus...");
    Wire1.begin(SOFTWARE_WIRE_SDA, SOFTWARE_WIRE_SCL, SOFTWARE_WIRE_FREQUENCY);
    logger->info("Wire bus initialized");

    // Create the SSD1306 screen instance
    logger->info("Initializing the Screen...");
    screen = new Adafruit_SSD1306(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire1);

    // This is required because the SSD1306 library does not property use the provided I2C object
    screen->onWireBegin(wireBegin);
    screen->onWireBeginTransmission(wireBeginTransmission);
    screen->onWireEndTransmission(wireEndTransmission);
    screen->onWireWrite(wireWrite);

    // Initialize the screen, halt if error
    if (!screen->begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS))
    {
        logger->warning("SSD1306 allocation failed");
        state->transitionTo(FAULT);
        state_led->update();

        // Don't proceed, loop forever
        for (;;)
        {
            ;
        }
    }
    logger->info("Screen initialized");

    // Initialize the Display
    logger->info("Initializing the Display...");
    display = new Display(screen, displayHandler);
    logger->info("Display initialized");

    // Transition to the IDLE state and complete setup
    state->transitionTo(IDLE);
    display->initializing("Complete", 1.0f);
    logger->info("Setup Complete!");
}

/**
 * @brief Main loop called as fast as possible after the setup function
 */
void loop()
{
    // Control the maximum cycle rate of the main code loop
    if (millis() >= (last_loop_time + LOOP_TIME))
    {
        last_loop_time = millis();

        // Update the monitoring instance at the start of the main code execution
        monitoring->start();

        // After updating all inputs, update the state machine
        state->update();

        // Update the State LED with the latest state and effect
        state_led->update();

        // After updating the state, update the display
        display->update();

        // Update the monitoring instance at the end of the main code execution
        monitoring->end();
    }
}
