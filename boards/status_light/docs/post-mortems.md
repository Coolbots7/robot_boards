# Status Light Post-mortems

## HW v0.1.0

### Problems

Describe any problems with this version of the board, the cause, and propose solutions for the next version:

| Problem                               | Cause                                                                                       | Proposed Solution                                |
| ------------------------------------- | ------------------------------------------------------------------------------------------- | ------------------------------------------------ |
| Unable to use the onboard OLED screen | ATMega32u4 only has a single I2C bus. Screen library not working with Software TwoWire bus. | Use a microcontroller with at least 2 I2C busses |

### Desired Features

Desired features to include on the next version:

- Buzzer or speaker for audible alerts
