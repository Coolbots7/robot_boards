# Communication

This document covers how communication is implemented for the supported
protocols.

## Contents

- [Library Structure](#library-structure)
- [Message Structure](#message-structure)
- [Heartbeat](#heartbeat)
- Protocols
  - [TwoWire](#twowire)

### Library Structure

- `lib/`
  - `boards/` - Library for communicating with boards
    - `common/` - Classes for common board interfaces
      - `state.h` - Class for interfacing with board State Machines
    - `status_light/` - Class for communicating with Status Light boards
    - `board.h` - Base class for common board communication functions
  - `communication/` - Library for implementing communication protocols
    - `twowire/` - Source files for implementing the TwoWire communication
      protocol
      - `common.h` - Class for implementing TwoWire utilities used by both the
        master and slaves
      - `master.h` - Class for communicating with a board over TwoWire
      - `slave.h` - Class for boards to handle communication over TwoWire

### Message Structure

Messages contain the identifier of the value being sent as a single byte and the
value for that type as a byte array.

### Heartbeat

Heartbeat messages are messages sent on a regular period for boards to
communicate their current state with the rest of the network. The Main Board's
heartbeat message is used by all other boards to sync their clocks and states.
Boards send their state back to the Main Board which it uses to detect faults
and halt the entire system.

## Protocols

### TwoWire

TwoWire aka I2C aka IIC is a common protocol used for microcontrollers to
communicate with peripheral devices. The same two wires can be used to
communicate with multiple devices each with a unique address.

A downside of the TwoWire protocol is that peripheral devices cannot initiate
communication to the controller or communicate at all with other peripheral
devices.

#### Topography

All boards are connected to the same SDA and SCL bus lines. This is done by
daisy chaining the boards using cables that provide SDA and SCL as well as power
and ground (reference). The Main Board is the master of the TwoWire bus, and all
other boards are slaves.

#### Data Flow

Since TwoWire does not allow for slave devices to initiate communication with
the master (or other slaves), the master must read from and write to the slaves
at a regular period.

- The master will send messages to slave boards when events occur on the main
  board that are relevant to a board.
- The master will periodically send a [heartbeat](#heartbeat) message to every
  attached board and request each boards state in return.

#### Addressing

Each board can be assigned an address within the allotted address range for that
type of board. Since addresses do not affect bus priority, as the master is in
charge of data flow, all boards are treated the same.

| Start Address (hex) | End Address (hex) | Range |    Usage     |
| :-----------------: | :---------------: | :---: | :----------: |
|        0x00         |       0x00        |   1   |  Main Board  |
|        0x50         |       0x53        |   4   | Status Light |

#### Registers

Registers are unique addresses that identify the data being written to or read
from a slave device:

| Register (hex) |         Value         | Read / Write |
| :------------: | :-------------------: | :----------: |
|      0xFC      |    HEARTBEAT_STATE    |      W       |
|      0xFD      |    HEARTBEAT_TIME     |      W       |
|      0xFE      |         STATE         |      R       |
|      0xFF      | REGISTER_REQUEST_TYPE |      W       |
