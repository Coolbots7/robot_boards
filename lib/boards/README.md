# lib/boards

Library for communicating with Robot Boards from a controller device like the
[Main Board](../../boards/main_board/README.md).

## Structure

- `src/` - Directory for board source code files.
  - `common/` - Directory for common interfaces between boards.
  - `status_light/` - Directory for library to interface with a Status Light
    board.
- `library.json` - The libraries manifest file.
- `README.md` - This file!

## Dependencies

Dependencies are tracked in the `library.json` file.

## Usage

See [communication](../../docs/communication.md) documentation for usage.
