#include "./State.h"
#include "../communication/I2C.h"

State::State(void (*stateHandler)(State *))
{
    this->stateLogic = stateHandler;

    this->previous_state = NONE;
    this->current_state = STARTING;
}

void State::update(I2CDevice *i2cDevice)
{
    if (i2cDevice != NULL)
    {
        if (!i2cDevice->getFirstContact())
        {
            // If the board has not yet recieved contact from the master, stay in IDLE
            this->transitionTo(IDLE);
            return;
        }

        if (i2cDevice->getFirstContact() && (millis() - i2cDevice->getMasterLastHeartbeatTime()) > HEARTBEAT_TIMEOUT)
        {
            // If the board has recieved contact but not in the last Nms, transition to HALT
            this->transitionTo(HALT);
            return;
        }

        // transition to IDLE if master is in IDLE
        if (i2cDevice->getMasterState() == IDLE)
        {
            this->transitionTo(IDLE);
            return;
        }
        // transision to HALT if master is HALT or FAULT
        else if (i2cDevice->getMasterState() == HALT || i2cDevice->getMasterState() == FAULT)
        {
            this->transitionTo(HALT);
            return;
        }
    }

    if (this->stateLogic)
    {
        this->stateLogic(this);
    }
}

SystemState State::transitionTo(SystemState desired_state)
{
    SystemState next_state = NONE;

    if (this->current_state == STARTING)
    {
        switch (desired_state)
        {
        case IDLE:
        case FAULT:
            next_state = desired_state;
            break;
        default:
            break;
        }
    }
    else if (this->current_state == IDLE)
    {
        switch (desired_state)
        {
        case RUNNING:
        case HALT:
        case FAULT:
            next_state = desired_state;
            break;
        default:
            break;
        }
    }
    else if (this->current_state == RUNNING)
    {
        switch (desired_state)
        {
        case IDLE:
        case HALT:
        case FAULT:
            next_state = desired_state;
            break;
        default:
            break;
        }
    }
    else if (this->current_state == HALT)
    {
        switch (desired_state)
        {
        case IDLE:
        case RUNNING:
        case FAULT:
            next_state = desired_state;
            break;
        default:
            break;
        }
    }
    else if (this->current_state == FAULT)
    {
        switch (desired_state)
        {
        case STARTING:
            next_state = desired_state;
            break;
        default:
            break;
        }
    }

    if (next_state > NONE)
    {
        this->previous_state = this->current_state;
        this->current_state = next_state;
    }

    return this->current_state;
}

SystemState State::getPreviousState()
{
    return this->previous_state;
}

SystemState State::getCurrentState()
{
    return this->current_state;
}

String State::getStateDescription(SystemState state)
{
    switch (state)
    {
    case NONE:
        return "NONE";
    case STARTING:
        return "Starting";
    case IDLE:
        return "Idle";
    case RUNNING:
        return "Running";
    case HALT:
        return "Halt";
    case FAULT:
        return "Fault";
    default:
        return "Unknown State";
    };
}