# Main Board

The Main Board runs the operational code for the robot. It interfaces with other
boards that provide sensing and other abilities. The board provides Bluetooth
and Wi-Fi connectivity from the onboard MCU and SBUS for RC control.

<p align="center">
<image src="./images/v0.1.0_3.jpg" width="350px" />
</p>

## Structure

- `images/` - Directory for board images.
- `src/` - Directory for board source code files.
- `platformio.ini` - PlatformIO configuration file for the project.
- `README.md` - This file!

## Dependencies

Dependencies are tracked in the `platformio.ini` file.

## Hardware

The current board prototype uses COTS breakout modules of the main components
wired together on protoboard.

### Pin Usage

| Pin | Usage      |
| --- | ---------- |
| 16  | SBUS RX    |
| 17  | SBUS TX    |
| 21  | I2C SDA    |
| 22  | I2C SCL    |
| 25  | SW I2C SDA |
| 27  | SW I2C SCL |
| 32  | RGB LED    |

### Software I2C Address Usage

Address usage on the on-board I2C bus.

| Address | Usage       |
| ------- | ----------- |
| 0x3C    | OLED screen |

## Usage

### Switching Workspaces

Use `ctrl + shift + P` to open the VSCode commands and search for `PlatformIO: Switch Project Environment`. Select the `Default boards/main_board` dropdown option.

### Build

Use the PlatformIO task to build the boards firmware `ctrl + alt + B`.

### Upload

Use the PlatformIO task to upload the boards firmware `ctrl + alt + U`.

### Serial Monitor

Use the PlatformIO task to open a serial connection to the board `ctrl + alt + S`.
