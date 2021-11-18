#ifndef CANNON__H
#define CANNON__H

#include <Arduino.h>
#include <Servo.h>
#include <controller.h>

class Cannon{
    
private:
    int pin;
    int relayPin = 12;
    bool canFire = false;
    double angle = 90;
    double lowerPSI = 40;
    double upperPSI = 150;
    Servo cannon;

    int getPSI();
    bool checkSafe();

public:
    Cannon(int pin);
    void init();
    void cannonControll();
    void setAngle(double input);
    void changeAngle(double input);
    void fire();

};

#endif