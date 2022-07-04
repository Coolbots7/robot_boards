#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Wire.h>


class MotorController
{
private:
    struct WireReceive
    {
        uint8_t message_type;
        uint8_t data[4];
    };

    //Define constants used to distinguish the intent of the value being sent
    enum MessageType
    {
        ENABLED = 0x01,
        COAST = 0x02,
        SPEED = 0x03,
        // REVERSE = 0x02,
        // STEPS_PER_REVOLUTION = 0x03,
        // RESET = 0x07,
        // CONTINUOUS = 0x08,
        // MOVE_STEPS = 0x09,
        // GO_TO_STEP = 0x0A
        REQUEST_TYPE = 0xFF
    };

    //Define constants used to select the value to be sent on next I2C request event
    enum RequestType
    {
        CURRENT = 1,
        // STEPS = 2
    };

    uint8_t wire_address;

    void sendByte(MessageType message_type, uint8_t value) {
      uint8_t data[4];
      memset(data, 0x00, sizeof(data));
      memcpy(data+3, &value, 1);
      sendData(message_type, data);
    }

    void sendDouble(MessageType message_type, double value) {
      uint8_t data[4];
      memset(data, 0x00, sizeof(data));
      memcpy(data, &value, 4);
      sendData(message_type, data);
    }

    //Send a message to the motor controller with value
    void sendData(MessageType message_type, const char* value)
    {
        WireReceive send_data;
        send_data.message_type = message_type;
        memcpy(send_data.data, value, 4);
        sendData(send_data);
    }

    //Send message data structure to motor controller
    void sendData(WireReceive send_data)
    {
        //Create byte array to hold payload as raw bytes
        byte send_bytes[sizeof(WireReceive)];

        //Copy payload bytes to byte array
        memcpy(send_bytes, &send_data, sizeof(WireReceive));

        //Send raw byte array to slave
        Wire.beginTransmission(wire_address);
        Wire.write(send_bytes, sizeof(WireReceive));
        Wire.endTransmission();
    }

    double getDouble(RequestType request_type) {      
        double value;
        
        sendByte(REQUEST_TYPE, request_type);
        //Trigger request event on slave
        Wire.requestFrom(wire_address, sizeof(value));

        //Wait for data to be available
        while (Wire.available() <= 0)
            ;

        //Read raw bytes from slave
        uint8_t num_bytes = Wire.available();
        byte received_bytes[num_bytes];
        for (int i = 0; i < num_bytes; i++)
        {
            received_bytes[i] = Wire.read();
        }

        //Copy raw bytes to the destination pointer
        memcpy(&value, received_bytes, sizeof(value));

        return value;
    }

    //Get specified value from the motor controller
//    void getData(RequestType request_type, const void *destination, uint8_t num_bytes)
//    {
//        //Create message payload
////        WireReceive send_data;
////        send_data.message_type = REQUEST_TYPE;
////        send_data.value = request_type;
//        //Send message to motor specifying what value to return on next I2C request
////        sendData(send_data);
//        sendByte(REQUEST_TYPE, request_type);
//
//        //Trigger request event on slave
//        Wire.requestFrom(wire_address, num_bytes);
//
//        //Wait for data to be available
//        while (Wire.available() <= 0)
//            ;
//
//        //Read raw bytes from slave
//        num_bytes = Wire.available();
//        byte received_bytes[num_bytes];
//        for (int i = 0; i < num_bytes; i++)
//        {
//            received_bytes[i] = Wire.read();
//        }
//
//        //Copy raw bytes to the destination pointer
//        memcpy(destination, received_bytes, num_bytes);
//    }

public:
    MotorController(uint8_t address)
    {
        this->wire_address = address;
    }

    // // ====== Begin Communication ======
    // //Initialize motor with I2C address and steps per revolution
    // //Use default not reversed and enable state
    // begin(byte wire_address, unsigned int steps_per_revolution)
    // {
    //     begin(wire_address, steps_per_revolution, false);
    // }

    // //Initialize motor I2C address, steps per revolution, and reversed state
    // //Use default enable pin state
    // begin(byte wire_address, unsigned int steps_per_revolution, bool reversed)
    // {
    //     begin(wire_address, steps_per_revolution, reversed, DEFAULT_ENABLE_STATE);
    // }

    // //Initialize motor I2C address, steps per revolution, reversed state, and enable pin state
    // begin(byte address, unsigned int steps_per_revolution, bool reversed, bool enable_state)
    // {
    //     wire_address = address;

    //     setStepsPerRevolution(steps_per_revolution);
    //     setReversed(reversed);
    //     setEnableState(enable_state);

    //     reset();
    // }


    // ====== Set Enabled ======
    //Disable the motor
    void setDisabled()
    {
        setEnabled(false);
    }

    //Enable the motor
    void setEnabled()
    {
        setEnabled(true);
    }

    //Set motor enabled state
    void setEnabled(bool enabled)
    {
        sendByte(ENABLED, enabled ? 0x01 : 0x00);
    }


    // ====== Set Coast ======
    void setCoast()
    {
        setCoast(true);
    }

    void setBrake()
    {
        setCoast(false);
    }

    void setCoast(bool coast)
    {
        sendByte(COAST, coast ? 0x01 : 0x00);
    }


    // ====== Set Speed ======
    //Set motor speed
    void setSpeed(double speed)
    {
        sendDouble(SPEED, speed);
    }


    // //Reset motor to post initialization state
    // reset() {
    //   sendData(RESET, double(0));
    // }

    // setStepsPerRevolution(int steps_per_revolution) {
    //   sendData(STEPS_PER_REVOLUTION, steps_per_revolution);
    // }

    // //Set the enable pin state for the motor to be enabled
    // setEnableState(bool state) {
    //   sendData(ENABLE_STATE, state == HIGH ? double(1) : double(0));
    // }

    // //Move the specified number of steps
    // moveSteps(long steps) {
    //   sendData(MOVE_STEPS, double(steps));
    // }

    // //Go to the specified step from the last time the motor was initialized
    // goToStep(long step) {
    //   sendData(GO_TO_STEP, double(step));
    // }

    // //Set motor to ocntinuous rotation mode
    // setContinuous() {
    //   setContinuous(true);
    // }

    // //Set motor continuous rotation state
    // setContinuous(bool continuous) {
    //   sendData(CONTINUOUS, continuous ? double(1) : double(0));
    // }

    // //Invert motor forward direction
    // setReversed() {
    //   setReversed(true);
    // }

    // //Set motor reversed state
    // setReversed(bool reversed) {
    //   sendData(REVERSE, reversed ? double(1) : double(0));
    // }


    // ====== Get Current ======
    double getCurrent() {
        double current = getDouble(CURRENT);
        return current;
    }


    // //Get the current motor step position
    // getCurrentSteps() {
    //   long current_steps;
    //   getData(CURRENT_STEPS, &current_steps, sizeof(current_steps));
    //   return current_steps;
    // }
};

#endif
