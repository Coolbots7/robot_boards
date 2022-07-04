
#define CELL_CHARGED_VOLTAGE_EEPROM_ADDR 0  // 0-3
#define CELL_NOMINAL_VOLTAGE_EEPROM_ADDR 4  // 4-7
#define CELL_CRITICAL_VOLTAGE_EEPROM_ADDR 8 // 8-11

// EEPROM addresses of BMS configuration values
#define CELL_CHARGED_VOLTAGE_EEPROM_ADDR 0     // 0-3
#define CELL_NOMINAL_VOLTAGE_EEPROM_ADDR 4     // 4-7
#define CELL_CRITICAL_VOLTAGE_EEPROM_ADDR 8    // 8-11
#define TEMP_RESOLUTION_EEPROM_ADDR 12         // 12
#define TEMP_OVERHEAT_WARNING_EEPROM_ADDR 13   // 13-16
#define TEMP_OVERHEAT_CRITICAL_EEPROM_ADDR 17  // 17-20
#define TEMP_UNDERHEAT_WARNING_EEPROM_ADDR 21  // 21-24
#define TEMP_UNDERHEAT_CRITICAL_EEPROM_ADDR 25 // 25-28
#define CURRENT_WARNING_EERPOM_ADDR 29         // 29-32
#define CURRENT_CRITICAL_EEPROM_ADDR 33        // 33-36
#define LED_BRIGHTNESS_EEPROM_ADDR 37          // 37
#define REFRESH_RATE_EEPROM_ADDR 38            // 38

void setCellChargedVoltageEEPROM(float voltage)
{
    EEPROM.put(CELL_CHARGED_VOLTAGE_EEPROM_ADDR, voltage);
    cellChargedVoltage = voltage;
}
float getCellChargedVoltageEEPROM()
{
    float voltage = 0.00f;
    EEPROM.get(CELL_CHARGED_VOLTAGE_EEPROM_ADDR, voltage);
    return voltage;
}

void setCellNominalVoltageEEPROM(float voltage)
{
    EEPROM.put(CELL_NOMINAL_VOLTAGE_EEPROM_ADDR, voltage);
    cellNominalVoltage = voltage;
}
float getCellNominalVoltageEEPROM()
{
    float voltage = 0.00f;
    EEPROM.get(CELL_NOMINAL_VOLTAGE_EEPROM_ADDR, voltage);
    return voltage;
}

void setCellCriticalVoltageEEPROM(float voltage)
{
    EEPROM.put(CELL_CRITICAL_VOLTAGE_EEPROM_ADDR, voltage);
    cellCriticalVoltage = voltage;
}
float getCellCriticalVoltageEEPROM()
{
    float voltage = 0.00f;
    EEPROM.get(CELL_CRITICAL_VOLTAGE_EEPROM_ADDR, voltage);
    return voltage;
}

// ============ EEPROM Setters & Getters ============

// Cell charged voltage
void setCellChargedVoltageEEPROM(float voltage)
{
    EEPROM.put(CELL_CHARGED_VOLTAGE_EEPROM_ADDR, voltage);
}
float getCellChargedVoltageEEPROM()
{
    float voltage = 0.00f;
    EEPROM.get(CELL_CHARGED_VOLTAGE_EEPROM_ADDR, voltage);
    return voltage;
}

// Cell nominal voltage
void setCellNominalVoltageEEPROM(float voltage)
{
    EEPROM.put(CELL_NOMINAL_VOLTAGE_EEPROM_ADDR, voltage);
}
float getCellNominalVoltageEEPROM()
{
    float voltage = 0.00f;
    EEPROM.get(CELL_NOMINAL_VOLTAGE_EEPROM_ADDR, voltage);
    return voltage;
}

// Cell critical voltage
void setCellCriticalVoltageEEPROM(float voltage)
{
    EEPROM.put(CELL_CRITICAL_VOLTAGE_EEPROM_ADDR, voltage);
}
float getCellCriticalVoltageEEPROM()
{
    float voltage = 0.00f;
    EEPROM.get(CELL_CRITICAL_VOLTAGE_EEPROM_ADDR, voltage);
    return voltage;
}

// Temperature resolution
void setTempResolutionEEPROM(uint8_t resolution)
{
    EEPROM.put(TEMP_RESOLUTION_EEPROM_ADDR, resolution);
}
uint8_t getTempResolutionEEPROM()
{
    uint8_t resolution = 0;
    EEPROM.get(TEMP_RESOLUTION_EEPROM_ADDR, resolution);
    return resolution;
}

// Temperature overheat critical
void setTempOverheatCriticalEEPROM(float threshold)
{
    EEPROM.put(TEMP_OVERHEAT_CRITICAL_EEPROM_ADDR, threshold);
}
float getTempOverheatCriticalEEPROM()
{
    float threshold = 0.0f;
    EEPROM.get(TEMP_OVERHEAT_CRITICAL_EEPROM_ADDR, threshold);
    return threshold;
}

// Temperature overheat warning
void setTempOverheatWarningEEPROM(float threshold)
{
    EEPROM.put(TEMP_OVERHEAT_WARNING_EEPROM_ADDR, threshold);
}
float getTempOverheatWarningEEPROM()
{
    float threshold = 0.0f;
    EEPROM.get(TEMP_OVERHEAT_WARNING_EEPROM_ADDR, threshold);
    return threshold;
}

// Temperature underheat critical
void setTempUnderheatCriticalEEPROM(float threshold)
{
    EEPROM.put(TEMP_UNDERHEAT_CRITICAL_EEPROM_ADDR, threshold);
}
float getTempUnderheatCriticalEEPROM()
{
    float threshold = 0.0f;
    EEPROM.get(TEMP_UNDERHEAT_CRITICAL_EEPROM_ADDR, threshold);
    return threshold;
}

// Temperature underheat warning
void setTempUnderheatWarningEEPROM(float threshold)
{
    EEPROM.put(TEMP_UNDERHEAT_WARNING_EEPROM_ADDR, threshold);
}
float getTempUnderheatWarningEEPROM()
{
    float threshold = 0.0f;
    EEPROM.get(TEMP_UNDERHEAT_WARNING_EEPROM_ADDR, threshold);
    return threshold;
}

// Current warning
void setCurrentWarningEEPROM(float threshold)
{
    EEPROM.put(CURRENT_WARNING_EERPOM_ADDR, threshold);
}
float getCurrentWarningEEPROM()
{
    float threshold = 0.0f;
    EEPROM.get(CURRENT_WARNING_EERPOM_ADDR, threshold);
    return threshold;
}

// Current critical
void setCurrentCriticalEEPROM(float threshold)
{
    EEPROM.put(CURRENT_CRITICAL_EEPROM_ADDR, threshold);
}
float getCurrentCriticalEEPROM()
{
    float threshold = 0.0f;
    EEPROM.get(CURRENT_CRITICAL_EEPROM_ADDR, threshold);
    return threshold;
}

// LED brightness
void setLEDBrightnessEEPROM(uint8_t brightness)
{
    EEPROM.put(LED_BRIGHTNESS_EEPROM_ADDR, brightness);
}
uint8_t getLEDBrightnessEEPROM()
{
    uint8_t brightness = 0;
    EEPROM.get(LED_BRIGHTNESS_EEPROM_ADDR, brightness);
    return brightness;
}

// Refresh rate
void setRefreshRateEEPROM(uint8_t rate)
{
    EEPROM.put(REFRESH_RATE_EEPROM_ADDR, rate);
}
uint8_t getRefreshRateEEPROM()
{
    uint8_t rate = 0;
    EEPROM.get(REFRESH_RATE_EEPROM_ADDR, rate);
    return rate;
}