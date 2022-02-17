#include "cannon.h"
#include <../lib/IBusBM/IBusBM.h>

Servo servoL;
Servo servoR;
//Create the communications object. Use Serial for the communications.
Controller controller(Serial3);
Cannon cannon(11, 13, 7, 12);   //motor pin, fill pin, bleed pin, laser pin

void joystickCrontrol();

bool disconnected = false;
const double ERRORCORRETIONL = 4;
const int fireDelay = 500, fillDelay = 50;
unsigned long long int fireT, fillT;
bool fillPSI = false;

//initial setup code
void setup() {
  //attach drive motors
  servoL.attach(9);
  servoR.attach(10);

  //set solenoid pins to HIGH
  digitalWrite(13, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(12, HIGH);

  //set serial Baud rate
  Serial.begin(9600);
  
  //initialize the receiver and cannon
  controller.init();
  cannon.init();
  
  //check for connection
  Serial.println("Waiting for connection...");
  while (!controller.connected()) { delay(10); }
  Serial.println("Connected...");
  
  //set a deadzone for the joysticks
  controller.setJoyDeadzone(0.08);
}

//main code
void loop() {
  
  if (controller.connected()) {

    //call drive control function   
    joystickCrontrol();

    //get PSI and update cannon class
    cannon.getPSI();

    //if fire button clicked and safety button clicked
    if (controller.dpad(LEFT) && controller.button(UP) )
    {
      //if barrel oppened set fireT = program time
      if(cannon.barrelOpen())
        fireT = millis();
    }

    //if enough time passed since fire, close barrel
    if((millis() - fireT > fireDelay) && cannon.fired)
      cannon.barrelClose(); 
    
    //adjust angle based on input
    cannon.changeAngle( controller.joystick(LEFT, Y) / 50 );
    
    //Increase PSI in tank
    //if(button pressed)
    //fill to desired psi
    if(controller.dpad(RIGHT) && (millis()-fillT > fillDelay))
    {
      if(fillPSI == true)
        fillPSI = false;
      else
        fillPSI = true;

      fillT = millis();
    }

    if(fillPSI == true)
    {
      if(cannon.ballastFill())
        fillPSI = false;
    }
    else        //stops fill upon second press
      cannon.stopFill();

    //call function to bleed PSI if ballast overfilled
    cannon.ballastBleed(controller.dpad(DOWN));  

    disconnected = false;
  }
  //if controller disconnects, print message 
  else {
    if (!disconnected) {
      Serial.println("Disconnected! Waiting for reconnect...");
      disconnected = true;
    }
  }
}

//Drive control -- adjusts servos to mdpad(RIGHT)
void joystickCrontrol()
{
  double speedF = controller.joystick(RIGHT, Y) * 70 + 90;
  double speedLR = ( controller.joystick(RIGHT, X) * 70 * -1 );

  servoL.write( ( ( 180 - speedF ) + ERRORCORRETIONL + speedLR ) );
  servoR.write( ( ( speedF ) + ERRORCORRETIONL + speedLR ) );
}