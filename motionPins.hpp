#ifndef PINS_INCLUDED
#define PINS_INCLUDED

#undef __ARM_FP
#include <mbed.h>

//limit switches 
//goes to header J4
#define PIN_SW_X p18
#define PIN_SW_Y1 p19
#define PIN_SW_Y2 p20

//motor driver signals 
//https://www.omc-stepperonline.com/digital-stepper-driver-1-0-4-2a-20-50vdc-for-nema-17-23-24-stepper-motor-dm542t?language=en&currency=USD&gad_source=1&gclid=CjwKCAiAivGuBhBEEiwAWiFmYdR1WoTheA0wUPmKfKyZ0-7841H2cseZyz5g4IijemTSOw3-G_ua-RoC5zcQAvD_BwE

//1 and 2 goes to U1 for level conversion

//J1
#define PIN_PUL_1 p25 
#define PIN_DIR_1 p24
#define PIN_ENA_1 p23

//J2
#define PIN_PUL_2 p8 
#define PIN_DIR_2 p9
#define PIN_ENA_2 p10

//goes to U2 for level conversion
//J3 
#define PIN_PUL_3 p28 
#define PIN_DIR_3 p27
#define PIN_ENA_3 p26

//goes to both
#define PIN_OE p7

//CAN between motion board and wireless power board
//connected with MCP2551-1/SN

#define PIN_CAN_RX_1 p30
#define PIN_CAN_TX_1 p29

#endif // PINS_INCLUDED