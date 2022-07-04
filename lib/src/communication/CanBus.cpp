#include "./CanBus.h"

// CanBus
static uint23_t CanBus::getCANAddress(uint32_t addr, uint16_t reg)
{
    // <16 bits> & <13 bits>
    // TODO bit masks
    return ((reg << CAN_ADDRESS_BITS) & 0000b) | (addr & 0000b);
}

static void CanBus::writeFrame(uint16_t addr, uint32_t reg, uint8_t *data, uint8_t n)
{
    can_frame frame;

    frame.can_id = this->getCANAddress(reg);
    frame.can_dlc = n;
    memset(&frame.data, 0, 8);
    memcpy(&frame.data, data, n);

    // TODO send frame
}

// CanBusDevice

// void publishByte(uint8_t data)
// {
//     CanBus::writeFrame(reg, data, 1);
// }

// void publishBool(bool data)
// {
//     uint8_t buff[sizeof(bool)];
//     Prim::encodeBool(data, buff, sizeof(bool));
//     CanBus::writeFrame(reg, buff, sizeof(bool))
// }

// void publishFloat(float data)
// {
//     // TODO
// }

// void publishULong(uint32_t data)
// {
//     // TODO
// }

// CanBusController
CanBusController::CanBusController(uint8_t cs_pin, uint8_t interrupt_pin, uint16_t base_address, uint8_t address_offset) : base(base_address, address_offset)
{
    this->cs_pin = cs_pin;
    this->interrupt_pin = interrupt_pin;

    // TODO init SPI

    // TODO attach interrupt
}

bool CanBusController::ping()
{
    // TODO ping over canbus
    return true;
}

void CanBusController::writeRegisterByte(uint16_t reg, uint8_t data)
{
    // TODO combine address and register into can id

    // TODO send data frame to device register
}

uint8_t CanBusController::readRegisterByte(uint16_t reg)
{
    // TODO combine address and register into can id

    // TODO send remote frame to can id

    // TODO filter for response
}

// TODO bool

// TODO float

// TODO ulong