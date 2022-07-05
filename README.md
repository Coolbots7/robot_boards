# Robot Boards

This project was created to fill a void between the abundance of breakout boards
for the hobbyist community and industrial automation and robotics equipment.
This project is fully open source and suggestions and contributions are welcome.

## Structure

- `boards/` - Directory containing board designs and firmware.
  - `main_board/` - Board for running operational code and commanding other
    boards. See the [README](./boards/main_board/README.md) for more
    information.
  - `motor_controller/` - Board for receiving movement commands and commanding a
    motor driver. See the [README](./boards/motor_controller/README.md) for more
    information.
  - `status_light/` - Board for indicating the operational state of the robot.
    See the [README](./boards/status_light/README.md) for more information.
- `lib/` - Directory for custom libraries.
  - `display/` - Library for boards to display information on a OLED screen. See
    the [README](./lib/display/README.md) for more information.
  - `observability/` - Libraries for observing a board. See the
    [README](./lib/observability/README.md) for more information.
  - `state/` - Libraries for managing a boards state. See the
    [README](./lib/state/README.md) for more information.
- `README.md` - The file you are reading right now!
- `robot_boards.code-workspace` - File for tracking the VSCode Workspace
  settings. See [VSCode Workspace](#vscode-workspace) for more information.

## Requirements

- [VSCode](https://code.visualstudio.com/) - Visual Studio Code is the
  recommended IDE for working on this project. Instructions may assume VSCode
  usage.
- [PlatformIO](https://platformio.org/) - The PlatformIO VSCode Extension is
  used for embedded device development. The extension is installed in VSCode or
  is available for other IDEs (check their documentation).

### VSCode Workspace

Multiple root folders are used in the VSCode Workspace to support Multi-project
Workspaces in PlatformIO. These additional root folders are copies of other
folders already under the root workspace and can be ignored.
