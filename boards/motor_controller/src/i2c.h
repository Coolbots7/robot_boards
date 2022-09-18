#ifndef _ROBOT_BOARDS_MOTOR_CONTROLLER_I2C_H_
#define _ROBOT_BOARDS_MOTOR_CONTROLLER_I2C_H_

#include "motor_controller/motor_controller.h"
#include "state-machine.h"
#include "twowire/device.h"

#include "./dc_motor.h"

/**
 * @brief Helper for the Motor Controller board to be communicated with over the TwoWire protocol
 */
class MotorControllerI2CSlave : public I2CDevice
{
private:
    // Instance of the DCMotor for motor 1
    DCMotor *motor_1;

    // Instance of the DCMotor for motor 2
    DCMotor *motor_2;

public:
    /**
     * @brief Construct a new Motor Controller I2C Slave object
     *
     * @param address_offset Address offset for the Status Board from the default address
     * @param state Instance of the State Machine for the board
     */
    MotorControllerI2CSlave(uint8_t address_offset, StateMachine *state, DCMotor *motor_1, DCMotor *motor_2);

    /**
     * @brief Function to process TwoWire on receive events for the Motor Controller board
     *
     * @param num_bytes Number of bytes received
     */
    void onReceive(int num_bytes);

    /**
     * @brief Function to process TwoWire on request events for the Motor Controller board
     */
    void onRequest();
};

#endif
