#include "motorDriver.hpp"


motorDriver::motorDriver(PinName DIR, PinName ENA, PinName PUL, bool invertDirection): direction(DIR), enable(ENA), step(PUL), invertDirection(invertDirection) { 
    enable = 0; //set enable high initially 
    maxStepCount = 1000000;
    steps = 500000;

    //operation is in 16 microstep
}

void motorDriver::stepTicker() {

    if (steps < 0 || steps >= maxStepCount) {
        return;
    }

    step = !step;
    steps += stepDirection;
}

void motorDriver::setSpeed(float speed) {
    direction = (speed > 0) ^ invertDirection;   // positive speed: 1 ^ true = 0, negative speed: 0 ^ true = 1
    printf("direction: %d\n", direction.read());

    //check the limits 
    if(speed > 1) {
        speed = 1;
    } else if (speed < -1) {
        speed = -1;
    }

    if (speed > 0) {
        stepDirection = 1;
    } else {
        stepDirection = -1;
    }
    
    t.detach();

    // for our motors, 500rpm is max recommended and 200 steps per revolution.
    // so, min delay between steps is 0.6ms between steps, but half this is needed
    // for toggling step signal.

    if (abs(speed) > 0.00001) { //making sure the function doesn't return inf
        t.attach_us(callback(this, &motorDriver::stepTicker), abs(80/speed));
    } 
}

int motorDriver::getPosition() {
    return steps;
}

void motorDriver::resetPosition(int maxStepCount) {
    steps = 0;
    this->maxStepCount = maxStepCount * 2; // x2 for the positive and negative edge of the output signal.
}

void motorDriver::takeStep(int stepCount, float speed) {
    //check the limits 
    if(speed > 1) {
        speed = 1;
    } else if (speed < -1) {
        speed = -1;
    }

    if (speed > 0) {
        stepDirection = 1;
    } else {
        stepDirection = -1;
    }
    
    int delay = abs(80/speed);  
    direction = (speed > 0) ^ invertDirection;   // positive speed: 1 ^ true = 0, negative speed: 0 ^ true = 1
    //direction = stepDirection;
    //make only one step given the delay by the user    
    for (int i = 0; i < stepCount; i++) {
        step = 1;
        wait_us(delay);
        step = 0;
        wait_us(delay);
        steps += stepDirection;
        if (steps < 0 || steps >= maxStepCount) {
            return;
        }
    }
}

void motorDriver::testMultipleRounds(int count, float stepCount, float speed) {
    //check limits of speed
    //check the limits 
    if(speed > 1) {
        speed = 1;
    } else if (speed < -1) {
        speed = -1;
    } 
    //int delay = abs(80/speed);
    int i, j;
    for (i = 0; i < count; i++) { //how many times motor will go and come back 
        //go to right 
        for (j = 0; j < stepCount; j++) {
            takeStep(stepCount, speed);
        }
        wait_us(1000);
        //go to left
        for (j = 0; j < stepCount; j++) {
            takeStep(stepCount, speed);
        }
        printf("Round %d completed\n", i);
    }

    printf("Test mutltiple rounds completed");
    setSpeed(0); //just in case 
}