#ifndef ROBOT_BOARDS_BMS_BOARD_H
#define ROBOT_BOARDS_BMS_BOARD_H

#include "./Board.h"

/**
 *
 */
class BMSBoard : public Board
{
public:
    enum Registers
    {
        // cell voltages
        CELL_0_VOLTAGE = 0x00,
        CELL_1_VOLTAGE = 0x01,
        CELL_2_VOLTAGE = 0x02,
        CELL_3_VOLTAGE = 0x03,

        // battery
        BATTERY_VOLTAGE = 0x10,
        BATTERY_CURRENT = 0x11,
        BATTERY_POWER = 0x12,

        // TODO temp

        // enum MessageType{
        //     // Constants to represent the intent of the value being sent to the battery manager
        //     CELL_CHARGED_VOLTAGE = 0x01,
        //     CELL_NOMINAL_VOLTAGE = 0x02,
        //     CELL_CRITICAL_VOLTAGE = 0x03,
        //     TEMPERATURE_RESOLUTION = 0x04,
        //     TEMPERATURE_OVERHEAT_WARNING = 0x05,
        //     TEMPERATURE_OVERHEAT_CRITICAL = 0x06,
        //     TEMPERATURE_UNDERHEAT_WARNING = 0x07,
        //     TEMPERATURE_UNDERHEAT_CRITICAL = 0x08,
        //     CURRENT_WARNING = 0x09,
        //     CURRENT_CRITICAL = 0x0A,
        //     REQUEST_TYPE = 0x0B,
        //     LED_BRIGHTNESS = 0x0C,
        //     REFRESH_RATE = 0x0D,
        //     // Constants to represent the intended value being requested from the battery manager
        //     CELL_0_VOLTAGE = 0x81,
        //     CELL_1_VOLTAGE = 0x82,
        //     CELL_2_VOLTAGE = 0x83,
        //     CELL_3_VOLTAGE = 0x84,
        //     BATTERY_VOLTAGE = 0x85,
        //     BATTERY_CURRENT = 0x86,
        //     BATTERY_POWER = 0x87,
        //     BATTERY_TEMPERATURE = 0x88,
        //     BATTERY_ERROR = 0x89,
        //     BATTERY_STATUS = 0x8A,
        //     CELL_COUNT = 0x8B};
    };

    BMSBoard(uint8_t addr) : Board(addr)
    {
    }

    static void adoptBoards()
    {
        // TODO search for BMS boards in expected address range
    }

    float getCellVoltage(uint8_t cellIdx)
    {
        switch (cellIdx)
        {
        case 0:
            return this->readFloat(BMSBoard::CELL_0_VOLTAGE);
        case 1:
            return this->readFloat(BMSBoard::CELL_1_VOLTAGE);
        case 2:
            return this->readFloat(BMSBoard::CELL_2_VOLTAGE);
        case 3:
            return this->readFloat(BMSBoard::CELL_3_VOLTAGE);
        default:
            return -1.0;
        }
    }

    float getBatteryVoltage()
    {
        return this->readFloat(BMSBoard::BATTERY_VOLTAGE);
    }

    float getBatteryCurrent()
    {
        return this->readFloat(BMSBoard::BATTERY_CURRENT);
    }

    float getBatteryPower()
    {
        return this->readFloat(BMSBoard::BATTERY_POWER);
    }
};

#endif