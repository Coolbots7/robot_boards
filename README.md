# Robot Boards

This project was created to fill a void between the abundance of breakout boards
for the hobbyist community and industrial automation and robotics equipment.
This project is fully open source and suggestions and contributions are welcome.

### Standard Features

- USB-C programming and debugging interface
- RGB Status Indicator LED
- OLED Debugging Screen
- CANBus Communication
  - Address Select
- Datalogging SD Card

## Milestones

- v0 - Prototype phase
- v1 - AVR based
- v2 - ARM based

## Project Structure

- `boards/` - TODO
  - `battery_management_system/` - TODO
  - `brain_board/` - TODO
  - `emergency_stop/` - TODO
  - `motor_controller/` - TODO
  - `power_distribution_unit/` - TODO
  - `status_light/` - TODO
- `clients/` - TODO
- `lib/` - TODO
- `libraries/` - TODO

## Communication

When establishing a connection to a board the communication protocol can be
specified.

### Features

- Heartbeat w/ acknowledge
- Heartbeat timeout
- Synchronous data fetching
- Asynchronous data publishing and subscribing

### Protocols

#### CAN bus

TODO

#### I2C

I2C was the communication protocol used in the early prototypes. While it is
still available it is being replaced with CAN bus as the primary communication
protocol.

## Design Standards

### PCBs

#### Silkscreen

- Revision
- Date
- QR Code
