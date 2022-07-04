# Robot Boards

## Library

### Communication

The communication layer is the data and physical layer, it handles sending and receiving data over various
communication methods. The current supported communication methods are:

- I2C
- CanBus

The communication layer used for each board is configured as part of the initialization process.

```cpp
void setup() {
    board = new Board();
    board.beginCanBus();
}
```

### Boards

The board layer is the application layer, it handles performing specific actions against a board. The actions include setting configuration, reading input data, and commanding outputs. The current supported boards are:

- Status Light
- Motor Controller
- BMS
- EStop

```cpp
MotorControllerBoard *motorController;

void setup() {
    Serial.begin(115200);

    motorController = new MotorControllerBoard();
    motorController->beginCanBus();

    motorController->enableA();
    motorController->enableB();
    motorController->reverseB();
}

void loop() {
    motorController->setSpeedA(0.8);
    motorController->setSpeedB(0.8);

    Serial.println("Motor A Current: " + String(motorController->getCurrentA()));
}
```
