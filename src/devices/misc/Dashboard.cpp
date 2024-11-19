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
    attachedCANBus->attach(this, 0x310, 0x000, false);
    tickHandler.attach(this, DashboardTickInt);
}

/*For all multibyte integers the format is MSB first, LSB last
*/
void DashboardDevice::handleCanFrame(const CAN_message_t &frame) {
    if(Logger::isDebug()){
        Logger::debug("dash Test id=%X len=%X data=%X,%X,%X,%X,%X,%X,%X,%X",
                      frame.id, frame.len, 
                      frame.buf[0], frame.buf[1], frame.buf[2], frame.buf[3],
                      frame.buf[4], frame.buf[5], frame.buf[6], frame.buf[7]);
    }
    if(frame.id == 0x310){
        speed = frame.buf[0];
    }else if(frame.id == 0x311){
        temp = frame.buf[0];
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

    var.len = 4; // changed to 
    var.id = 0x203;
    
    var.buf[0] = speed;
    var.buf[1] = temp;
    var.buf[2] = 0x10;
    attachedCANBus->sendFrame(var);
    
    // var.buf[0] = 0x51;
    // // 0x04 to DISABLE
    // //var.buf[1] = 0x04;
    // // 0x00 to ENABLE
    // var.buf[1] = 0x00;
    // var.buf[2] = 0x00;
    // attachedCANBus->sendFrame(var);
    
    // // send 5% speed
    // var.buf[0] = 0x31;
    // var.buf[1] = 0x66;
    // var.buf[2] = 0x06;
    // attachedCANBus->sendFrame(var);
}
//testDevice test_device;
DashboardDevice dash_device;
