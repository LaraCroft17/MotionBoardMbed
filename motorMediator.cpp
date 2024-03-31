#include "motorMediator.hpp"

motorMediator::motorMediator() { 
    tick.attach(callback(&tickerFunction , &motorMediator::tickerFunction), 2.0) //fix the interval later 

}


motorMediator* motorMediator::instancePtr = nullptr;

motorMediator* motorMediator::getInstance() {
    // If there is no instance of class then we can create an instance.
    if (instancePtr == NULL) 
    {
    // We can access private members  within the class.
    instancePtr = new motorMediator(); 
    // returning the instance pointer
    return instancePtr; 
    }
    else
    {
    // if instancePtr != NULL that means the class already have an instance. 
    // So, we are returning that instance and not creating new one.
    return instancePtr;
    }
}

void addControlledMotor(motorDriver* newMotor) {

    //idk?
}