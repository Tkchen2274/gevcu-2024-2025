#include "Dashboard.h"

// #include "BamocarMotorController.h"

DashboardDevice::DashboardDevice():Device() {
    commonName = "Dashboard";
    shortName = "Dash";
}

void DashboardDevice::earlyInit()
{
    prefsHandler = new PrefHandler(DashboardID);
}

void DashboardDevice::setup() {
    tickHandler.detach(this);
    Logger::info("add device: DashboardDevice (id: %X, %X)", DashboardID, this);
    Device::setup(); // run the parent class version of this function
    setAttachedCANBus(0);
    //Relevant BMS messages are 0x300 - 0x30F
    attachedCANBus->attach(this, 0x190, 0xfff, false);
    tickHandler.attach(this, DashboardTickInt);
    speed = 0;
    bamocar_temp = 0;
    motor_temp = 0;
    battery = 0;
}

/*For all multibyte integers the format is MSB first, LSB last
*/
void DashboardDevice::handleCanFrame(const CAN_message_t &frame) {
    switch (frame[0]){
        //motor speed
        case (0x30):
            speed = decode_hex(frame[1], frame[2]);
            break;
        case (0x)
    }
}

DeviceId DashboardDevice::getId() {
    return (DashboardID);
}

DeviceType DashboardDevice::getType() {
    return (DEVICE_MISC);
}

void DashboardDevice::handleTick()
{
    var.len = 4; // changed to 4, max space for classes
    var.id = 0x203;
    
    var.buf[0] = speed;
    var.buf[1] = temp;
    var.buf[2] = battery;
    var.buf[3] = somethingElse;
    attachedCANBus->sendFrame(var);
    
}
//testDevice test_device;
DashboardDevice dash_device;
