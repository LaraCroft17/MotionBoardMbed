#ifndef PID_CONTROLLER
#define PID_CONTROLLER

class PIDController {
private:
    float Kp, Ki, Kd;
    float prevError, integral;

public:
    PIDController(float p, float i, float d):Kp(p), Ki(i), Kd(d), prevError(0), integral(0) {}
    
    double calculateControlSignal(float reach, float currValue) {
        float error = reach - currValue;
        integral += error; //sum 
        float derivative = error - prevError; //difference
        prevError = error;
        return (Kp * error) + (Ki * integral) + (Kd * derivative);
    }
};

#endif