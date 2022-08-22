# MCU Main Board Post-mortems

## HW v0.1.0

### Problems

Describe any problems with this version of the board, the cause, and propose solutions for the next version:

| Problem                                                                             | Cause                                         | Proposed Solution                        |
| ----------------------------------------------------------------------------------- | --------------------------------------------- | ---------------------------------------- |
| Not enough room on the screen for the amount of information desired to be displayed | 128x32 screen too small                       | Use a larger screen                      |
| -                                                                                   | Need more than one 'page'                     | Add pages and navigation to the display  |
| Microcontroller freezes while attempting to communicate with other boards           | I2C bus connection issues                     | Use a more rigid connector               |
| -                                                                                   | Saturating the I2C bus / other protocol issue | Use a more robust communication protocol |

### Desired Features

Desired features to include on the next version:

- SD Card for data logging
- Real Time Clock to support data logging
