#include "Arduino.h"

#include "./logger.h"

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
Logger::Logger(Serial_ *serial, bool enabled)
#else
Logger::Logger(HardwareSerial *serial, bool enabled)
#endif
{
    this->serial = serial;
    this->enabled = enabled;
}

void Logger::begin(String device_name, String hardware_version, String firmware_version)
{
    this->print("============");
    this->print(device_name);
    this->print("Hardware Ver: " + hardware_version);
    this->print("Firmware Ver: " + firmware_version);
    this->print("============\n");
}

void Logger::enable(bool enabled)
{
    this->enabled = enabled;
}

void Logger::disable()
{
    this->enable(false);
}

bool Logger::getEnabled()
{
    return this->enabled;
}

void Logger::print(String message)
{
    // Skip if debugging is disabled
    if (!this->enabled)
    {
        return;
    }

    // Add new line character
    message += '\n';

    // Print to serial connection
    this->serial->print(message.c_str());
}

void Logger::debug(String message)
{
    this->print("DEBUG (" + String(millis()) + "): " + message);
}

void Logger::info(String message)
{
    this->print("INFO (" + String(millis()) + "): " + message);
}

void Logger::warning(String message)
{
    this->print("WARN (" + String(millis()) + "): " + message);
}

void Logger::error(String message)
{
    this->print("ERROR (" + String(millis()) + "): " + message);
}
