#include "cannon.h"

Cannon::Cannon(int _pin)
{
    pin = _pin;
}

void Cannon::init()
{
    cannon.attach(pin);
}

void Cannon::setAngle(double input)
{
    angle = input;
    
    if(angle > 90)
        angle = 90;

    if(angle < 35)
        angle = 35;
      
    cannon.write(angle);
}

void Cannon::changeAngle(double input)
{
    setAngle(angle -= input);
}

bool Cannon::checkSafe()
{
    int PSI = getPSI();

    if( PSI > lowerPSI && PSI < upperPSI)
        canFire = true;
    else
        canFire = false;
    
    return canFire;
}

void Cannon::fire()
{
    checkSafe();
    
    if( canFire )
        digitalWrite( relayPin, HIGH );
    else
        digitalWrite( relayPin, LOW );
}

int Cannon::getPSI()
{
    return ( analogRead(A0) * (5.0 / 1023) ) * (200 / 5.0);
}