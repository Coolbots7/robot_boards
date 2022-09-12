#include "twowire/common.h"

#include "status_light.h"

StatusLightInterface::StatusLightInterface(uint8_t address_offset, I2CMaster *i2c_master) : BoardInterface(I2C_STATUS_LIGHT_BASE_ADDRESS + address_offset, i2c_master) {}
