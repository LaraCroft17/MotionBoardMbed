#ifndef MOTOR_DRIVER
#define MOTOR_DRIVER

#include "mbed.h"

//motor driver class for the three motors on mechanical system 

class motorDriver {
private:
    DigitalOut direction;
    DigitalOut enable;
    DigitalOut step;
    Ticker t;
    int steps; //keeps track of position 
    int stepDirection; //helper to calculate steps
    void stepTicker();
    bool invertDirection;
    int maxStepCount;
    

public:
    motorDriver(PinName DIR, PinName ENA, PinName PUL, bool invertDirection = false); // set the pulse/rev

    void setSpeed(float speed); //between -1 and +1 

    int getPosition();

    void resetPosition(int maxStepCount);

    void testMultipleRounds(int count, float stepCount, float speed);

    void takeStep(int stepCount, float speed); //to get rid of the small step change in between the motors on the same axes 

};

#endif 