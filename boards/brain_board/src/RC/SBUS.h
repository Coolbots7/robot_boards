#include <sbus.h>

#ifndef SBUS_H
#define SBUS_H

struct SBUSData
{
  float channels[bfs::SbusRx::NUM_CH()];
  bool channel17;
  bool channel18;
  bool lost_frame;
  bool failsafe;
  float rssi;
  uint32_t timestamp;
};

class SBUSReceiver
{
private:
  bfs::SbusRx *sbusRx;

  SBUSData sbusData;
  uint32_t lostFrames;

  float normalizeChannel(int16_t raw, bool extended = false)
  {
    if (extended)
    {
      return fmap(raw, 0.0f, 2048.0f, -1.0f, 1.0f);
    }
    else
    {
      return fmap(raw, 172.0f, 1811.0f, -1.0f, 1.0f);
    }
  }

  static float fmap(float raw, float fromLow, float fromHigh, float toLow, float toHigh)
  {
    return (raw - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
  }

public:
  SBUSReceiver()
  {
    this->sbusRx = new bfs::SbusRx(&Serial2);

    this->sbusData = {};
    this->sbusData.failsafe = true;
    this->sbusData.rssi = 0.0f;

    this->lostFrames = 0;
  }

  void begin(uint8_t rxPin, uint8_t txPin)
  {
    this->sbusRx->Begin(rxPin, txPin);
  }

  bool read(SBUSData &out)
  {
    if (this->sbusRx->Read())
    {
      SBUSData data = {};

      // normalize channels
      std::array<int16_t, bfs::SbusRx::NUM_CH()> rawChannels = this->sbusRx->ch();
      data.channels[0] = normalizeChannel(rawChannels[0]);
      data.channels[1] = normalizeChannel(rawChannels[1]);
      data.channels[2] = normalizeChannel(rawChannels[2]);
      data.channels[3] = normalizeChannel(rawChannels[3]);
      data.channels[4] = normalizeChannel(rawChannels[4]);
      data.channels[5] = normalizeChannel(rawChannels[5]);
      data.channels[6] = normalizeChannel(rawChannels[6]);
      data.channels[7] = normalizeChannel(rawChannels[7]);
      data.channels[8] = normalizeChannel(rawChannels[8]);
      data.channels[9] = normalizeChannel(rawChannels[9]);
      data.channels[10] = normalizeChannel(rawChannels[10]);
      data.channels[11] = normalizeChannel(rawChannels[11]);
      data.channels[12] = normalizeChannel(rawChannels[12]);
      data.channels[13] = normalizeChannel(rawChannels[13]);
      data.channels[14] = normalizeChannel(rawChannels[14]);
      data.channels[15] = normalizeChannel(rawChannels[15]);

      // get RSSI from channel 8 / 16
      data.rssi = data.channels[15];

      // ch17
      data.channel17 = this->sbusRx->ch17();

      // ch18
      data.channel18 = this->sbusRx->ch18();

      // lost frame
      data.lost_frame = this->sbusRx->lost_frame();

      if (data.lost_frame)
      {
        this->lostFrames++;
      }

      // failsafe
      data.failsafe = this->sbusRx->failsafe();

      data.timestamp = millis();

      this->sbusData = data;

      // Return most recent data
      out = this->sbusData;
      return true;
    }

    return false;
  }

  SBUSData getData()
  {
    return this->sbusData;
  }

  uint32_t getLostFrames()
  {
    return this->lostFrames;
  }

  float getRSSI()
  {
    return this->sbusData.rssi;
  }

  bool getFailsafe()
  {
    return this->sbusData.failsafe;
  }
};

#endif
