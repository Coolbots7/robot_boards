#include <cmath>

#include "Wire.h"

#include <Adafruit_SSD1306.h>

#include "display.h"
#include "logger.h"
#include "monitoring.h"
#include "motor_controller/motor_controller.h"
#include "state-led.h"
#include "state-machine.h"
#include "status_light/status_light.h"
#include "twowire/master.h"

#include "./sbus_receiver.h"

// ====== ABOUT ======
#define BOARD_NAME "Main Board"
#define HARDWARE_VERSION "0.1.0"
#define FIRMWARE_VERSION "0.8.0"

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
#define SOFTWARE_WIRE_FREQUENCY 1000000UL

// ====== Display ======
// OLED display address
#define DISPLAY_ADDRESS 0x3C
// OLED display width, in pixels
#define DISPLAY_WIDTH 128
// OLED display height, in pixels
#define DISPLAY_HEIGHT 32

// ====== SBUS ======
// RF transceiver SBUS receive pin
#define SBUS_RX_PIN 16
// RF transceiver SBUS transmit pin
#define SBUS_TX_PIN 17

// ====== JOY CHANNEL MAPPINGS ======
// Controller analog joystick dead zone
#define CONTROLLER_DEAD_ZONE 0.1f
// SBUS controller channel for forward / reverse speed
#define LINEAR_X_CHANNEL 2
// SBUS controller channel for left / right speed
#define ANGULAR_Z_CHANNEL 1
// SBUS controller channel for arm command
#define ARM_CHANNEL 7
// SBUS controller channel for max speed control
#define THROTTLE_SCALER_CHANNEL 11

// ====== GLOBALS ======
// SSD1306 display instance for the onboard display
Adafruit_SSD1306 *screen;

// Display instance for the board
Display *display;

// I2CMaster instance to communicate with boards over TwoWire
I2CMaster *i2c_master;

// Logger instance for the board
Logger *logger;

// Monitoring instance for the board
Monitoring *monitoring;

// MotorControllerInterface instance to interface with a Motor Controller board
MotorControllerInterface *motor_controller_interface;

// SBUS receiver instance for the board
SbusReceiver *sbus_receiver;

// State LED instance for the board
StateLED *state_led;

// StatusLightInterface instance to interface with a Status Light board
StatusLightInterface *status_light;

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
    // Get Motor Controller board state
    State motor_controller_state = motor_controller_interface->state->getBoardState();

    // Get SBUS data
    SbusData data = sbus_receiver->getData();

    // Halt if a board has faulted
    if (motor_controller_state == State::FAULT)
    {
        s->transitionTo(State::HALT);
    }
    // Halt if SBUS has failsafed
    else if (data.failsafe)
    {
        s->transitionTo(State::HALT);
    }
    // Idle if disarmed
    else if (data.channels[ARM_CHANNEL - 1] < 0.0f)
    {
        s->transitionTo(State::IDLE);
    }
    else
    {
        s->transitionTo(State::RUNNING);
    }
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

    // Show SBUS Received Signal Strength Indicator (RSSI) and Failsafe flag
    display->leftJustify(display->getLineY(1), "SS:" + String(sbus_receiver->getRSSI()) + " FS:" + String(sbus_receiver->getFailsafe()));

    // Show SBUS Lost Frames
    display->leftJustify(display->getLineY(2), "LF:" + String(sbus_receiver->getLostFrames()));

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

    logger->info("Initializing Monitoring...");
    // Create the Monitoring instance
    monitoring = new Monitoring();
    logger->info("Monitoring initialized");

    logger->info("Initializing the State Machine...");
    // Create state machine and initialize
    state = new StateMachine(stateUpdateHandler, logger);
    logger->info("State Machine initialized");

    logger->info("Initializing the State LED...");
    // Initialize the onboard State LED
    state_led = new StateLED(state);
    state_led->init<STATE_LED_PIN>(NUM_LEDS);
    logger->info("State LED initialized");

    logger->info("Initializing Peripheral TwoWire bus...");
    // Initialize second Wire bus for onboard peripherals
    Wire1.begin(SOFTWARE_WIRE_SDA, SOFTWARE_WIRE_SCL, SOFTWARE_WIRE_FREQUENCY);
    logger->info("Peripheral TwoWire bus initialized");

    logger->info("Initializing the Screen...");
    // Create the SSD1306 screen instance
    screen = new Adafruit_SSD1306(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire1);

    // Initialize the screen, halt if error
    if (!screen->begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS))
    {
        logger->error("Screen initialization failed!");
        state->transitionTo(State::FAULT);
        state_led->update();

        // Don't proceed, loop forever
        for (;;)
        {
            ;
        }
    }
    logger->info("Screen initialized");

    logger->info("Initializing the Display...");
    // Initialize the Display
    display = new Display(screen, displayHandler);
    logger->info("Display initialized");

    logger->info("Initializing TwoWire as master...");
    display->initializing("I2C", 0.7f);
    // Join TwoWire bus as master
    i2c_master = new I2CMaster();
    i2c_master->begin();
    logger->info("TwoWire initialized");

    logger->info("Initializing Status Light Board...");
    // Initialize the Status Light board interface
    status_light = new StatusLightInterface(0, i2c_master);
    logger->info("Status Light Board initialized");

    logger->info("Initializing Motor Controller Board...");
    // Initialize the Motor Controller interface
    motor_controller_interface = new MotorControllerInterface(0, i2c_master);
    // Reverse the motor 2 direction
    motor_controller_interface->motor_2->setReversed();
    // Set the motors to coast mode
    motor_controller_interface->motor_1->setCoast();
    motor_controller_interface->motor_2->setCoast();
    logger->info("Motor Controller Board initialized");

    // Initialize SBUS communication
    logger->info("Initializing SBUS receiver...");
    display->initializing("SBUS", 0.9f);
    sbus_receiver = new SbusReceiver();
    sbus_receiver->begin(&Serial2, SBUS_RX_PIN, SBUS_TX_PIN);
    logger->info("SBUS receiver initialized");

    // Transition to the IDLE state and complete setup
    state->transitionTo(State::IDLE);
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

        // Read latest SBUS data
        SbusData data;
        bool read_success = sbus_receiver->read(data);
        if (read_success && !data.failsafe)
        {
            logger->info("Read SBUS data");

            // Calculate command velocity from joystick inputs

            // Get linear X command from SBUS data
            float linear_x = data.channels[LINEAR_X_CHANNEL - 1];
            if (abs(linear_x) < CONTROLLER_DEAD_ZONE)
            {
                linear_x = 0.0f;
            }

            // Get angular Y command from SBUS data
            float angular_z = data.channels[ANGULAR_Z_CHANNEL - 1];
            if (abs(angular_z) < CONTROLLER_DEAD_ZONE)
            {
                angular_z = 0.0f;
            }

            // Get throttle scaler command from SBUS data and shift from -1 - 1 to 0 - 1
            float throttle_scaler = (data.channels[THROTTLE_SCALER_CHANNEL - 1] + 1.0f) / 2.0f;

            // Calculate tank drive motor speeds using command velocity values
            float left_speed = (linear_x + angular_z) * throttle_scaler;
            float right_speed = (linear_x - angular_z) * throttle_scaler;

            // Send motor speeds to motor controller
            motor_controller_interface->motor_1->setSpeed(left_speed);
            motor_controller_interface->motor_2->setSpeed(right_speed);
        }
        else if (!read_success)
        {
            logger->warning("SBUS read failed");
        }
        else if (data.failsafe)
        {
            logger->warning("SBUS failsafed");
        }
        else
        {
            logger->warning("SBUS read failed for unknown reason");
        }

        // After updating all inputs, update the state machine
        state->update();

        // Send the master state heartbeat to the Status Light board
        status_light->state->setMasterState(state->getCurrentState());
        // Send the master time heartbeat to the Status Light board
        status_light->state->setMasterTime(millis());

        // Send the master state heartbeat to the Motor Controller board
        motor_controller_interface->state->setMasterState(state->getCurrentState());
        // Send the master time heartbeat to the Motor Controller board
        motor_controller_interface->state->setMasterTime(millis());

        // Update the State LED with the latest state and effect
        state_led->update();

        // After updating the state, update the display
        display->update();

        // Update the monitoring instance at the end of the main code execution
        monitoring->end();
    }
}
