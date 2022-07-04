#include "./RB_Debug.h"

/**
 *
 */
Debug::Debug(String deviceName, bool enabled = false)
{
    this->enabled = enabled;

    this->info("============\n");
    this->info(deviceName + "\n");
    // TODO hardware version
    this->info("Hardware Ver: \n");
    // TODO firmware version
    this->info("Firmware Ver: \n");
    this->info("============\n\n");
}

void Debug::enable(bool enabled = true)
{
    this->enabled = enabled;
}

void Debug::disable()
{
    this->enable(false);
}

bool Debug::getEnabled()
{
    return this->enabled;
}

void Debug::print(String message)
{
    // Skip if debugging is disabled
    if (!this->enabled)
    {
        return;
    }

    Serial.print(message.c_str());
}

void Debug::info(String message)
{
    this->print("INFO: " + message);
}

void Debug::debug(String message)
{
    this->print("DEBUG: " + message);
}

void Debug::warning(String message)
{
    this->print("WARN: " + message);
}

void Debug::error(String message)
{
    this->print("ERROR: " + message);
}

/**
 *
 */
void Debuggable::addDebugger(Debug *d)
{
    this->debugger = d;
}

void Debuggable::print(String message)
{
    if (this->debugger)
    {
        this->debugger->print(message);
    }
}

void Debuggable::info(String message)
{
    if (this->debugger)
    {
        this->debugger->info(message);
    }
}

void Debuggable::debug(String message)
{
    if (this->debugger)
    {
        this->debugger->debug(message);
    }
}

void Debuggable::warning(String message)
{
    if (this->debugger)
    {
        this->debugger->warning(message);
    }
}

void Debuggable::error(String message)
{
    if (this->debugger)
    {
        this->debugger->error(message);
    }
}