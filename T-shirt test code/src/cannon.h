#include <Arduino.h>
#include <Servo.h>
#include <..\lib\controller\Controller.h>

#ifndef CANNON__H
#define CANNON__H


class Cannon{
public:

    Cannon(int pin);
    void cannonControll();
    void setAngle(double input);
    void changeAngle(double input);
    void fire();



private:

    int relayPin = 12;

    bool canFire = false;
    static double angle = 90;
    double lowerPSI = 40;
    double upperPSI = 150;
    Servo cannon;

    int getPSI();
    bool checkSafe();
}

#endif CANNON__H