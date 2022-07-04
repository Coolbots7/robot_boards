#include <Wire.h>

#include "MotorController.h"

#define MOTOR_ADDRESS 0x40
// #define MOTOR_ADDRESS 0x41
// #define MOTOR_ADDRESS 0x42
// #define MOTOR_ADDRESS 0x43

MotorController *motor;


void setup() {
  Serial.begin(115200);

  Wire.begin();

//  Serial.println("Initialize Motor Controller");
  motor = new MotorController(MOTOR_ADDRESS);

//  Serial.println("Enable Motor");
//  motor->setEnabled();

//  //  Serial.println("Set Motor to Coast");
  motor->setCoast();
//    motor->setBrake();
//
//  Serial.println("Set motor speed to 0.25");
//  motor->setSpeed(0);
//
//  delay(2000);

//  motor->setSpeed(0);
//  motor->setDisabled();

//  delay(2000);
}

double delta = 0.01;
double speed = 0;
unsigned long lastCurrentTime = 0;
const int currentInterval = 500;
void loop() {
//  Serial.println("disabled");
//  motor->setDisabled();
//  delay(2000);
//  Serial.println("enabled");
//  motor->setEnabled();
//  delay(2000);
  
  if (millis() - lastCurrentTime > currentInterval) {
    lastCurrentTime = millis();
    Serial.print("Motor current: ");
    Serial.println(motor->getCurrent());
  }

  if (speed > 1.0 || speed < 0) {
    delta *= -1;
  }

  speed += delta;
//  Serial.println(speed);
  motor->setSpeed(speed);

  delay(20);
}
