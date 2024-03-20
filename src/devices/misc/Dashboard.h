#ifndef Dashboard_H_
#define Dashboard_H_

#include <Arduino.h>
#include "../Device.h"
#include "../../DeviceManager.h"
#include "../../CanHandler.h"


#define DashboardID 0x521
#define DashboardTickInt 1000000

class DashboardDevice: public Device, CanObserver{
public:
    DashboardDevice(); //called nearly immediately to initialize your own variables
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
