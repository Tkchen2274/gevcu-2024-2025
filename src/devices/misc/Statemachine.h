#ifndef Statemachine_H_
#define Statemachine_H_

#include <Arduino.h>
#include "../Device.h"
#include "../../DeviceManager.h"
#include "../../CanHandler.h"


#define StatemachineID 0x001
#define StatemachineTickInt 1000000

class StatemachineDevice: public Device, CanObserver{
public:
    StatemachineDevice(); //called nearly immediately to initialize your own variables
    void setup(); //called only if the device is actually enabled
    void earlyInit(); //called early and whether or not the device is enabled. Just used to setup configuration
    void handleTick(); 
    void handleCanFrame(const CAN_message_t &frame);
    DeviceId getId();
    DeviceType getType();
private:
    CAN_message_t var;
    int temp = 0;
    int speed = 0;
};
#endif
