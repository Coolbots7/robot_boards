#include "./Communication.h"

class EventDevice : public CommunicationDevice
{
public:
    // publish values
    // Publish the value of the specified register
    virtual void publishByte(uint8_t reg, uint8_t val);
    virtual void publishBool(uint8_t reg, bool val);
    virtual void publishFloat(uint8_t reg, float val);
    virtual void publishULong(uint8_t reg, uint32_t val);
}

class EventController : public CommunicationController
{
public:
    // TODO subscribe
}