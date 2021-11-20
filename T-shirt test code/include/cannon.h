#ifndef CANNON__H
#define CANNON__H

#include <Arduino.h>
#include <Servo.h>
#include <controller.h>

class Cannon{
    
private:
    int pin;                //for cannon motor
    int relayPin;      //for firing
    int fillPin;            //for ballast fill solenoid
    int bleedPin;           //for ballast bleed solenoid
    double PSI;
    int desiredPSI = 100;
    bool canFire = false;
    double angle = 90;
    double lowerPSI = 40;
    double upperPSI = 150;
    Servo cannon;

    bool checkSafe();

public:
    Cannon(int _pin, int fpin, int bpin, int rpin);
    bool fired;
    double getPSI();
    void init();
    void cannonControll();
    void setAngle(double input);
    void changeAngle(double input);
    bool barrelOpen();
    void barrelClose();
    bool ballastFill();    
    void ballastBleed();

};

#endif