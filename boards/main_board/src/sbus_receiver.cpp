#include "./sbus.h"
#include "./sbus_receiver.h"

SbusReceiver::SbusReceiver()
{
    this->sbus_data = {};
    this->sbus_data.failsafe = true;
    this->sbus_data.rssi = 0.0f;

    this->lost_frames = 0;
}

float SbusReceiver::normalizeChannel(int16_t raw, bool extended)
{
    if (extended)
    {
        return SbusReceiver::fmap(raw, 0.0f, 2048.0f, -1.0f, 1.0f);
    }
    else
    {
        return SbusReceiver::fmap(raw, 172.0f, 1811.0f, -1.0f, 1.0f);
    }
}

float SbusReceiver::fmap(float raw, float from_low, float from_high, float to_low, float to_high)
{
    return (raw - from_low) * (to_high - to_low) / (from_high - from_low) + to_low;
}

void SbusReceiver::begin(HardwareSerial *serial, uint8_t rx_pin, uint8_t tx_pin)
{
    // Initialize the SBUS interface
    this->sbus_rx = new SbusRx(serial);
    this->sbus_rx->begin(rx_pin, tx_pin, true);
}

bool SbusReceiver::read(SbusData &out)
{
    // Attempt to read data from SBUS transceiver
    if (this->sbus_rx->parse())
    {
        SbusData data = {};
        data.timestamp = millis();

        // Get raw channel data from receiver
        std::array<int16_t, SBUS_CHANNEL_COUNT> rawChannels = this->sbus_rx->getChannels();

        // Normalize channels
        for (uint8_t i = 0; i < SBUS_CHANNEL_COUNT; i++)
        {
            data.channels[i] = SbusReceiver::normalizeChannel(rawChannels[i]);
        }

        // Get RSSI from channel 16
        data.rssi = data.channels[15];

        // Get channel 17 data from receiver
        data.channel_17 = this->sbus_rx->getChannel17();

        // Get channel 18 data from receiver
        data.channel_18 = this->sbus_rx->getChannel18();

        // Get lost frame from receiver
        data.lost_frame = this->sbus_rx->getLostFrame();

        // Increment lost frame counter if frame was lost
        if (data.lost_frame)
        {
            this->lost_frames++;
        }

        // Get failsafe from receiver
        data.failsafe = this->sbus_rx->getFailsafe();

        // Pass most recent data
        this->sbus_data = data;
        out = this->sbus_data;

        return true;
    }

    return false;
}

SbusData SbusReceiver::getData()
{
    return this->sbus_data;
}

uint32_t SbusReceiver::getLostFrames()
{
    return this->lost_frames;
}

float SbusReceiver::getRSSI()
{
    return this->sbus_data.rssi;
}

bool SbusReceiver::getFailsafe()
{
    return this->sbus_data.failsafe;
}
