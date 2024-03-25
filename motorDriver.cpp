#include "motorDriver.hpp"

motorDriver::motorDriver(PinName DIR, PinName ENA, PinName PUL, pulseRev pulseValue): pulsePerRev(0), direction(DIR), enable(ENA), step(PUL) { 

    switch (pulseValue) {
        case PULSE_NC_STEP_NC:
            pulsePerRev = 0.0f;  
            break;
        case PULSE_200_STEP_1:
            pulsePerRev = 200.0f;
            break;
        case PULSE_400_STEP_2A:
            pulsePerRev = 400.0f;
            break;
        case PULSE_400_STEP_2B:  //any difference between a and b??
            pulsePerRev = 400.0f;
            break;
        case PULSE_800_STEP_4:
            pulsePerRev = 800.0f;
            break;
        case PULSE_1600_STEP_8:
            pulsePerRev = 1600.0f;
            break;
        case PULSE_3200_STEP_16:
            pulsePerRev = 3200.0f;
            break;
        case PULSE_6400_STEP_32:
            pulsePerRev = 6400.0f;
            break;
    }

}

void motorDriver::setDirection(bool dir) {
    //wait after PUL just in case
    wait_us(5);

    //1 for clockwise 0 for counterclockwise
    if(dir) {
        direction = 1;
    } else {
        direction = 0;
    }

}

void motorDriver::oneStep(int delay_us) {
    //make only one step given the delay by the user 
    step = 1;
    wait_us(delay_us);
    step = 0;
    wait_us(delay_us);
}

void motorDriver::oneRev (int delay_us) {
    for (int i = 0; i < pulsePerRev; i++) {
        step = 1;
        wait_us(delay_us);
        step = 0;
        wait_us(delay_us);
    }
}

void motorDriver::run(int revCount, int delay_us) {

    for (int i =0; i < revCount * pulsePerRev; i++) {
        oneStep(delay_us);
    }
}
