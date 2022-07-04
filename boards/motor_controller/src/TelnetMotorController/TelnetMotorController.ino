#include <ConfigurableEthernet.h>
#include <SPI.h>
#include <Ethernet.h>

//EEPROM Address Definitions
//0-21 used by ConfigurableEthernet library
//velocity
//ramp up
//ramp down

String command;

ConfigurableEthernet eth;

// telnet defaults to port 23
EthernetServer server(23);
boolean alreadyConnected = false;

void setup() {
  Serial.begin(9600);

  eth.begin(Serial);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start listening for clients
  server.begin();

  Serial.print("Chat server address:");
  Serial.println(Ethernet.localIP());

}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clear out the input buffer:
      client.flush();
      Serial.println("We have a new client");
      client.print(Ethernet.localIP());
      client.print(" $");
      alreadyConnected = true;
    }

    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();

      if (thisChar == '\n') {
        Serial.println();
        client.println("\nEcho: " + command);
        client.print(Ethernet.localIP());
        client.print(" $");
        int commandLen = command.length();
        char com[commandLen];
        command.toCharArray(com, commandLen);
        UseCommand(com);
        command = "";
      }
      else {
        if (thisChar != '\r') {
          command += thisChar;
          Serial.write(thisChar);
        }
      }
    }
  }
}

void UseCommand(char* Message) {

  //Get command of message
  String Command = strtok_r(Message, " ", &Message);
  Serial.println("Command: " + Command);

  //Split command arguments
  int argc = 0;
  for (int i = 0; i < sizeof(Message); i++) {
    if (Message[i] == ' ') argc++;
  }
  if (argc > 0) argc++;
  Serial.print("Num Args: ");
  Serial.println(argc);

  String argv[argc];
  int argIdx = 0;
  char *p = Message;
  char *str;
  while ((str = strtok_r(p, " ", &p)) != NULL) { // delimiter is the semicolon
    argv[argIdx++] = str;
  }
  Serial.println("Args: ");
  for (int i = 0; i < argc; i++) {
    Serial.print("\t");
    Serial.println(argv[i]);
  }

  //commands
  //controller.ethernet.config.mac
  //controller.ethernet.config.ip
  //controller.ethernet.config.dns
  //controller.ethernet.config.gateway
  //controller.ethernet.config.subnet

  //controller.encoder.config.type (continuous, incremental)
  //controller.encoder.config.has_index_pulse (true,false)
  //controller.encoder.enocder_position

  //controller.motor.config.mode (servo, continuous)
  //controller.motor.config.type (DC, Stepper)
  //controller.motor.config.control_method (step_direction, h_bridge)
  //controller.motor.config.invert_direction (true,false)
  //controller.motor.config.max_velocity (counts/sec)
  //controller.motor.config.acceleration
  //controller.motor.config.deceleration
  //controller.motor.velocity
  //controller.motor.position

  //controller.reset -power cycle the arduino
  //controller.commit -save current configuration settings to non-volatile memory
  //controller.revert -overwrite current configuration settings with settings in non-volatile memory

  //Determine command
  if (Command == "GOTO") {

  }
  else if (Command == "MOV") {

  }
  else {

  }
}
