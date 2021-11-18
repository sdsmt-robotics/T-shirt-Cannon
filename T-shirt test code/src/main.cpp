
#include "cannon.h"


Servo servoL;
Servo servoR;
//Create the communications object. Use Serial for the communications.
Controller controller(Serial3);

void joystickCrontrol();

bool disconnected = false;
double ERRORCORRETIONL = 4;

void setup() {
  // put your setup code here, to run once:
  
  Cannon cannon(11);
  servoL.attach(9);
  servoR.attach(10);

  pinMode(12, OUTPUT);
  
  Serial.begin(9600);
  
  //initialize the receiver
  controller.init();
  
  Serial.println("Waiting for connection...");
  while (!controller.connected()) { delay(10); }
  Serial.println("Connected...");
  
  //set a deadzone for the joysticks
  controller.setJoyDeadzone(0.08);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (controller.connected()) {
    
    joystickCrontrol();

    if (controller.joyButton(LEFT))
      cannon.fire();
    
    cannon.changeAngle( controller.joystick(LEFT, Y) / 50 );

    disconnected = false;

  } else {
    if (!disconnected) {
      Serial.println("Disconnected! Waiting for reconnect...");
      disconnected = true;
    }
  }
}

void joystickCrontrol()
{
  double speedF = controller.joystick(RIGHT, Y) * 70 + 90;
  double speedLR = ( controller.joystick(RIGHT, X) * 70 * -1 );

  servoL.write( ( ( 180 - speedF ) + ERRORCORRETIONL + speedLR ) );
  servoR.write( ( ( speedF ) + ERRORCORRETIONL + speedLR ) );
}