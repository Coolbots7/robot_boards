#ifndef ROBOT_BOARDS_STATE_MACHINE_H
#define ROBOT_BOARDS_STATE_MACHINE_H

#include "Arduino.h"

#include "./logger.h"

// States of the State Machine
enum State
{
    NONE = -1,
    STARTING = 0x00,
    IDLE = 0x01,
    RUNNING = 0x02,
    HALT = 0x04,
    FAULT = 0xFF
};

/**
 * State Machine
 *
 * State Machine object to track state of the board and enforce transition rules
 */
class StateMachine
{
private:
    // Logger instance for the State Machine to use for logging
    Logger *logger;

    // Provided update handler for the State Machine
    void (*state_update_handler)(StateMachine *);

    // The previous state of the State Machine
    State previous_state;

    // The current state of the state machine
    State current_state;

public:
    /**
     * @brief Construct a new System State object
     *
     * @param state_update_handler Update handler for the State Machine to use
     * @param logger Logger instance to use for logging
     */
    StateMachine(void (*state_update_handler)(StateMachine *), Logger *logger);

    /**
     * @brief Update the state of the State Machine using the latest inputs
     */
    void update();

    /**
     * @brief Enforce state machine transition rules
     *
     * @param desired_state SystemState desired to transition the state machine to
     * @return SystemState The resulting state of the transition
     */
    State transitionTo(State desired_state);

    /**
     * @brief Get the previous state of the state machine
     *
     * @return SystemState
     */
    State getPreviousState();

    /**
     * @brief Get the current state of the state machine
     *
     * @return SystemState
     */
    State getCurrentState();

    /**
     * @brief Get String of a SystemState
     *
     * @param state SystemState
     * @return String
     */
    static String getStateDescription(State state);
};

#endif
