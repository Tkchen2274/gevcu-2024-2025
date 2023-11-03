#ifndef CANRecieveTest_H_
#define CANRecieveTest_H_

#include <Arduino.h>
#include "../../config.h"
#include "../Device.h"
#include "../../DeviceManager.h"
#include "../../CanHandler.h"

#define CANRecieveTestID 0x1234

class CANRecieveTestConfiguration: DeviceConfiguration
{
public:
    uint8_t canbusNum;
};


class CANRecieveTest : public Device, CanObserver
{
public:
    CANRecieveTest();
    void setup();
    void earlyInit();
    void handleCanFrame(const CAN_message_t &frame);
    DeviceId getId();
    DeviceType getType();

protected:
private:
};

#endif
