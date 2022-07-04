#ifndef I2C_CONTROLLER
#define I2C_CONTROLLER

class I2CController {
  private:
    struct WireReceive
    {
      byte message_type;
      uint8_t data[4];
    };

    //Define constants used to distinguish the intent of the value being sent
    enum MessageType
    {
      REQUEST = 255,
    };

    //Define constants used to select the value to be sent on next I2C request event
    enum RequestType
    {
      STATE = 1,
    };
    
    uint8_t wire_address;
    uint8_t request_type;

    signed char decodeByte(const char *data)
    {
      signed char value;
      memcpy(&value, data + 3, 1);
      return value;
    }

    double decodeDouble(const char *data)
    {
      double value;
      memcpy(&value, data, 4);
      return value;
    }

  public:
    I2CController(uint8_t address)
    {
      this->wire_address = address;
    }

    uint8_t getAddress()
    {
      return this->wire_address;
    }

    void receiveEvent(int byte_count)
    {
      //Read incoming bytes from master
      uint8_t received_bytes[byte_count];
      for (int i = 0; i < byte_count; i++)
      {
        received_bytes[i] = Wire.read();
      }

      //Convert raw incoming bytes to data structure
      WireReceive wire_data;
      memcpy(&wire_data, received_bytes, byte_count);

      switch (wire_data.message_type)
      {
        //Set the value to be returned on the next I2C request event
        //        case REQUEST_TYPE:
        //          request_type = int(decodeByte(wire_data.data));
        //          request_motor_idx = wire_data.motor_idx;
        //          break;
        default:
          Serial.print("ERROR: Unknown message type: ");
          Serial.println(wire_data.message_type);
      }
    }

    void requestEvent()
    {
      //    //Send the current draw from the motor
      //    if (request_type == CURRENT)
      //    {
      //      double current = 0;
      //
      //      if (motor1Attached && request_motor_idx == 1)
      //      {
      //        current = motor1->getCurrent();
      //      }
      //
      //      if (motor2Attached && request_motor_idx == 2)
      //      {
      //        current = motor2->getCurrent();
      //      }
      //
      //      uint8_t send_data[4];
      //      memset(send_data, 0x00, sizeof(send_data));
      //      memcpy(send_data, &current, sizeof(send_data));
      //      Wire.write(send_data, sizeof(send_data));
      //
      //      //        Serial.print("Current: ");
      //      //        Serial.println(current, 3);
      //    }
      // //Send the current step position of the motor
      // if (request_type == CURRENT_STEPS) {
      //   byte send_data[sizeof(current_steps)];
      //   memcpy(send_data, &current_steps, sizeof(current_steps));
      //   Wire.write(send_data, sizeof(current_steps));
      // }
    }
    
};

#endif
