#include "motorDriver.hpp"


motorDriver::motorDriver(PinName DIR, PinName ENA, PinName PUL, bool invertDirection): direction(DIR), enable(ENA), step(PUL), invertDirection(invertDirection) { 
    maxStepCount = 1000000;
    steps = 500000;
}

void motorDriver::stepTicker() {
    if (steps <= 0 || steps >= maxStepCount) {
        return;
    }

    step = !step;
    steps += stepDirection;
}

void motorDriver::setSpeed(float speed) {
    if ((speed > 0) != (direction ^ invertDirection)) {
        direction = (speed > 0) ^ invertDirection;
    }
    //check the limits 
    if(speed > 1) {
        speed = 1;
        stepDirection = 1;
    } else if (speed < -1) {
        speed = -1;
        stepDirection = -1;
    }
    
    t.detach();

    // for our motors, 500rpm is max recommended and 200 steps per revolution.
    // so, min delay between steps is 0.6ms between steps, but half this is needed
    // for toggling step signal.

    if (abs(speed) > 0.01) { //making sure the function doesn't return inf
        t.attach_us(callback(this, &motorDriver::stepTicker), abs(300/speed));
    } 
}

int motorDriver::getPosition() {
    return steps;
}

void motorDriver::resetPosition(int maxStepCount) {
    steps = 0;
    this->maxStepCount = maxStepCount;
}