#ifndef MOTOR_DRIVER
#define MOTOR_DRIVER

#include "mbed.h"

class motorDriver {
private:
    float pulsePerRev;
    DigitalOut direction;
    DigitalOut enable;
    DigitalOut step;

public:
    enum pulseRev {
        PULSE_NC_STEP_NC,
        PULSE_200_STEP_1,
        PULSE_400_STEP_2A,
        PULSE_400_STEP_2B,
        PULSE_800_STEP_4,
        PULSE_1600_STEP_8,
        PULSE_3200_STEP_16,
        PULSE_6400_STEP_32
    };

    motorDriver(PinName DIR, PinName ENA, PinName PUL, pulseRev pulseValue); // set the pulse/rev

    void setDirection(bool dir); //1 for clockwise 0 for counterclockwise

    void oneStep(int delay_us); //only one step? is this useful?
    //after testing this it can be made private function?

    void oneRev (int delay_us); //one full revolution 
    //after testing this it can be made private function?

    void run(int revCount, int delay_us); //move x amount of steps 
};

#endif 