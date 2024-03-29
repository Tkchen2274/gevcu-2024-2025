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

    //CONFIGURATIONS FOR THE MOTORCONTROLLER
//CONFIGURATIONS FOR THE MOTORCONTROLLER
    bool once = true;

    var.len = 3;
    var.id = 0x201; 


/*
    if (once) //send once to start up 
    {
        var.len = 3;
        var.id = 0x201;
    
        //N Nom set to 5500
        var.buf[0] = 0x59;
        var.buf[2] = 0x15;
        var.buf[1] = 0x7C;
        attachedCANBus->sendFrame(var);
    
        //F NOM set to 300 - wrong - still ran with wrong numbers
        var.buf[0] = 0x05;
        var.buf[2] = 0x03;
        var.buf[1] = 0x2C;
        attachedCANBus->sendFrame(var);
        

        //Not correct - works in isolation - still ran with 0
        //V NOM set to 710

        var.buf[0] = 0x06;
        var.buf[2] = 0x02;
        var.buf[1] = 0xC6;
        attachedCANBus->sendFrame(var);
    
        //cos Phi set to 0
        var.buf[0] = 0x0e;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

     

        //Not correct - works in isolation - works 2nd time - still ran with 1000
        //I Max eff set to 250.0
        var.buf[0] = 0x4d;
        var.buf[2] = 0x00;
        var.buf[1] = 0xFA;
        attachedCANBus->sendFrame(var);

        //I nom set to 115
        var.buf[0] = 0x4e;
        var.buf[2] = 0x00;
        var.buf[1] = 0x73;
        attachedCANBus->sendFrame(var);

        //M-Pole set to 20
        var.buf[0] = 0x4f;
        var.buf[2] = 0x00;
        var.buf[1] = 0x14;
        attachedCANBus->sendFrame(var);

        // //Kt set to 0.940 which you beed to send 940
        // var.buf[0] = 0x87;
        // var.buf[2] = 0x03
        // var.buf[1] = 0xAC;
        // attachedCANBus->sendFrame(var);

        // //Ke set to 73.48 which you beed to send 940
        // var.buf[0] = 0x87L;
        // var.buf[2] = 0x03
        // var.buf[1] = 0xAC;
        // attachedCANBus->sendFrame(var);

        //Brake Delay set to 0
        var.buf[0] = 0xf1;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        //Coast Stop ??? :TODO:

        //set M-temp to 15000 - 2nd time doesnt work - 
        var.buf[0] = 0xa3;
        var.buf[2] = 0x3A;
        var.buf[1] = 0x98;
        attachedCANBus->sendFrame(var);

        //FB-Pole set to 2
        var.buf[0] = 0xa7;
        var.buf[2] = 0x00;
        var.buf[1] = 0x02;
        attachedCANBus->sendFrame(var);

        //Not correct - works in isolation - 2nd time doesnt work
        //FB-offset to 115
        var.buf[0] = 0x44;
        var.buf[2] = 0x00;
        var.buf[1] = 0x73;
        attachedCANBus->sendFrame(var);

        //FB-Incr to 2048 
        var.buf[0] = 0xa6;
        var.buf[2] = 0x08;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        //Factor-ext to 0
        var.buf[0] = 0x7e;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        //Not correct - works isolated - 2nd time does work
        //set axis to "AT"
        var.len = 5;
        var.buf[0] = 0xf8;
        var.buf[1] = 0x54;
        var.buf[2] = 0x41;
        var.buf[3] = 0x00;
        var.buf[4] = 0x00;
        attachedCANBus->sendFrame(var);

        // TODO: MAINS Type????

        //Not correct - works isolated - 2nd time doesnt work
        //Mains Voltage to 100

        var.len = 3;
        var.buf[0] = 0x64;
        var.buf[2] = 0x00;
        var.buf[1] = 0x64;
        attachedCANBus->sendFrame(var);

        //Not correct - DOES NOT WORK  IN ISOLATION - 2nd time does not work
        //DC-Bus max to 120, DC-Bus min to 6
        var.len = 5;
        var.buf[0] = 0xa5;
        var.buf[1] = 0x78;
        var.buf[2] = 0x00;
        var.buf[3] = 0x06;
        var.buf[4] = 0x00;
        attachedCANBus->sendFrame(var);

        //Regen set to int TODO ?????

        //Regen-P to 25 
        var.buf[0] = 0x65;
        var.buf[1] = 0x00;
        var.buf[2] = 0x00;
        var.buf[3] = 0x19;
        var.buf[4] = 0x00;
        attachedCANBus->sendFrame(var);

        //BTB Power TODO ????

        // TODO set PWM Freq to 8 kHz i16
        // var.len = 3;
        // var.buf[0] = 0x19;
        // var.buf[2] = 

        //set mode to Dig.Commands TODO
        
        //set cutoff (dig.) to 0 
        var.len = 3;
        var.buf[0] = 0x1e;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        //set cutoff (dig.) to 0
        var.len = 3;
        var.buf[0] = 0x1e;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        //Not correct - works in isolation - 2nd time doesnt work
        // KP current set to 20
        var.buf[0] = 0x1c;
        var.buf[2] = 0x00;
        var.buf[1] = 0x14;
        attachedCANBus->sendFrame(var);

        
        // TI set to 1600
        var.buf[0] = 0x1d;
        var.buf[2] = 0x06;
        var.buf[1] = 0x40;
        attachedCANBus->sendFrame(var);

        //TiM set to 80
        var.buf[0] = 0x2b;
        var.buf[2] = 0x00;
        var.buf[1] = 0x50;
        attachedCANBus->sendFrame(var);
        
        //xKp2 to 0
        var.buf[0] = 0xc9;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);
        
        //Kf set to 0
        var.buf[0] = 0xcb;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);
        
        //Ramp set to 2000
        var.buf[0] = 0x25;
        var.buf[2] = 0x07;
        var.buf[1] = 0xD0;
        attachedCANBus->sendFrame(var);
        
        //Not correct - works in isolation - 2nd time doesnt work
        //I max pk to 20
        var.buf[0] = 0xc4;
        var.buf[2] = 0x00;
        var.buf[1] = 0x14;
        attachedCANBus->sendFrame(var);
        //Not correct - works in isolation - 2nd time doesnt work
        //I con eff to 20
        var.buf[0] = 0xc5;
        var.buf[2] = 0x00;
        var.buf[1] = 0x14;
        attachedCANBus->sendFrame(var);
        //T-peak to 15
        var.buf[0] = 0xf0;
        var.buf[2] = 0x00;
        var.buf[1] = 0x0F;
        attachedCANBus->sendFrame(var);


        //Not correct - DOES NOT WORK IN ISOLATION - 2nd time doesnt work
        // I lim dig to 95 
        var.buf[0] = 0x46;
        var.buf[2] = 0x00;
        var.buf[1] = 0x5F;
        attachedCANBus->sendFrame(var);
        
        //Not correct - works in isolation - 2nd time doesnt work
        //I-red-N to 100
        var.len = 3;
        var.id = 0x201;
        var.buf[0] = 0x3C;
        var.buf[2] = 0x00;
        var.buf[1] = 0x64;
        attachedCANBus->sendFrame(var);

        
        //i-red-Td to 21000 
        var.buf[0] = 0x58;
        var.buf[2] = 0x52;
        var.buf[1] = 0x08;
        attachedCANBus->sendFrame(var);

        
        //i-red-te to 23000
        var.buf[0] = 0x4c;
        var.buf[2] = 0x59;
        var.buf[1] = 0xD8;
        attachedCANBus->sendFrame(var);
        
        //i-red-tm 32767
        var.buf[0] = 0xa2;
        var.buf[2] = 0x7F;
        var.buf[1] = 0xFF;
        attachedCANBus->sendFrame(var);
        
        //T Dc 0
        var.buf[0] = 0x07;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);
        
        //Not correct - works in isolation - 2nd time does not work
        //V Dc 40
        var.len = 3;
        var.id = 0x201;
        var.buf[0] = 0x08;
        var.buf[2] = 0x00;
        var.buf[1] = 0x28;
        attachedCANBus->sendFrame(var);

        //V min set to 4 TODO ????

                
        //F min set to 0 
        var.buf[0] = 0x0b;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);
        
        //V Corner set to 0
        var.buf[0] = 0x0c;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);
        
        //F corner set to 0
        var.buf[0] = 0x0d;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        //f-sh set to linear?? TODO

        
        // Kp speed set to 10
        var.buf[0] = 0x2c;
        var.buf[2] = 0x00;
        var.buf[1] = 0x0A;
        attachedCANBus->sendFrame(var);

        
        // Ti speed set to 20
        var.buf[0] = 0x2d;
        var.buf[2] = 0x00;
        var.buf[1] = 0x14;
        attachedCANBus->sendFrame(var);

        
        //Td speed set to 0
        var.buf[0] = 0x2e;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        
        //TiM set to 60
        var.buf[0] = 0x3b;
        var.buf[2] = 0x00;
        var.buf[1] = 0x3C;
        attachedCANBus->sendFrame(var);
        
        //Kacc set to 0 
        var.buf[0] = 0x5b;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        
        //filter set to 4
        var.buf[0] = 0x5e;
        var.buf[2] = 0x00;
        var.buf[1] = 0x04;
        attachedCANBus->sendFrame(var);

        //TODO N R-acc and N R-Dec
        
        //Not correct  - does not work in isolation - 2nd time does not
        var.len = 5;
        var.id = 0x201;
        //R-Lim set to 1000
        var.buf[0] = 0xc7;
        var.buf[4] = 0x00;
        var.buf[3] = 0x00;
        var.buf[2] = 0x03;
        var.buf[1] = 0xE8;
        attachedCANBus->sendFrame(var);
        
        //TODO M R-acc, M R-Dec, M R-rcp, N-100%

        
        //N-Lim set to 10
        var.len = 3;
        var.buf[0] = 0x34;
        var.buf[2] = 0x00;
        var.buf[1] = 0x0A;
        attachedCANBus->sendFrame(var);
        
        //N-Lim+ set to 100
        var.buf[0] = 0x3f;
        var.buf[2] = 0x00;
        var.buf[1] = 0x64;
        attachedCANBus->sendFrame(var);

        
        // //Not correct - does not work with loop - does not work in isolation - related to speed limit negative
        // //N-Lim- set to -100
        // var.len = 3;
        // var.id = 0x201;
        // var.buf[0] = 0x3e;
        // var.buf[2] = 0x00;
        // var.buf[1] = 0x00;
        // attachedCANBus->sendFrame(var);
        
        //Kp position to 0
        var.buf[0] = 0x6a;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        
        //Ti position set to 0
        var.buf[0] = 0x6b;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        
        //Td position set to 0 
        var.buf[0] = 0x6c;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        
        //TiM posiiton set to 0
        var.buf[0] = 0x71;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);
        
        //Tol-wind set to 0
        var.buf[0] = 0x79;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        
        //Off Ref set to 0 
        var.len = 5;
        var.buf[0] = 0x72;
        var.buf[4] = 0x00;
        var.buf[3] = 0x00;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        //ND-Scale set to 0
        var.buf[0] = 0x7c;
        var.buf[4] = 0x00;
        var.buf[3] = 0x00;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);
        
        //ND-Offset set to 0
        var.buf[0] = 0x7d;
        var.buf[4] = 0x00;
        var.buf[3] = 0x00;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);
        
        //speed 1 set to 120
        var.len = 3;
        var.buf[0] = 0x76;
        var.buf[2] = 0x00;
        var.buf[1] = 0x78;
        attachedCANBus->sendFrame(var);
                
        //speed 2 set to 120
        var.buf[0] = 0x77;
        var.buf[2] = 0x00;
        var.buf[1] = 0x78;
        attachedCANBus->sendFrame(var);
                
        //reso edge set to 0
        var.buf[0] = 0x75;
        var.buf[2] = 0x00;
        var.buf[1] = 0x00;
        attachedCANBus->sendFrame(var);

        //TODO ref-ramp set to LIM

        once = false;
    }
    */



    MotorController::handleTick();
    if (throttleRequested < 0) throttleRequested = 0;
    if (throttleRequested > 1000 && throttleRequested < 1300) throttleRequested = 1000;
    if (throttleRequested > 1400) throttleRequested = 0;

    
    //Logger::warn("throttleRequested | %i", throttleRequested);
    // //rounding to nearest 10th
    int a = throttleRequested;
    if (a < 50){
        a = 0;
    }
    else{
        a = a/20;
        //131071 is 2^17-1 which is in binary is 16 1's since this uses two's complement, this gives you a speed of around -1, as A increases to its max of 100, it will subtract around 2^16-1 from the binary giving you just a leading bit of 1 and a very large negative number as your speed.
        a = 65535 - a * 327;
        // (the following comments disregard the if/else statement)
        // at a = 0 (throttle not pressed), a becomes 2^17 -1 which is 17 1s. When this number is passed through first and second half and through the frame, the 17th bit gets truncated (buf values are 8 bits) --> -1 speed command
        // at a = 1000 (fully pressed), a becomes 65535 which is 16 1s. 
    }
    uint32_t secondhalf = (a & 0xFF);
    uint32_t firsthalf = ((a>>8));
    
    // Logger::warn("%i First half %i | Second half %i", a, firsthalf, secondhalf);
    // check if the motor will still spin even if the pedal is released all the way up

    var.len = 3;
    var.id = 0x201;

    // attachedCANBus->sendFrame(var);
    var.buf[0] = 0x51;
    // 0x04 to DISABLE
    // var.buf[1] = 0x04;
    // 0x00 to ENABLE
    var.buf[1] = 0x04;
    var.buf[2] = 0x00;
    attachedCANBus->sendFrame(var);



    var.buf[0] = 0x31;
    var.buf[1] = secondhalf; //secondhalf
    var.buf[2] = firsthalf; // first half
    attachedCANBus->sendFrame(var);
}

void BamocarMotorController::handleCanFrame(const CAN_message_t &frame) {
    
    Logger::info("Test id=%X len=%X data=%X,%X,%X,%X,%X,%X,%X,%X",
                      frame.id, frame.len, 
                      frame.buf[0], frame.buf[1], frame.buf[2], frame.buf[3],
                      frame.buf[4], frame.buf[5], frame.buf[6], frame.buf[7]);
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