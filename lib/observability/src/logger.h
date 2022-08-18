#ifndef ROBOT_BOARDS_LOGGER_H
#define ROBOT_BOARDS_LOGGER_H

#include "Arduino.h"

/**
 * Logger
 *
 * The purpose of this class is to log messages to a serial output.
 */
class Logger
{
private:
  // Serial device to use for logging
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  Serial_ *serial;
#else
  HardwareSerial *serial;
#endif

  // Enabled state of the logger
  bool enabled;

  /**
   * @brief Print messages to serial while enforcing all debug logic
   *
   * @param message String to print to serial
   */
  void print(String message);

public:
  /**
   * @brief Construct a new Logger object
   *
   * @param enabled Optional initial enabled state
   */
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  Logger(Serial_ *serial, bool enabled = true);
#else
  Logger(HardwareSerial *serial, bool enabled = true);
#endif

  /**
   * @brief Enable or disable all logging
   *
   * @param enabled Optional enabled state
   */
  void enable(bool enabled = true);

  /**
   * @brief Disable all logging
   */
  void disable();

  /**
   * @brief Get the enabled state of the logger
   *
   * @return true Logging is enabled
   * @return false Logging is disabled
   */
  bool getEnabled();

  /**
   * @brief Initialize the logger utility
   *
   * @param device_name Name of the board
   * @param hardware_version Current hardware version
   * @param firmware_version Current firmware version
   */
  void begin(String device_name, String hardware_version, String firmware_version);

  /**
   * @brief Log a message at the 'debug' severity level
   *
   * @param message Log message to send
   */
  void debug(String message);

  /**
   * @brief Log a message at the 'info' severity level
   *
   * @param message Log message to send
   */
  void info(String message);

  /**
   * @brief Log a message at the 'warning' severity level
   *
   * @param message Log message to send
   */
  void warning(String message);

  /**
   * @brief Log a message at the 'error' severity level
   *
   * @param message Log message to send
   */
  void error(String message);
};

#endif
