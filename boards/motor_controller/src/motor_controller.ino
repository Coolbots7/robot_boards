#include <cmath>

#include "logger.h"
#include "state-machine.h"

// ====== ABOUT ======
#define BOARD_NAME "Motor Controller"
#define HARDWARE_VERSION "0.1.0"
#define FIRMWARE_VERSION "0.2.0"

// ====== DEBUG ======
#define ENABLE_LOGGING true
#define SERIAL_BAUD_RATE 115200
#define SERIAL_INIT_DELAY 500

// ====== RATE ======
// Maximum execution rate of the main loop
#define RATE 40 // Hz

// ====== GLOBALS ======
// Logger instance for the board
Logger *logger;

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

    // Initialize the logger
    logger->begin(BOARD_NAME, HARDWARE_VERSION, FIRMWARE_VERSION);

    logger->info("Setup Beginning...");

    logger->info("Initializing the State Machine...");
    // Create state machine and initialize
    state = new StateMachine(stateUpdateHandler, logger);
    logger->info("State Machine initialized");

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
    }
}
