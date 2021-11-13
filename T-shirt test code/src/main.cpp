#include <Arduino.h>
#include <Servo.h>
#include <..\lib\controller\Controller.h>

Servo servoL;
Servo servoR;
Servo servoCannon;

//Create the communications object. Use Serial for the communications.
Controller controller(Serial3);

void joystickCrontroll();
void buttonControll();
void cannonControll();
double getVoltage();

bool disconnected = false;
double ERRORCORRETIONL = 4;


void setup() {
  // put your setup code here, to run once:

  servoL.attach(9);
  servoR.attach(10);
  servoCannon.attach(11);

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
    
    //buttonControll();

    joystickCrontroll();
    cannonControll();

    disconnected = false;

  } else {
    if (!disconnected) {
      Serial.println("Disconnected! Waiting for reconnect...");
      disconnected = true;
    }
  }
}


void buttonControll()
{
  if(controller.button(UP))
    {
      servoL.write(120);
      Serial.println("Button UP");
    }
    else
      servoL.write(94);
  
    if(controller.button(DOWN))
    {
      servoL.write(60);
      Serial.println("Button DOWN");
    }
    else
      servoL.write(94);
}

void joystickCrontroll()
{
  double speedF = controller.joystick(RIGHT, Y) * 70 + 90;
  double speedLR = ( controller.joystick(RIGHT, X) * 70 * -1 );

  servoL.write( ( ( 180 - speedF ) + ERRORCORRETIONL + speedLR ) );
  servoR.write( ( ( speedF ) + ERRORCORRETIONL + speedLR ) );
}


void cannonControll()
{
  bool isSafe = false;
  static double angle = 90;
  angle -= controller.joystick(LEFT, Y) / 50;
  double PSI = getVoltage() * (200 / 5.0);
  double lowerPSI = 40;
  double upperPSI = 150;

  if(angle > 90)
    angle = 90;

  if(angle < 35)
    angle = 35;
  
  if(controller.button(UP))
    isSafe = true;

  servoCannon.write(angle);

  if( controller.dpad(LEFT) && isSafe && PSI > lowerPSI && PSI < upperPSI )
    digitalWrite( 12, HIGH );
  else
    digitalWrite( 12, LOW );
}


double getVoltage()
{
  return analogRead(A0) * (5.0 / 1023);
}