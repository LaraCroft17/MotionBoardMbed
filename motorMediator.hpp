#ifndef MEDIATOR
#define MEDIATOR

#include "mbed.h"

class motorMediator {
private:
    Ticker tick;
    tickerFunction();


    //implementing a singleton class  
    static motorMediator*
         
    // static pointer which will points  to the instance of this class
    static motorMediator* instancePtr; 
    
    // Default constructor
    motorMediator() 
    {
    }

public:
    // deleting copy constructor
    motorMediator(const motorMediator& obj) = delete; 

    static motorMediator* getInstance();

    void addControlledMotor(motorDriver* newMotor); 

};

#endif