
//Create Motor Controller objects
MotorController motor1;
MotorController motor2;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  //After Wire master has been initialized, initialize motor controllers with motor properties
  motor1.begin(0x40, 400);
  motor2.begin(0x43, 400, true);

  //Enable motors
  motor1.setEnabled();
  motor2.setEnabled();
}

void loop() {
  //Set motors to continuous rotation mode
  motor1.setContinuous();
  motor2.setContinuous();

  //Test continuous rotation mode by moving motor speed in sinusoidal wave
  for (double i = 0; i < TWO_PI; i += TWO_PI / 100) {
    double RPM = sin(i) * 120;
    motor1.setSpeed(RPM);
    motor2.setSpeed(RPM);

    //Debug motor step position while moving
    Serial.println(motor1.getCurrentSteps());
    delay(50);
  }

  //Set motors to positional mode
  motor1.setContinuous(false);
  motor2.setContinuous(false);

  //Set motor speed for positional test
  motor1.setSpeed(60);
  motor2.setSpeed(60);

  //Test motors moving to steps relative to motor 0
  motor1.goToStep(400);
  motor2.goToStep(400);
  delay(5000);
  //Debug motor step position
  Serial.println(motor1.getCurrentSteps());

  //Test motors moving to steps relative to current position
  motor1.moveSteps(-400);
  motor2.moveSteps(-400);
  delay(5000);
  //Debug motor step position
  Serial.println(motor1.getCurrentSteps());

  delay(2000);
}
