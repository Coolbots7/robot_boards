#include <cmath>

#include "logger.h"
#include "state-led.h"
#include "state-machine.h"

// ====== ABOUT ======
#define BOARD_NAME "Main Board"
#define HARDWARE_VERSION "0.1.0"
#define FIRMWARE_VERSION "0.3.0"

// ====== DEBUG ======
#define ENABLE_LOGGING true
#define SERIAL_BAUD_RATE 115200
#define SERIAL_INIT_DELAY 500

// ====== RATE ======
// Maximum execution rate of the main loop
#define RATE 30 // Hz

// ====== State LED ======
// Number of inline addressable LEDs attached to the board
#define NUM_LEDS 1
// Hardware pin the addressable LEDs are attached to
#define STATE_LED_PIN 32

// ====== GLOBALS ======
// Logger instance for the board
Logger *logger;

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

    // Create state machine and initialize
    state = new StateMachine(stateUpdateHandler, logger);

    logger->info("Initializing the State LED...");
    state_led = new StateLED(state);
    state_led->init<STATE_LED_PIN>(NUM_LEDS);
    logger->info("State LED initialized");

    // Transition to the IDLE state and complete setup
    state->transitionTo(IDLE);
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

        // After updating all inputs, update the state machine
        state->update();

        // Update the State LED with the latest state and effect
        state_led->update();
    }
}
