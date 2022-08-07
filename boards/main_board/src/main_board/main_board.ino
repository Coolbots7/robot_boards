#include "logger.h"

// ====== ABOUT ======
#define BOARD_NAME "Main Board"
#define HARDWARE_VERSION "0.1.0"
#define FIRMWARE_VERSION "0.1.0"

// ====== DEBUG ======
#define ENABLE_LOGGING true
#define SERIAL_BAUD_RATE 115200
#define SERIAL_INIT_DELAY 500

// ====== GLOBALS ======
Logger *logger;

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

    logger->info("Startup Beginning...");

    logger->info("Startup Complete!");
}

void loop()
{
}
