# Main Board

The Main Board runs the operational code for the robot. It interfaces with other
boards that provide sensing and other abilities. The board provides Bluetooth
and Wi-Fi connectivity from the onboard MCU and SBUS for RC control.

<p align="center">
<image src="./images/v0.1.0_3.jpg" width="350px" />
</p>

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
