/*
 * BamocarMotorController.cpp
 */

#include "BamocarMotorController.h"
#include <sstream>

BamocarMotorController::BamocarMotorController() : MotorController() {    
    selectedGear = DRIVE;
    commonName = "Bamocar Inverter";
    shortName = "BamocarInverter";
}

void BamocarMotorController::earlyInit()
{
    prefsHandler = new PrefHandler(BAMOCARINVERTER);
}

void BamocarMotorController::setup() {
    tickHandler.detach(this);

    Logger::info("add device: Bamocar Inverter (id:%X, %X)", BAMOCARINVERTER, this);

    loadConfiguration();
    MotorController::setup(); // run the parent class version of this function

    running = true;
    setPowerMode(modeTorque);
    setSelectedGear(DRIVE);
    setOpState(ENABLE);

    tickHandler.attach(this, CFG_TICK_INTERVAL_MOTOR_CONTROLLER_BAMOCAR);
}

void BamocarMotorController::handleTick() {
    BamocarMotorControllerConfiguration *config = (BamocarMotorControllerConfiguration *)getConfiguration();
    
    MotorController::handleTick();
    int percent = torqueRequested; 

    //rounding to nearest 10th
    int a = (percent/10)*10;
    int b = a + 10;
    percent = (percent - a > b - percent)? b : a;
    int input = percent*327;

    uint32_t firsthalf = (input & 0xFF);
    uint32_t secondhalf = ((input >> 8) & 0xFF);


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
    var.buf[1] = secondhalf;
    var.buf[2] = firsthalf;
    attachedCANBus->sendFrame(var);
}

void BamocarMotorController::handleCanFrame(const CAN_message_t &frame) {
    
}

void BamocarMotorController::setGear(Gears gear) {
    selectedGear = gear;
    //if the gear was just set to drive or reverse and the DMOC is not currently in enabled
    //op state then ask for it by name
    if (selectedGear != NEUTRAL) {
        operationState = ENABLE;
    }
    //should it be set to standby when selecting neutral? I don't know. Doing that prevents regen
    //when in neutral and I don't think people will like that.
}

DeviceId BamocarMotorController::getId() {
    return (BAMOCARINVERTER);
}

uint32_t BamocarMotorController::getTickInterval()
{
    return CFG_TICK_INTERVAL_MOTOR_CONTROLLER_BAMOCAR;
}

void BamocarMotorController::loadConfiguration() {
    BamocarMotorControllerConfiguration *config = (BamocarMotorControllerConfiguration *)getConfiguration();

    if (!config) {
        config = new BamocarMotorControllerConfiguration();
        setConfiguration(config);
    }

    MotorController::loadConfiguration(); // call parent
}

void BamocarMotorController::saveConfiguration() {
    MotorController::saveConfiguration();
}

BamocarMotorController bamocarMC;