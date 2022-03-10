#include "cannon.h"

Servo servoL;
Servo servoR;
Cannon cannon(11, 8, 7, 12);   //motor pin, fill pin, bleed pin, fire pin

//IBUS stuff
IBusBM IBusServo;               //input for reading controller output
IBusBM IBusSensor;              //output for sending sensor data
const int safeChannel = 5;      //switch for safety
const int fireChannel = 4;      //switch for firing
const int fillBleed = 3;        //joystick, fill < 1500, bleed > 1500
const int angleChannel = 2;     //joystick for angle adjustment
const int driveF = 1;           //joystick for drive forward/backward
const int driveLR = 0;          //joystick for left right drive

void joystickCrontrol();

const double ERRORCORRETIONL = 4;
const int fireDelay = 500, fillDelay = 50;
unsigned long long int fireT, fillT;
bool fillPSI = false;

//initial setup code
void setup() {
  //set serial Baud rate
  Serial.begin(115200);

  //attach drive motors
  servoL.attach(9);
  servoR.attach(10);

  // iBUS setup
  IBusServo.begin(Serial1);  
  IBusSensor.begin(Serial2);

  //set solenoid pins to HIGH
  digitalWrite(13, LOW);
  digitalWrite(7, LOW);
  digitalWrite(12, LOW);

  
  //initialize the receiver and cannon
  cannon.init();
  
  //start IBUS monitor
  Serial.println("Start monitoring Cannon code");
  
  IBusSensor.addSensor(IBUS_PRESS);
  
}

//main code
void loop() {
  // show first 8 servo channels
  for (int i=0; i<8 ; i++) {
    Serial.print(IBusServo.readChannel(i));
    Serial.print(" ");
  }
  delay(50);
  Serial.print("PSI=");
  Serial.print(cannon.getPSI());


    //call drive control function   
    joystickCrontrol();

    //get PSI and update cannon class
    cannon.getPSI();

    //FIXME sensor readint
    //uint16_t speed=0;
    //IBusSensor.setSensorMeasurement(1,uint16_t(cannon.getPSI()));


    //if fire button clicked and safety button clicked
    if ((IBusServo.readChannel(4) > 1900) && (IBusServo.readChannel(5) > 1900))
    {
        Serial.print("Open cannon barrel");

      //if barrel oppened set fireT = program time
      if(cannon.barrelOpen())
        fireT = millis();
    }

    //if enough time passed since fire, close barrel
    if((millis() - fireT > fireDelay) && cannon.fired)
      cannon.barrelClose(); 
    
    //adjust angle based on input
    cannon.changeAngle( IBusServo.readChannel(angleChannel)/500); //FIXME adjust value
    
    //Increase PSI in tank
    //if(button pressed)
    //fill to desired psi
    if(IBusServo.readChannel(fillBleed) < 1300 && (millis()-fillT > fillDelay))
        fillPSI = true;
    else
        fillPSI = false;

    if(fillPSI == true)
    {
      if(cannon.ballastFill())
        fillPSI = false;
    }
    else        //stops fill upon second press
      cannon.stopFill();

    //call function to bleed PSI if ballast overfilled
    cannon.ballastBleed(IBusServo.readChannel(fillBleed) > 1700);  
    Serial.print('\n');
}

//Drive control -- adjusts servos to mdpad(RIGHT)
void joystickCrontrol()
{
  double speedF = IBusServo.readChannel(driveF) * 70 + 90;
  double speedLR = ( IBusServo.readChannel(driveLR) * 70 * -1 );

  servoL.write( ( ( 180 - speedF ) + ERRORCORRETIONL + speedLR ) );
  servoR.write( ( ( speedF ) + ERRORCORRETIONL + speedLR ) );
}