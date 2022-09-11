#include <cmath>

#include "logger.h"
#include "monitoring.h"
#include "state-led.h"
#include "state-machine.h"

#include "./dc_motor.h"
#include "./i2c.h"

// ====== ABOUT ======
#define BOARD_NAME "Motor Controller"
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
#define STATE_LED_PIN 10

// ====== Motor Definitions ======
#define MOTOR_1_ENABLE_PIN 5
#define MOTOR_1_A_PIN 4
#define MOTOR_1_B_PIN 7
#define MOTOR_1_PWM_PIN 5

#define MOTOR_2_ENABLE_PIN 6
#define MOTOR_2_A_PIN 8
#define MOTOR_2_B_PIN 9
#define MOTOR_2_PWM_PIN 6

// ====== GLOBALS ======
// DC Motor instance for motor 1
DCMotor *motor_1;

// DC Motor instance for motor 2
DCMotor *motor_2;

// Logger instance for the board
Logger *logger;

// Monitoring instance for the board
Monitoring *monitoring;

// MotorControllerI2CSlave instance for the board to be interacted with over TwoWire
MotorControllerI2CSlave *motor_controller_i2c_slave;

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
    // get the most recent Master state
    State master_state = motor_controller_i2c_slave->getMasterState();

    // If master state is HALT or FAULT set board state to HALT
    if (master_state == State::HALT || master_state == State::FAULT)
    {
        s->transitionTo(State::HALT);
    }
    // Transition to RUNNING if master RUNNING
    else if (master_state == State::RUNNING)
    {
        s->transitionTo(State::RUNNING);
    }
    // Default transition to IDLE
    else
    {
        s->transitionTo(State::IDLE);
    }
}

/**
 * @brief TwoWire handler for on receive events
 *
 * @param num_bytes Number of bytes received
 */
void onReceive(int num_bytes)
{
    motor_controller_i2c_slave->onReceive(num_bytes);
}

/**
 * @brief TwoWire handler for on request events
 */
void onRequest()
{
    motor_controller_i2c_slave->onRequest();
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

    logger->info("Initializing Monitoring...");
    // Create the Monitoring instance
    monitoring = new Monitoring();
    logger->info("Monitoring initialized");

    logger->info("Initializing the State Machine...");
    // Create state machine and initialize
    state = new StateMachine(stateUpdateHandler, logger);
    logger->info("State Machine initialized");

    logger->info("Initializing the State LED...");
    state_led = new StateLED(state);
    state_led->init<STATE_LED_PIN>(NUM_LEDS);
    logger->info("State LED initialized");

    logger->info("Initializing Motors...");
    // Create DC Motor controllers
    motor_1 = new DCMotor(MOTOR_1_A_PIN, MOTOR_1_B_PIN, MOTOR_1_PWM_PIN, MOTOR_1_ENABLE_PIN);
    motor_2 = new DCMotor(MOTOR_2_A_PIN, MOTOR_2_B_PIN, MOTOR_2_PWM_PIN, MOTOR_2_ENABLE_PIN);
    logger->info("Motors initialized");

    logger->info("Initializing TwoWire...");
    // Initialize TwoWire communication
    motor_controller_i2c_slave = new MotorControllerI2CSlave(0x00, state, motor_1, motor_2);
    motor_controller_i2c_slave->begin(onReceive, onRequest);
    logger->info("TwoWire initialized");

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

        // Update the monitoring instance at the start of the main code execution
        monitoring->start();

        // After updating all inputs, update the state machine
        state->update();

        // Enable motors if RUNNING
        if (state->getCurrentState() == State::RUNNING)
        {
            motor_1->setEnabled();
            motor_2->setEnabled();
        }
        // Otherwise, disable motors
        else
        {
            motor_1->setDisabled();
            motor_2->setDisabled();
        }

        // Update outputs to the motor driver
        motor_1->writeSpeed();
        motor_2->writeSpeed();

        // Update the State LED with the latest state and effect
        int32_t master_offset = motor_controller_i2c_slave->getMasterOffset();
        // Use most recent master time offset to keep pulsing smooth between heartbeat messages
        state_led->update(master_offset);

        // Update the monitoring instance at the end of the main code execution
        monitoring->end();
    }
}
