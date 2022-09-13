#ifndef _ROBOT_BOARDS_MAIN_BOARD_SBUS_H_
#define _ROBOT_BOARDS_MAIN_BOARD_SBUS_H_

#include <array>

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include "core/core.h"
#endif

#define SBUS_BAUD_RATE 100000UL
#define SBUS_MESSAGE_LENGTH 25
#define SBUS_CHANNEL_COUNT 16

#define START_BYTE 0x0F
#define END_BYTE 0x00
#define END2_BYTE 0x04

#define CHANNEL_17_MASK 0x01
#define CHANNEL_18_MASK 0x02
#define LOST_FRAME_MASK 0x04
#define FAILSAFE_MASK 0x08

// Define SBUS channel array type
typedef std::array<int16_t, SBUS_CHANNEL_COUNT> Channels;

/**
 * SbusRx
 *
 * Class to interface with a SBUS receiver
 */
class SbusRx
{
private:
        // Hardware Serial interface to use for the SBUS device
        HardwareSerial *serial;

        // SBUS current frame channel data
        Channels channels;

        // SBUS current frame failsafe state
        bool failsafe = false;

        // SBUS lost current frame
        bool lost_frame = false;

        // SBUS current frame channel 17 state
        bool channel_17 = false;

        // SBUS current frame channel 18 data
        bool channel_18 = false;

        /**
         * @brief Read a new SBUS if available
         *
         * @param message_buffer Byte array to read the message into
         * @return true New SBUS data was read
         * @return false No new SBUS data was read
         */
        bool read(uint8_t *message_buffer);

public:
        /**
         * @brief Construct a new Sbus Rx object
         *
         * @param serial Hardware Serial interface to use for the SBUS device
         */
        SbusRx(HardwareSerial *serial);

#if defined(ESP32)
        /**
         * @brief Initialize the Hardware Serial interface for SBUS communication
         *
         * @param rx_pin Hardware Serial interface receive pin
         * @param tx_pin Hardware Serial interface transmit pin
         * @param inverted Use the internal inverter for the Hardware Serial interface
         */
        void begin(const uint8_t rx_pin, const uint8_t tx_pin, bool inverted = true);
#else
        /**
         * @brief Initialize the Hardware Serial interface for SBUS communication
         */
        void begin();
#endif

        /**
         * @brief Parse
         *
         * @return true SBUS data parse was successful
         * @return false SBUS data parse failed
         */
        bool parse();

        /**
         * @brief Get the Channels data for the latest frame
         *
         * @return Channels Array of raw channel data
         */
        Channels getChannels();

        /**
         * @brief Get the Failsafe state for the latest frame
         *
         * @return bool Failsafe state
         */
        bool getFailsafe();

        /**
         * @brief Get the Lost Frame state for the latest frame
         *
         * @return bool Lost Frame state
         */
        bool getLostFrame();

        /**
         * @brief Get the Channel 17 state for the latest frame
         *
         * @return bool Channel 17 state
         */
        bool getChannel17();

        /**
         * @brief Get the Channel 18 state for the latest frame
         *
         * @return bool Channel 18 state
         */
        bool getChannel18();
};

#endif
