#include "testDevice.h"

testDevice::testDevice() : Device() {
    commonName = "testDevice";
    shortName = "test";
}

void testDevice::earlyInit()
{
    prefsHandler = new PrefHandler(testDeviceID);
}

void testDevice::setup() {
    tickHandler.detach(this);
    Logger::info("add device: testDevice (id: %X, %X)", testDeviceID, this);


    Device::setup(); // run the parent class version of this function

    setAttachedCANBus(0);

    //Relevant BMS messages are 0x300 - 0x30F
    attachedCANBus->attach(this, 0x200, 0x7f0, false);
    tickHandler.attach(this, testDeviceTickInt);
}

/*For all multibyte integers the format is MSB first, LSB last
*/
void testDevice::handleCanFrame(const CAN_message_t &frame) {
    Logger::info("Test id=%X len=%X data=%X,%X,%X,%X,%X,%X,%X,%X",
                      frame.id, frame.len, 
                      frame.buf[0], frame.buf[1], frame.buf[2], frame.buf[3],
                      frame.buf[4], frame.buf[5], frame.buf[6], frame.buf[7]);
}

DeviceId testDevice::getId() {
    return (testDeviceID);
}

DeviceType testDevice::getType() {
    return (DEVICE_MISC);
}

void testDevice::handleTick()
{
    var.len = 3;
    var.id = 0x201;
    var.buf[0] = 0x51;
    // 0x04 to DISABLE
    var.buf[1] = 0x04;
    // 0x00 to ENABLE
    //var.buf[1] = 0x00;
    var.buf[2] = 0x00;
    attachedCANBus->sendFrame(var);
    
    // send 5% speed
    var.buf[0] = 0x31;
    var.buf[1] = 0x66;
    var.buf[2] = 0x06;
    attachedCANBus->sendFrame(var);


}

testDevice test_device;
