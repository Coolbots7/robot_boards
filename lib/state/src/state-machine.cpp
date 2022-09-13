#include "logger.h"

#include "./state-machine.h"

StateMachine::StateMachine(void (*state_update_handler)(StateMachine *), Logger *logger)
{
    logger->info("Initializing state machine...");

    // Initialize data members
    this->logger = logger;
    this->state_update_handler = state_update_handler;

    // Initialize state
    this->previous_state = State::NONE;
    this->current_state = State::STARTING;

    this->logger->info("State machine initialized with state '" + StateMachine::getStateDescription(this->current_state) + "'");
}

void StateMachine::update()
{
    // Call provided state update handler
    this->state_update_handler(this);
}

State StateMachine::transitionTo(State desired_state)
{
    // If the desired state is the same as the current state, skip
    if (desired_state == this->current_state)
    {
        return this->current_state;
    }

    State next_state = State::NONE;

    // STARTING => IDLE
    // STARTING => FAULT
    if (this->current_state == State::STARTING)
    {
        switch (desired_state)
        {
        case State::IDLE:
        case State::FAULT:
            next_state = desired_state;
            break;
        default:
            break;
        }
    }
    // IDLE => RUNNING
    // IDLE => HALT
    // IDLE => FAULT
    else if (this->current_state == State::IDLE)
    {
        switch (desired_state)
        {
        case State::RUNNING:
        case State::HALT:
        case State::FAULT:
            next_state = desired_state;
            break;
        default:
            break;
        }
    }
    // RUNNING => IDLE
    // RUNNING => HALT
    // RUNNING => FAULT
    else if (this->current_state == State::RUNNING)
    {
        switch (desired_state)
        {
        case State::IDLE:
        case State::HALT:
        case State::FAULT:
            next_state = desired_state;
            break;
        default:
            break;
        }
    }
    // HALT => IDLE
    // HALT => RUNNING
    // HALT => FAULT
    else if (this->current_state == State::HALT)
    {
        switch (desired_state)
        {
        case State::IDLE:
        case State::RUNNING:
        case State::FAULT:
            next_state = desired_state;
            break;
        default:
            break;
        }
    }
    // FAULT => STARTING
    else if (this->current_state == State::FAULT)
    {
        switch (desired_state)
        {
        case State::STARTING:
            next_state = desired_state;
            break;
        default:
            break;
        }
    }

    // If the transition was valid, perform the transition
    if (next_state != State::NONE)
    {
        this->previous_state = this->current_state;
        this->current_state = next_state;
        this->logger->debug("Transitioned state from " + StateMachine::getStateDescription(this->previous_state) + " to " + StateMachine::getStateDescription(this->current_state));
    }
    else
    {
        // Log if the transition was not valid
        this->logger->error("Failed to transition state from " + StateMachine::getStateDescription(this->current_state) + " to " + StateMachine::getStateDescription(desired_state));
        // Transition to fault state
        return this->transitionTo(State::FAULT);
    }

    return this->current_state;
}

State StateMachine::getPreviousState()
{
    return this->previous_state;
}

State StateMachine::getCurrentState()
{
    return this->current_state;
}

String StateMachine::getStateDescription(State state)
{
    switch (state)
    {
    case State::NONE:
        return "NONE";
    case State::STARTING:
        return "Starting";
    case State::IDLE:
        return "Idle";
    case State::RUNNING:
        return "Running";
    case State::HALT:
        return "Halt";
    case State::FAULT:
        return "Fault";
    default:
        return "Unknown";
    };
}
