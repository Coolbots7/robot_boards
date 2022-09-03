#ifndef _ROBOT_BOARDS_MOTOR_CONTROLLER_I2C_H_
#define _ROBOT_BOARDS_MOTOR_CONTROLLER_I2C_H_

#include "motor_controller/motor_controller.h"
#include "state-machine.h"
#include "twowire/device.h"

/**
 * @brief Helper for the Motor Controller board to be communicated with over the TwoWire protocol
 */
class MotorControllerI2CSlave : public I2CDevice
{
public:
    /**
     * @brief Construct a new Motor Controller I2C Slave object
     *
     * @param address_offset Address offset for the Status Board from the default address
     * @param state Instance of the State Machine for the board
     */
    MotorControllerI2CSlave(uint8_t address_offset, StateMachine *state) : I2CDevice(I2C_MOTOR_CONTROLLER_BASE_ADDRESS + address_offset, state) {}
};

#endif
