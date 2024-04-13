#ifndef PID_CONTROLLER
#define PID_CONTROLLER

#include "mbed.h"


class PIDController {
private:
    float Kp, Ki, Kd;
    float prevError, error, integral, derivative;
    Timer t;
    float prevTime, currTime, deltaTime;

public:
    PIDController(float p, float i, float d):Kp(p), Ki(i), Kd(d), prevError(0), integral(0), prevTime(0), derivative(0) {
        t.start();
    }

    /*
    reach = 0 
    currValue = difference of position between where carriage is and where appliance is 
    */
    
    float calculateControlSignal(float reach, float currValue) {
        t.stop();
        currTime = t.read(); //in seconds
        t.start();
        deltaTime = currTime - prevTime;
        prevTime = currTime;
 
        error =  reach - currValue;
        derivative = (error - prevError) / deltaTime; // distance / delta time 
        integral += (error * deltaTime); 

        prevError = error;

        return (Kp * error) + (Ki * integral) + (Kd * derivative);

        // error = reach - currValue;
        // integral += error; //sum 
        // float derivative = error - prevError; //difference
        // prevError = error;


        
    }
};

#endif