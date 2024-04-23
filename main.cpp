
#include "mbed.h"
#include "motionPins.hpp"
#include "pidControl.hpp"
#include "motorDriver.hpp"
#include "CANMsg.h"
#include <cstdio>


// const unsigned int  RX_ID = 0x100;
// const unsigned int  TX_ID = 0x101;
 

/**
* Most basic test to verify the functionality of the motors and motor drivers
* Not used later in the actual program
* Select 4 microstep and 2A Current on the motor driver 
*/
void simpleMotorTest() {
    DigitalOut outputEnable (PIN_OE);
    outputEnable = 1;

    DigitalOut enable (PIN_ENA_3);
    enable = 0;
    DigitalOut direction (PIN_DIR_3);
    direction = 0; 
    DigitalOut pull (PIN_PUL_3);
    pull = 1;

    DigitalIn limitSwitch (PIN_SW_X);
    limitSwitch.mode(PullUp);

    while(limitSwitch) {
        pull = 0;
        wait_us(1000);

        pull = 1;
        wait_us(1000);
    }
    return;
}

/**
* First calibration method to move the carriage towards one end and to reset the position
* Then the carriage will go in the opposite dir until the end
* Using the fact that only ONE side has a limit switch 
* @param ptr to motorDriver class, x-axis limit switch 
*/
void calibrateX (motorDriver* motor, DigitalIn* limitSwitch) {
    motor->setSpeed(-0.15);
    printf("motor running \n");
    while(limitSwitch->read()) {

    }
    motor->setSpeed(0);
    printf("motor at right edge \n");    
    motor->resetPosition(35000); // usually 39500 the limit
}

/**
* Second calibration method to move the bigger carriage in the y-direction with two motors 
* Test both ways, stop both motors if one hits it's limit switch 
* Using the fact that only ONE side has a limit switch 
* @param pointers to both motorDriver classes and both y-axis limit switches
*/
void calibrateY (motorDriver* motor1, motorDriver* motor2, DigitalIn* limitY1, DigitalIn* limitY2) {
    motor1->setSpeed(-0.15);
    motor2->setSpeed(-0.15);

    while(limitY2->read()) {
        //if either limit switch is pressed the big carriage hits to side so stop both motors 
        if (!limitY2->read()) {
            motor1->setSpeed(0);
            printf("motor 1 stopped!!\n");

        }
        if (!limitY2->read()) {
            motor2->setSpeed(0);
            //printf("motor 2 stopped\n");

        }
    }

    motor1->setSpeed(0);
    motor2->setSpeed(0);
    
    printf("both motors stopped\n");

    //check the differences in step count in both
    // int stepsDifference = motor1->getPosition() - motor2->getPosition();
    // printf("Steps difference: %d\n", stepsDifference);

    // if (stepsDifference > 0) {
    //     //motor 1 went farther than motor2 
    //     motor2->takeStep(abs(stepsDifference), 0.10);
    // } else if (stepsDifference < 0) {
    //     motor1->takeStep(abs(stepsDifference), 0.10);
    // }

    // motor1->resetPosition(1000); //test this 1000 number
    // motor2->resetPosition(1000); // 300mm

    // printf("going to other direction \n");
    // motor1->setSpeed(-0.15);
    // motor2->setSpeed(-0.15);
    // wait_us(500000); //half sec for the limit switches to be de-pressed 
}

int main()
{
    printf("running\n");
    // Pull OE low to place all outputs in Tri-state mode/high impedence Referenced to VCCA
    // ensure high impedence during power up or power down
    // DigitalOut outputEnable (PIN_OE);    //proved not to be useful 
    // outputEnable = 1; 


    /* After the final installation - check 
    motor 1 -> y-axis not next to motion board 
    motor 2 -> x-axis 
    motor 3 -> y-axis next to motion board 
    switchX on pcb -> limit switch on x-axis
    switchY1 -> y-axis, the one next to the motion PCB 
    switchY2 -> y-axis the one not next to motion PCB 
    */

    // init limit switches 
    DigitalIn limitX (PIN_SW_X);
    limitX.mode(PullUp);
    wait_us(1000);
    DigitalIn limitY1 (PIN_SW_Y1);
    limitY1.mode(PullUp);
    wait_us(1000);
    DigitalIn limitY2 (PIN_SW_Y2);
    limitY2.mode(PullUp);
    wait_us(1000);

    //calibrate x-direction
    motorDriver motorx(PIN_DIR_2, PIN_ENA_2, PIN_PUL_2, true); 
    calibrateX(&motorx, &limitX);
    printf("X-axis calibration done\n");

    // motorx.takeStep(20000, 0.15);
    // printf("steps to left \n");

    // int i;
    // for (i = 0; i < 10; i++) {
    //     motorx.takeStep(10000, -0.5);
    //     printf("steps to right \n");
    //     motorx.takeStep(10000, 0.5);
    //     printf("steps to left \n");
    //     printf("round %d complete\n", i);
    // }



    // y direction motors 
    motorDriver motory1(PIN_DIR_1, PIN_ENA_1, PIN_PUL_1, false); 
    motorDriver motory2(PIN_DIR_3, PIN_ENA_3, PIN_PUL_3, true); 

    calibrateY(&motory1, &motory2, &limitY2, &limitY1);

    // for (int i = 0; i < 5; i++) {
    //     motory1.setSpeed(0.15);
    //     motory2.setSpeed(0.15);
    //     wait_us(5000000);
    //     calibrateY(&motory1, &motory2, &limitY2, &limitY1);
     
    // }
    printf("Y calibration done \n");

    //After the calibration suite the system is ready to receive communication 
    //this board doesn't send info; it only receives 

    //initialize CAN now      
    CAN can1(PIN_CAN_RX_1, PIN_CAN_TX_1);               // CAN interface
    can1.frequency(100000); // set CAN bit rate to 1Mbps
    
    //message includes these two variables 
    int location; 
    char direction;



    // //PID CONTROL variables 
    // float kpx = 0.1;
    // float kix = 0.1;
    // float kdx = 0.05;     //CHANGE THESE DEPENDING ON TESTS 
    // PIDController controlXaxis (kpx, kix, kdx); // Example gains
    // float kpy = 0.1;
    // float kiy = 0.1;
    // float kdy = 0.05;     //CHANGE THESE DEPENDING ON TESTS 
    // PIDController controlYaxis (kpy, kiy, kdy); // Example gains

    printf("waitinf for CAN shit\n");
    while(1) {
        // block the while loop until a can message is sent from the other board
        CANMessage msg; 
        if (can1.read(msg)) {
            printf("Message received\n");

            //printing message details - delete if not necessary
            
            // extract data from the received CAN message 
            // in the same order as it was added on the transmitter side
            //msg.data >> direction;
            //msg.data >> location;   
            float x;
            float y;
            memcpy(&x, &(msg.data[0]), sizeof(float));
            memcpy(&y, &(msg.data[4]), sizeof(float));
            printf("x: %d, y: %d\n", (int)(x * 10), (int)(y * 10));


            if (x > 1) {
                motorx.setSpeed(-0.5);
            } else if (x < -1) {
                motorx.setSpeed(0.5);
            } else {
                motorx.setSpeed(0);
            }

            if (y > 1) {
                motory1.setSpeed(0.5);
                motory2.setSpeed(0.5);
            } else if (y < -1) {
                motory1.setSpeed(-0.5);
                motory2.setSpeed(-0.5);
            } else {
                motory1.setSpeed(0);
                motory2.setSpeed(0);
            }
        } 


    }
    //can1.filter(RX_ID, 0xFFF, CANStandard, 0); // set filter #0 to accept only standard messages with ID == RX_ID

    //if the attached function doesn't work
    // while(1) {
    //     if(can1.read(msg)) {          // if message is available, read into msg
    //         printf("Message received: %d\n", msg.data[0]);  
    //     } 
    //     wait_us(200);
    // }



    while(1) {
        //msg.id = RX_ID
        // 
        // int location = 0x1234;
        // msg.data[0] = 'x'; // direction
        // memcpy(&(msg.data[1]), &location, sizeof(int));

        // if(can1.write(msg)) {       // transmit message

        //     printf("CAN message sent\n");

        //     printf("direction = %c\n", msg.data[0]);
        //     //printf("location = %d V\r\n", location);
        // } else {
        //     //printf("sad cat :( \n");
        // }
        wait_us(100000);
    }

    //set the pulse
    //TEST PID

    // float reachPoint = 100.0;
    // float measuredPoint = 80.0;

    // //test 
    // for (int i = 0; i < 50; ++i) {
    //     float controlSignal = motionControl.calculateControlSignal(reachPoint, measuredPoint);
    //     measuredPoint += controlSignal;
    //     printf("control signal:%f \n", controlSignal);
    // }

    //Automatically setting the direction of the motor controllers based ont he return value from the PID controller 



}
