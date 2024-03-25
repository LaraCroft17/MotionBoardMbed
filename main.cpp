
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
float location; 
char direction;

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

    if (msg.id == RX_ID) {
        // extract data from the received CAN message 
        // in the same order as it was added on the transmitter side

        //idk why these not work, i added the CANMsg.h for this
        //msg.data >> direction;
        //msg.data >> location;   
        direction = msg.data[0];
        memcpy(&location, &(msg.data[1]), sizeof(float));
 
        printf("direction = %c\n", direction);
        printf("location = %f\r\n", location);
    }

}


int main()
{
    //LIMIT switches
    DigitalIn limitX (PIN_SW_X);
    DigitalIn limitY (PIN_SW_Y1);
    DigitalIn limitZ (PIN_SW_Y2);
    
    //this board receives communication, doesn't send 
            
    can1.frequency(1000000); // set CAN bit rate to 1Mbps
    can1.filter(RX_ID, 0xFFF, CANStandard, 0); // set filter #0 to accept only standard messages with ID == RX_ID
    can1.attach(canMsgReceived);                // handler for receiving messages

    //if the attached function doesn't work
    // while(1) {
    //     if(can1.read(msg)) {          // if message is available, read into msg
    //         printf("Message received: %d\n", msg.data[0]);  
    //     } 
    //     wait_us(200);
    // }

    //test motor drivers?
    motorDriver motor1(PIN_DIR_1, PIN_ENA_1, PIN_PUL_1, motorDriver::PULSE_200_STEP_1); 
    motorDriver motor2(PIN_DIR_2, PIN_ENA_2, PIN_PUL_2, motorDriver::PULSE_200_STEP_1); 
    motorDriver motor3(PIN_DIR_3, PIN_ENA_3, PIN_PUL_3, motorDriver::PULSE_200_STEP_1); 

    //level conversion enable pin
    // Pull OE low to place all outputs in Tri-state mode/high impedence Referenced to VCCA
    DigitalOut outputEnable (PIN_OE);

    //ensure high impedence during power up or power down
    outputEnable = 0; 

    //set the pulse
    //TEST PID
    float kp = 0.1;
    float ki = 0.1;
    float kd = 0.05;
    PIDController motionControl(kp, ki, kd); // Example gains

    double reachPoint = 100.0;
    double measuredPoint = 80.0;

    //test 
    for (int i = 0; i < 50; ++i) {
        double controlSignal = motionControl.calculateControlSignal(reachPoint, measuredPoint);
        measuredPoint += controlSignal;
        printf("control signal:%f \n", controlSignal);
    }



}
