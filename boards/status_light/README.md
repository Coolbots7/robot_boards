# Status Light

The Status Light visually indicates the state of the robot using colors and
patterns.

<p align="center">
<image src="./images/v0.1.0_1.jpg" width="350px" />
</p>

## Structure

- `images/` - Directory for board images.
- `src/` - Directory for board source code files.
- `platformio.ini` - PlatformIO configuration file for the project.
- `README.md` - This file!

## Dependencies

Dependencies are tracked in the `platformio.ini` file.

## Hardware

The current board prototype uses COST breakout modules of the main components
wired together on protoboard.

### Pin Usage

| Pin | Usage      |
| --- | ---------- |
| 2   | I2C SDA    |
| 3   | I2C SCL    |
| 10  | RGB LED    |
| 14  | SW I2C SDA |
| 15  | SW I2C SCL |

### Software I2C Address Usage

Address usage on the on-board I2C bus.

| Address | Usage       |
| ------- | ----------- |
| 0x3C    | OLED screen |

## Usage

### Switching Workspaces

Use `ctrl + shift + P` to open the VSCode commands and search for `PlatformIO: Switch Project Environment`. Select the `Default boards/status_light` dropdown option.

### Build

Use the PlatformIO task to build the boards firmware `ctrl + alt + B`.

### Upload

Use the PlatformIO task to upload the boards firmware `ctrl + alt + U`.

### Serial Monitor

Use the PlatformIO task to open a serial connection to the board `ctrl + alt + S`.
