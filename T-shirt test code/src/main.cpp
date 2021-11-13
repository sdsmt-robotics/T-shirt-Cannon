#include <Arduino.h>
#include <Servo.h>

void setup() {
  // put your setup code here, to run once:
  Servo servoL;

  servoL.attach(9);

  servoL.write(100);
}

void loop() {
  // put your main code here, to run repeatedly:
}