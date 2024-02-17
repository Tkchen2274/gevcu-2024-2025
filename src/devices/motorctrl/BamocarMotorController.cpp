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

    setAttachedCANBus(0);
    //Relevant BMS messages are 0x300 - 0x30F
    attachedCANBus->attach(this, 0x200, 0x7f0, false);
    
    tickHandler.attach(this, CFG_TICK_INTERVAL_MOTOR_CONTROLLER_BAMOCAR);
}

void BamocarMotorController::handleTick() {
    BamocarMotorControllerConfiguration *config = (BamocarMotorControllerConfiguration *)getConfiguration();
    //if the brake is pressed beyond a certain point set the speed back down to 0
    //if the brake is pressed also send a speed signal but lower than the current speed
    //if the throttle is released a little then the speed should still be similar, not immediately down to 0
    //max press accelerate max 
    //make it as close to a regular car
    //brakes regular car as well
    //willl never really have a cruising speed
    //linear
    //max push max acceleration

    MotorController::handleTick();
    if (throttleRequested < 0) throttleRequested = 0;
    if (throttleRequested > 1000 && throttleRequested < 1300) throttleRequested = 1000;
    if (throttleRequested > 1400) throttleRequested = 0;

    

    // //rounding to nearest 10th
    int a = (throttleRequested/10);
    a = a*100;
    uint32_t firsthalf = (a & 0xFF);
    uint32_t secondhalf = ((a >> 8) & 0xFF);
    
    //Logger::warn("First half %i | Second half %i", firsthalf, secondhalf);
    // check if the motor will still spin even if the pedal is released all the way up

    var.len = 3;
    var.id = 0x201;

    // var.buf[0] = 0x51;
    // // 0x04 to DISABLE
    // var.buf[1] = 0x04;
    // // 0x00 to ENABLE
    // //var.buf[1] = 0x00;
    // var.buf[2] = 0x00;
    // attachedCANBus->sendFrame(var);

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