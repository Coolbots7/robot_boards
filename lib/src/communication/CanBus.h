#ifndef _ROBOT_BOARDS_CANBUS_H_
#define _ROBOT_BOARDS_CANBUS_H_

#include "Arduino.h"
#include <SPI.h>

#define CAN_ADDRESS_BITS 12
#define CAN_REGISTER_BITS 17

// xxx|0 0000 - 0000 0000 - 0000 1111 - 1111 1111
#define CAN_ADDRESS_MASK 0x00000FFF
// xxx|1 1111 - 1111 1111 - 1111 0000 - 0000 0000
#define CAN_REGISTER_MASK 0x1FFFF000

// REGISTERS
// - Registers with smaller values are given higher priority
// - Registers use the CAN_ADDRESS_BITS LSB of the 29 identifier bits [0x00000 - 0x1FFFF]

// 0x00000: Broadcast
// 0x00001 - 0x0000F: Reserved
// 0x00010: Heartbeat
#define CANBUS_REGISTER_HEARTBEAT 0x10
// 0x00011 - 0x00FFF: Reserved

// 0x01000 - 0x01FFF: Faults
// TODO

// 0x02000 - 0x02FFF: Alerts
// TODO

// 0x10000 - 0x10FFF: Reserved

// === 0x11000 - 0x1FFFF: Data

// = 0x00000 - 0x00000: Controller
// TODO

// = 0x00000 - 0x00000: Safety
// 0x00000 - 0x00000: Status Light
// TODO
// 0x00000 - 0x00000: EStop
// TODO

// = 0x00000 - 0x00000: Power
// 0x00000 - 0x00000: BMS
// TODO
// 0x00000 - 0x00000: PRU
// TODO
// 0x00000 - 0x00000: Power Monitor
// TODO
// #define CANBUS_POWER_MONITOR_VOLTAGE
// #define CANBUS_POWER_MONITOR_CURRENT
// #define CANBUS_POWER_MONITOR_POWER

// ADDRESSES
// - Addresses with smaller values are given higher priority
// - Addresses use the CAN_REGISTER_BITS MSB of the 29 identifier bits [0x000 - 0xFFF]

// 0x000: Broadcast
// 0x001 - 0x0FF: Reserved

// 0x100 - 0x1FF: Controller
#define CANBUS_BASE_ADDRESS_CONTROLLER 0x100
#define CANBUS_ADDRESS_RANGE_CONTROLLER 0xFF

// = 0x200 - 0x2FF: Safety
// 0x200 - 0x20F: Reserved
// 0x210 - 0x21F: Status Light
#define CANBUS_BASE_ADDRESS_STATUS_LIGHT 0x210
#define CANBUS_ADDRESS_RANGE_STATUS_LIGHT 0xF
// 0x220 - 0x22F: E-Stop
#define CANBUS_BASE_ADDRESS_ESTOP 0x220
#define CANBUS_ADDRESS_RANGE_ESTOP 0xF

// = 0x300 - 0x3FF: Power
// 0x300 - 0x30F: Reserved
// 0x310 - 0x31F: Battery Management System (BMS)
#define CANBUS_BASE_ADDRESS_BMS 0x310
#define CANBUS_ADDRESS_RANGE_BMS 0xF
// 0x320 - 0x32F: Power Regulation Unit (PRU)
#define CANBUS_BASE_ADDRESS_PRU 0x320
#define CANBUS_ADDRESS_RANGE_PRU 0xF
// 0x330 - 0x33F: Power Monitor
#define CANBUS_BASE_ADDRESS_POWER_MONITOR 0x330
#define CANBUS_ADDRESS_RANGE_POWER_MONITOR 0xF

// 0x400 - 0x04FF: Motor Controller
#define CANBUS_BASE_ADDRESS_MOTOR_CONTROLLER 0x400
#define CANBUS_ADDRESS_RANGE_POWER_MONITOR 0xFF

/**
 * @brief Helper methods for CanBus communicaiton
 *
 */
class CanBus
{
public:
    static uint32_t getID(uint32_t addr, uint16_t reg);

    static void writeFrame(uint16_t addr, uint32_t reg, uint8_t *data, uint8_t n);
};

/**
 * @brief Wrapper for boards to communicate over CanBus
 *
 * - Respond to remote frames
 * - Publish messages
 *
 */
class CanBusDevice : public CommunicationDevice
{
public:
    CanBusDevice(uint16_t base_address, uint8_t address_offset) : base(base_address, address_offset){};
};

/**
 * @brief Wrapper to communicate with boards over CanBus
 *
 * - Set registers on a board
 * - Read registers on a board
 * - Listen for published messages
 *
 */
class CanBusController : public CommunicationController
{
private:
    uint8_t cs_pin;
    uint8_t interrupt_pin;

public:
    CanBusController(uint8_t cs_pin, uint8_t interrupt_pin, uint16_t base_address, uint8_t address_offset);
};

#endif