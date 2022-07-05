# Motor Controller

The Motor Controller receives commands and interfaces with an external Motor
Driver to drive a motor.

<p align="center">
<image src="./images/v0.1.0_1.jpg" width="350px" />
</p>

## Hardware

The current board prototype uses COTS breakout modules of the main components
wired together on protoboard.

### Pin Usage

| Pin | Usage             |
| --- | ----------------- |
| 2   | I2C SDA           |
| 3   | I2C SCL           |
| 4   | Motor 1 A         |
| 5   | Motor 1 Enable    |
| 6   | Motor 2 Enable    |
| 7   | Motor 1 B         |
| 8   | Motor 2 A         |
| 9   | Motor 2 B         |
| 10  | RGB LED           |
| 14  | SW I2C SDA        |
| 15  | SW I2C SCL        |
| 16  | Address Select b0 |

### Software I2C Address Usage

Address usage on the on-board I2C bus.

| Address | Usage       |
| ------- | ----------- |
| 0x3C    | OLED screen |
