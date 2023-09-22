#ifndef testDevice_H_
#define testDevice_H_

#include <Arduino.h>
#include "../Device.h"
#include "../../DeviceManager.h"
#include "../../CanHandler.h"

#define testDeviceID 0x321 
#define testDeviceTickInt 1000000

class testDevice: public Device, CanObserver{
public:
    testDevice(); //called nearly immediately to initialize your own variables
    void setup(); //called only if the device is actually enabled
    void earlyInit(); //called early and whether or not the device is enabled. Just used to setup configuration
    void handleTick(); 
    void handleCanFrame(const CAN_message_t &frame);
    DeviceId getId();
    DeviceType getType();
private:
    CAN_message_t var;

};
#endif
