#include "./I2C.h"

/**
 * I2CPrim
 */

// Byte
static uint8_t I2CPrim::decodeByte(uint8_t *buff, uint8_t size)
{
    uint8_t value = buff[0];
    return value;
}

static void I2CPrim::encodeByte(uint8_t val, uint8_t *buff, uint8_t size)
{
    buff[0] = val;
}

// Bool
static bool I2CPrim::decodeBool(uint8_t *buff, uint8_t size)
{
    if ((buff[0] & 0x01) == 0x01)
    {
        return true;
    }
    return false;
}

static void I2CPrim::encodeBool(bool value, uint8_t *buff, uint8_t size)
{
    buff[0] = 0x00;
    if (value)
    {
        buff[0] = 0x01;
    }
}

// Float
static float I2CPrim::decodeFloat(uint8_t *buff)
{
    float value;
    memcpy(&value, buff, sizeof(float));
    return value;
}

static void I2CPrim::encodeFloat(float val, uint8_t *buff)
{
    memcpy(buff, &val, sizeof(float));
}

// ULong
static uint32_t I2CPrim::decodeULong(uint8_t *buff)
{
    uint32_t temp;
    memcpy(&temp, buff, sizeof(uint32_t));
    return temp;
}

static void I2CPrim::encodeULong(uint32_t val, uint8_t *buff)
{
    memcpy(buff, &val, sizeof(uint32_t));
}

// Message
static WireMessage I2CPrim::decodeMessage(uint8_t *buff)
{
    WireMessage message;
    memcpy(&message, buff, sizeof(WireMessage));
    return message;
}

static void I2CPrim::encodeMessage(WireMessage message, uint8_t *buff)
{
    memset(buff, 0, sizeof(WireMessage));
    memcpy(buff, &message, sizeof(WireMessage));
}

/**
 * I2CDevice
 */
I2CDevice::I2CDevice(uint8_t addr) : CommunicationDevice(addr, offset)
{
    Wire.begin(this->address);
}

void I2CDevice::writeBool(bool val)
{
    uint8_t buff[1];
    I2CPrim::encodeBool(val, buff, sizeof(buff));
    Wire.write(buff, sizeof(buff));
}

void I2CDevice::writeByte(uint8_t val)
{
    uint8_t buff[1];
    I2CPrim::encodeByte(val, buff, sizeof(buff));
    Wire.write(buff, sizeof(buff));
}

void I2CDevice::writeFloat(float val)
{
    uint8_t buff[4];
    I2CPrim::encodeFloat(val, buff);
    Wire.write(buff, sizeof(buff));
}

void I2CDevice::writeULong(uint32_t val)
{
    // TODO write unsigned long
}

WireMessage I2CDevice::receiveEvent(int byte_count)
{
    // TODO skip if byte count is less than wire message size

    // Read incoming bytes from master
    uint8_t received_bytes[byte_count];
    for (int i = 0; i < byte_count; i++)
    {
        received_bytes[i] = Wire.read();
    }

    // Convert raw incoming bytes to data structure
    WireMessage wire_data = I2CPrim::decodeMessage(received_bytes);
    this->firstContact = true;

    switch (wire_data.message_type)
    {
    case Registers::REGISTER_REQUEST_TYPE:
        this->request_register = I2CPrim::decodeByte(wire_data.data, sizeof(wire_data.data));
        break;
    case Registers::HEARTBEAT_STATUS:
        this->masterLastHeartbeatTime = millis();
        this->masterState = I2CPrim::decodeByte(wire_data.data, sizeof(wire_data.data));
        break;
    case Registers::HEARTBEAT_TIME:
        this->masterLastHeartbeatTime = millis();
        this->masterTime = I2CPrim::decodeULong(wire_data.data);
        break;
    }

    // switch (header.message_type)
    // {
    // case CELL_CHARGED_VOLTAGE:
    //     memcpy(&cell_charged_voltage, receive_data, sizeof(cell_charged_voltage));
    //     setCellChargedVoltageEEPROM(cell_charged_voltage);
    //     break;
    // case CELL_NOMINAL_VOLTAGE:
    //     memcpy(&cell_nominal_voltage, receive_data, sizeof(cell_nominal_voltage));
    //     setCellNominalVoltageEEPROM(cell_nominal_voltage);
    //     break;
    // case CELL_CRITICAL_VOLTAGE:
    //     memcpy(&cell_critical_voltage, receive_data, sizeof(cell_critical_voltage));
    //     setCellCriticalVoltageEEPROM(cell_critical_voltage);
    //     break;
    // case TEMPERATURE_RESOLUTION:
    //     memcpy(&temperature_resolution, receive_data, sizeof(temperature_resolution));
    //     temperature_sensor.setResolution(temperature_resolution);
    //     setTempResolutionEEPROM(temperature_resolution);
    //     break;
    // case TEMPERATURE_OVERHEAT_WARNING:
    //     memcpy(&temperature_overheat_critical, receive_data, sizeof(temperature_overheat_critical));
    //     setTempOverheatWarningEEPROM(temperature_overheat_critical);
    //     break;
    // case TEMPERATURE_OVERHEAT_CRITICAL:
    //     memcpy(&temperature_overheat_warning, receive_data, sizeof(temperature_overheat_warning));
    //     setTempOverheatCriticalEEPROM(temperature_overheat_warning);
    //     break;
    // case TEMPERATURE_UNDERHEAT_WARNING:
    //     memcpy(&temperature_underheat_warning, receive_data, sizeof(temperature_underheat_warning));
    //     setTempUnderheatWarningEEPROM(temperature_underheat_warning);
    //     break;
    // case TEMPERATURE_UNDERHEAT_CRITICAL:
    //     memcpy(&temperature_underheat_critical, receive_data, sizeof(temperature_underheat_critical));
    //     setTempUnderheatCriticalEEPROM(temperature_underheat_critical);
    //     break;
    // case CURRENT_WARNING:
    //     memcpy(&current_warning, receive_data, sizeof(current_warning));
    //     setCurrentWarningEEPROM(current_warning);
    //     break;
    // case CURRENT_CRITICAL:
    //     memcpy(&current_critical, receive_data, sizeof(current_critical));
    //     setCurrentCriticalEEPROM(current_critical);
    //     break;
    // case REQUEST_TYPE:
    //     memcpy(&request_type, receive_data, sizeof(request_type));
    //     break;
    // case LED_BRIGHTNESS:
    //     memcpy(&led_brightness, receive_data, sizeof(led_brightness));
    //     setLEDBrightnessEEPROM(led_brightness);
    //     break;
    // case REFRESH_RATE:
    //     memcpy(&refresh_rate, receive_data, sizeof(refresh_rate));
    //     setRefreshRateEEPROM(refresh_rate);
    //     break;
    // }

    return wire_data;
}

/**
 * I2CController
 */
void I2CController::writeByteMessage(uint8_t reg, uint8_t val)
{
    WireMessage message;
    message.message_type = reg;
    uint8_t buff[sizeof(message.data)];
    I2CPrim::encodeByte(val, buff, sizeof(message.data));
    memcpy(&message.data, buff, sizeof(message.data));

    this->writeMessage(message);
}

void I2CController::writeFloatMessage(uint8_t reg, float val)
{
    WireMessage message;
    message.message_type = reg;
    uint8_t buff[sizeof(message.data)];
    I2CPrim::encodeFloat(val, buff);
    memcpy(&message.data, buff, sizeof(message.data));

    this->writeMessage(message);
}

void I2CController::writeULongMessage(uint8_t reg, uint32_t val)
{
    WireMessage message;
    message.message_type = reg;
    uint8_t buff[sizeof(message.data)];
    I2CPrim::encodeULong(val, buff);
    memcpy(&message.data, buff, sizeof(message.data));

    this->writeMessage(message);
}

void I2CController::writeMessage(WireMessage message)
{
    uint8_t buff[sizeof(WireMessage)];
    I2CPrim::encodeMessage(message, buff);
    Wire.beginTransmission(this->address);
    Wire.write(buff, sizeof(WireMessage));
    Wire.endTransmission();
}

void I2CController::writeRegister(uint8_t reg)
{
    WireMessage message;
    message.message_type = Registers::REGISTER_REQUEST_TYPE;
    message.data[3] = reg;

    this->writeMessage(message);
}

bool I2CController::ping()
{
    Wire.beginTransmission(this->address);
    if (Wire.endTransmission() > 0)
    {
        return false;
    }
    return true;
}

// Byte
void I2CController::writeRegisterByte(uint8_t reg, uint8_t val)
{
    this->writeByteMessage(reg, val);
}

uint8_t I2CController::readRegisterByte(uint8_t reg)
{
    // set register on device
    this->writeRegister(reg);

    // request byte
    Wire.requestFrom(this->address, (uint8_t)1);

    // read byte
    return Wire.read();
}

// Bool
void I2CController::writeRegisterBool(uint8_t reg, bool val)
{
    uint8_t buff;
    I2CPrim::encodeBool(val, &buff, sizeof(buff));
    this->writeRegisterByte(reg, buff);
}

bool I2CController::readRegisterBool(uint8_t reg)
{
    uint8_t raw = this->readRegisterByte(reg);
    return I2CPrim::decodeBool(&raw, 1);
}

// Float
void I2CController::writeRegisterFloat(uint8_t reg, float val)
{
    this->writeFloatMessage(reg, val);
}

float I2CController::readRegisterFloat(uint8_t reg)
{
    // set register on device
    this->writeRegister(reg);

    // request float
    Wire.requestFrom(this->address, sizeof(float));

    // read float
    uint8_t buff[sizeof(float)];
    for (uint8_t i = 0; i < sizeof(buff); i++)
    {
        buff[i] = Wire.read();
    }

    // decode float
    return I2CPrim::decodeFloat(buff);
}

// Unsigned long
void I2CController::writeRegisterULong(uint8_t reg, uint32_t val)
{
    this->writeULongMessage(reg, val);
}

uint32_t I2CController::readRegisterULong(uint8_t reg)
{
    // set register on device
    this->writeRegister(reg);

    // request unsigned long
    Wire.requestFrom(this->address, sizeof(uint32_t));

    // read ulong
    uint8_t buff[sizeof(uint32_t)];
    for (uint8_t i = 0; i < sizeof(buff); i++)
    {
        buff[i] = Wire.read();
    }

    // decode ulong
    return I2CPrim::decodeULong(buff);
}