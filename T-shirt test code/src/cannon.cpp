#include "cannon.h"

Cannon::Cannon(int _pin, int fpin, int bpin, int rpin)
{
    pin = _pin;
    fillPin = fpin;
    bleedPin = bpin;
    relayPin = rpin;
}

//attach cannon to pin and set pins to output
void Cannon::init()
{
    cannon.attach(pin);
    pinMode(relayPin, OUTPUT);
    pinMode(bleedPin, OUTPUT);
    pinMode(fillPin, OUTPUT);
}

//writes angle of cannon given the parameter input/adjust angle from controller input
void Cannon::setAngle(double input)
{
    angle = input;
    
    //limits angle to set max and min
    if(angle > 90)
        angle = 90;
    if(angle < 35)
        angle = 35;

    //write angle to pin  
    cannon.write(angle);
}

//adjusts angle by controller input and then writes it to cannon
void Cannon::changeAngle(double input)
{
    setAngle(angle -= input);
}

//check to see if all clear to fire
bool Cannon::checkSafe()
{
    //if PSI in acceptable range, set canFire=true
    if( PSI > lowerPSI && PSI < upperPSI)
        canFire = true;
    else
        canFire = false;
    
    return canFire;
}

//update and return the PSI
double Cannon::getPSI()
{
    PSI = ( analogRead(A0) * (5.0/1023)) * (200/5.0);
    return 100;
}

//Checks if it is safe and able to fire, then opens the barrel
bool Cannon::barrelOpen()
{
    checkSafe();

    if(canFire)
    {
        digitalWrite( relayPin, LOW );  //set low to fire
        fired = true;
        return true;
    }
    fired = false;
    return false;
}

//Will close the barrel
void Cannon::barrelClose()
{
    digitalWrite( relayPin, HIGH );     //set high to close
    fired = false;
}

//If ballast fill button is pressed, solenoid will open -- all else solenoid is closed
bool Cannon::ballastFill()
{
    if(PSI >= desiredPSI)
    {
        digitalWrite( fillPin, HIGH);   //set high to stop filling
        return true;
    }

    digitalWrite( fillPin, LOW );       //set low to open solenoid
    return false;
}

void Cannon::stopFill()
{
    digitalWrite(fillPin, HIGH);
}

//check PSI and bleed if needed
void Cannon::ballastBleed(bool bleed)
{
    if ((PSI > upperPSI) || bleed)
        digitalWrite( bleedPin, LOW );      //set Low to bleed
    else
        digitalWrite( bleedPin, HIGH );     //set high to close bleed
}
