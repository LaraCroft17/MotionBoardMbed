
#include "mbed.h"
#include "motionPins.hpp"
#include "pidControl.hpp"
#include "motorDriver.hpp"
#include "CANMsg.h"
#include <cstdio>


const unsigned int  RX_ID = 0x100;
const unsigned int  TX_ID = 0x101;
 
CAN can1(PIN_CAN_RX_1, PIN_CAN_TX_1);               // CAN interface
  
CANMessage msg;

int location; 
char direction;


/**
CAN message receving pr
*/
void canMsgReceived() {
    can1.read(msg);
    printf("CAN message received: \n");

    //PRINTING MESSAGE DETAILS 
    printf("ID = 0x%.3x\n", msg.id);
    printf("Type = %d\n", msg.type);
    printf("Format = %d\n", msg.format);
    printf("Length = %d\n", msg.len);
    printf("Data =");
    for(int i = 0; i < msg.len; i++)
        printf(" 0x%.2X", msg.data[i]);
    printf("\r\n");
    //MESSAGE DETAILS finished

    if (msg.id == TX_ID) {
        // extract data from the received CAN message 
        // in the same order as it was added on the transmitter side

        //idk why these not work, i added the CANMsg.h for this
        //msg.data >> direction;
        //msg.data >> location;   
        direction = msg.data[0];
        memcpy(&location, &(msg.data[1]), sizeof(int));
 
        printf("sent direction = %c\n", direction);
        printf("sent location = %f\r\n", location);
    }

}



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
    motor->resetPosition(39500); // 300mm
    motor->setSpeed(0.25);
    wait_us(500000); //half sec to the limit switch to de-pressed by the carriage 
    printf("motor going left \n");
    // while(limitSwitch->read()) { //for initial test of this calibration routine only

    // }
    // motor->setSpeed(0);
}

/**
* Second calibration method to move the bigger carriage in the y-direction with two motors 
* Test both ways, stop both motors if one hits it's limit switch 
* Using the fact that only ONE side has a limit switch 
* @param pointers to both motorDriver classes and both y-axis limit switches
*/
void calibrateY (motorDriver* motor1, motorDriver* motor2, DigitalIn* limitY1, DigitalIn* limitY2) {
    motor1->setSpeed(0.15);
    motor2->setSpeed(0.15);

    while(limitY1->read() || limitY2->read())
        // motor2.setSpeed(0.15);
    // motor1.setSpeed(0.15);
    // while(limitY1 == 0 || limitY2 == 0) {

    // }
    // motor1.setSpeed(0);
    // motor2.setSpeed(0);
    // motor1.resetPosition(1000); // 300mm
    // motor2.resetPosition(1000); // 300mm


}

int main()
{
    // Pull OE low to place all outputs in Tri-state mode/high impedence Referenced to VCCA
    // ensure high impedence during power up or power down
    // DigitalOut outputEnable (PIN_OE);    //proved not to be useful 
    // outputEnable = 1; 

    // init limit switch X
    DigitalIn limitX (PIN_SW_X);
    limitX.mode(PullUp);
    wait_us(1000);
    
    // initialize motor for x-axis and calibrate position 
    motorDriver motor3(PIN_DIR_3, PIN_ENA_3, PIN_PUL_3, true); 
    calibrateX(&motor3, &limitX);
    printf("X-axis calibration done\n");



    //INSERT HERE THE NEW TEST
    motor3.testMultipleRounds(2, 1000, 0.3); //2 rounds of going back and forth, take 1000 steps each dir, speed 0.3 
    while(1) {

    }
    //NEW TEST FINISHES


    // init y-direction limit switches 
    DigitalIn limitY1 (PIN_SW_Y1);
    limitY1.mode(PullUp);
    wait_us(1000);
    DigitalIn limitY2 (PIN_SW_Y2);
    limitY2.mode(PullUp);
    wait_us(1000);

    // y direction motors 
    motorDriver motor1(PIN_DIR_1, PIN_ENA_1, PIN_PUL_1, true); 
    motorDriver motor2(PIN_DIR_2, PIN_ENA_2, PIN_PUL_2, true); 

    
    





    //this board receives communication, doesn't send 
            
    // can1.frequency(1000000); // set CAN bit rate to 1Mbps
    // can1.filter(RX_ID, 0xFFF, CANStandard, 0); // set filter #0 to accept only standard messages with ID == RX_ID
    // can1.attach(canMsgReceived);                // handler for receiving messages

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
    // float kp = 0.1;
    // float ki = 0.1;
    // float kd = 0.05;
    // PIDController motionControl(kp, ki, kd); // Example gains

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
