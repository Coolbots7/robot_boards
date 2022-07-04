#ifndef REMOTE_H
#define REMOTE_H

#include "RF24Network.h"
#include "RF24.h"
#include "RF24Mesh.h"
#include <SPI.h>
//Include eeprom.h for AVR (Uno, Nano) etc. except ATTiny
#include <EEPROM.h>

/***** Configure the chosen CE,CS pins *****/
RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

uint32_t displayTimer = 0;

struct Heartbeat {
  uint64_t time;
  uint8_t robot_state;
  uint8_t board_state;
  uint8_t estop_state;
}

class Remote {
  private:
    void sendHeartbeat() {
      Heartbeat message;
      
      byte data[sizeof(Heartbeat)];
      memcpy(&message, data, sizeof(Heartbeat));
      network.send(data, sizeof(Heartbeat));
      
    }

  public:
    Remote() {
      // Set the nodeID to 0 for the master node
      mesh.setNodeID(0);
      // Connect to the mesh
      mesh.begin();
    }

    update() {
      // Call mesh.update to keep the network updated
      mesh.update();

      // In addition, keep the 'DHCP service' running on the master node so addresses will
      // be assigned to the sensor nodes
      mesh.DHCP();

      this->sendHeartbeat();

      // Check for incoming data from the sensors
      if (network.available()) {
        RF24NetworkHeader header;
        network.peek(header);

        uint32_t dat = 0;
        switch (header.type) {
          // Display the incoming millis() values from the sensor nodes
          case 'M': network.read(header, &dat, sizeof(dat)); Serial.println(dat); break;
          default: network.read(header, 0, 0); Serial.println(header.type); break;
        }
      }

    }

    String debug() {
      if (millis() - displayTimer > 5000) {
        displayTimer = millis();
        Serial.println(" ");
        Serial.println(F("********Assigned Addresses********"));
        for (int i = 0; i < mesh.addrListTop; i++) {
          Serial.print("NodeID: ");
          Serial.print(mesh.addrList[i].nodeID);
          Serial.print(" RF24Network Address: 0");
          Serial.println(mesh.addrList[i].address, OCT);
        }
        Serial.println(F("**********************************"));
      }

    }
};

#endif
