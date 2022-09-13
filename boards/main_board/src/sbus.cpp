#if defined(ARDUINO)
#include <Arduino.h>
#else
#include "core/core.h"
#endif

#include "./sbus.h"

SbusRx::SbusRx(HardwareSerial *serial)
{
    this->serial = serial;
}

#if defined(ESP32)
void SbusRx::begin(const uint8_t rx_pin, const uint8_t tx_pin, bool inverted)
{
    this->serial->begin(SBUS_BAUD_RATE, SERIAL_8E2, rx_pin, tx_pin, inverted);
#else
void SbusRx::begin()
{
    this->serial->begin(SBUS_BAUD_RATE, SERIAL_8E2);
#endif

    // flush the bus
    this->serial->flush();
}

bool SbusRx::parse()
{
    // check for a new message and get bytes
    uint8_t message_buffer[SBUS_MESSAGE_LENGTH];
    bool new_data = this->read(message_buffer);

    if (new_data)
    {
        // parse channels
        this->channels[0] = static_cast<int16_t>(message_buffer[1] | ((message_buffer[2] << 8) & 0x07FF));

        this->channels[1] = static_cast<int16_t>((message_buffer[2] >> 3) | ((message_buffer[3] << 5) & 0x07FF));

        this->channels[2] = static_cast<int16_t>((message_buffer[3] >> 6) | (message_buffer[4] << 2) | ((message_buffer[5] << 10) & 0x07FF));

        this->channels[3] = static_cast<int16_t>((message_buffer[5] >> 1) | ((message_buffer[6] << 7) & 0x07FF));

        this->channels[4] = static_cast<int16_t>((message_buffer[6] >> 4) | ((message_buffer[7] << 4) & 0x07FF));

        this->channels[5] = static_cast<int16_t>((message_buffer[7] >> 7) | (message_buffer[8] << 1) | ((message_buffer[9] << 9) & 0x07FF));

        this->channels[6] = static_cast<int16_t>((message_buffer[9] >> 2) | ((message_buffer[10] << 6) & 0x07FF));

        this->channels[7] = static_cast<int16_t>((message_buffer[10] >> 5) | ((message_buffer[11] << 3) & 0x07FF));

        this->channels[8] = static_cast<int16_t>(message_buffer[12] | ((message_buffer[13] << 8) & 0x07FF));

        this->channels[9] = static_cast<int16_t>((message_buffer[13] >> 3) | ((message_buffer[14] << 5) & 0x07FF));

        this->channels[10] = static_cast<int16_t>((message_buffer[14] >> 6) | (message_buffer[15] << 2) | ((message_buffer[16] << 10) & 0x07FF));

        this->channels[11] = static_cast<int16_t>((message_buffer[16] >> 1) | ((message_buffer[17] << 7) & 0x07FF));

        this->channels[12] = static_cast<int16_t>((message_buffer[17] >> 4) | ((message_buffer[18] << 4) & 0x07FF));

        this->channels[13] = static_cast<int16_t>((message_buffer[18] >> 7) | (message_buffer[19] << 1) | ((message_buffer[20] << 9) & 0x07FF));

        this->channels[14] = static_cast<int16_t>((message_buffer[20] >> 2) | ((message_buffer[21] << 6) & 0x07FF));

        this->channels[15] = static_cast<int16_t>((message_buffer[21] >> 5) | ((message_buffer[22] << 3) & 0x07FF));

        // parse channel 17
        this->channel_17 = message_buffer[23] & CHANNEL_17_MASK;

        // parse channel 18
        this->channel_18 = message_buffer[23] & CHANNEL_18_MASK;

        // parse lost frame
        this->lost_frame = message_buffer[23] & LOST_FRAME_MASK;

        // parse failsafe
        this->failsafe = message_buffer[23] & FAILSAFE_MASK;
    }

    return new_data;
}

bool SbusRx::read(uint8_t *message_buffer)
{
    // check for serial data available
    if (this->serial->available())
    {
        // read data into buffer until end byte or end of message length
        uint8_t message_buffer_idx = 0;
        while (this->serial->available() && message_buffer_idx < SBUS_MESSAGE_LENGTH - 1)
        {
            // read next byte from the serial interface
            uint8_t current_byte = this->serial->read();

            // read until start byte
            if (message_buffer_idx == 0)
            {
                // if the current byte is the start byte, begin reading the message
                if (current_byte == START_BYTE)
                {
                    message_buffer[message_buffer_idx++] = current_byte;
                }
            }
            // keep reading until stop byte or index is greater than message length
            else if (message_buffer_idx > 0 && message_buffer_idx <= SBUS_MESSAGE_LENGTH - 1)
            {
                message_buffer[message_buffer_idx++] = current_byte;

                // if the current byte is an end byte, stop reading the message
                if (current_byte == END_BYTE || current_byte == END2_BYTE)
                {
                    break;
                }
            }
            // return false if index is greater than message length and no stop byte
            else
            {
                return false;
            }
        }

        // return false if no more bytes are available and a full message was not read
        if (message_buffer_idx < SBUS_MESSAGE_LENGTH - 1)
        {
            return false;
        }

        // return true if a full message was read
        return true;
    }

    return false;
}

Channels SbusRx::getChannels()
{
    return this->channels;
}

bool SbusRx::getFailsafe()
{
    return this->failsafe;
}

bool SbusRx::getLostFrame()
{
    return this->lost_frame;
}

bool SbusRx::getChannel17()
{
    return this->channel_17;
}

bool SbusRx::getChannel18()
{
    return this->channel_18;
}
