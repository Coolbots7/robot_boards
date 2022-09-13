#ifndef _ROBOT_BOARDS_MAIN_BOARD_SBUS_RECEIVER_H_
#define _ROBOT_BOARDS_MAIN_BOARD_SBUS_RECEIVER_H_

#include "./sbus.h"

/**
 * @brief Structure to hold SBUS frame data
 */
struct SbusData
{
    float channels[SBUS_CHANNEL_COUNT];
    bool channel_17;
    bool channel_18;
    bool lost_frame;
    bool failsafe;
    float rssi;
    uint32_t timestamp;
};

/**
 * SbusReceiver
 *
 * Wrapper for an SBUS receiver device
 */
class SbusReceiver
{
private:
    // SBUS receiver instance
    SbusRx *sbus_rx;

    // Most recent data from the SBUS receiver
    SbusData sbus_data;

    // Counter for number of frames lost by the SBUS receiver
    uint32_t lost_frames;

    /**
     * @brief Helper to normalize SBUS channel values to a useable value
     *
     * @param raw Raw SBUS channel value
     * @param extended Optional flag for an extended SBUS channel value range
     * @return float Normalized value
     */
    float normalizeChannel(int16_t raw, bool extended = false);

    /**
     * @brief Helper for mapping floating point value between ranges
     *
     * @param raw Input floating point value
     * @param from_low Lower limit of the starting range
     * @param from_high Upper limit of the starting range
     * @param to_low Lower limit of the ending range
     * @param to_high Upper limit of the ending range
     * @return float Mapped value
     */
    static float fmap(float raw, float from_low, float from_high, float to_low, float to_high);

public:
    /**
     * @brief Construct a new SBUSReceiver object
     */
    SbusReceiver();

    /**
     * @brief Initialize the SBUSReceiver
     *
     * @param serial Hardware serial communication bus for the SBUS transceiver
     * @param rx_pin Pin connected to the SBUS transceivers transmit pin
     * @param tx_pin Pin connected to the SBUS transceivers receive pin
     */
    void begin(HardwareSerial *serial, uint8_t rx_pin, uint8_t tx_pin);

    /**
     * @brief Read the most recent data from the SBUS receiver
     *
     * @param out Data read from the SBUS receiver if the read was successful
     * @return true Reading the most recent data from the SBUS receiver was successful
     * @return false Reading the most recent data from the SBUS receiver failed
     */
    bool read(SbusData &out);

    /**
     * @brief Get the most recent SBUS data
     *
     * @return SbusData
     */
    SbusData getData();

    /**
     * @brief Get the number of frames lost by the receiver
     *
     * @return uint32_t Total frames lost by the receiver
     */
    uint32_t getLostFrames();

    /**
     * @brief Get the most recent Failsafe state from the receiver
     *
     * @return true The receiver is in Failsafe mode
     * @return false The receiver is not in Failsafe mode
     */
    bool getFailsafe();

    /**
     * @brief Get the most recent Received Signal Strength Indicator (RSSI) from the receiver
     *
     * @return float Most recent received signal strength
     */
    float getRSSI();
};

#endif
