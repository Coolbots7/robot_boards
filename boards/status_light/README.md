# Status Light

The Status Light visually indicates the state of the robot using colors and
patterns.

<p align="center">
<image src="./images/v0.1.0_1.jpg" width="350px" />
</p>

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
