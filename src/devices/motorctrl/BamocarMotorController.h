/*
 * BamocarMotorController.h
 */

#ifndef _BAMOCARINVERTER_H_
#define _BAMOCARINVERTER_H_

#include <Arduino.h>
#include "../../config.h"
#include "MotorController.h"
#include "../../sys_io.h"
#include "../../TickHandler.h"
#include "../../CanHandler.h"

#define BAMOCARINVERTER 0x1010
#define CFG_TICK_INTERVAL_MOTOR_CONTROLLER_BAMOCAR     40000

/*
 * Class for DMOC specific configuration parameters
 */
class BamocarMotorControllerConfiguration : public MotorControllerConfiguration {
public:
};

class BamocarMotorController: public MotorController, CanObserver {
public:

public:
    virtual void handleTick();
    virtual void setup();
    void earlyInit();
    void setGear(Gears gear);

    BamocarMotorController();
    DeviceId getId();
    uint32_t getTickInterval();

    void handleCanFrame(const CAN_message_t &frame);

    virtual void loadConfiguration();
    virtual void saveConfiguration();

private:

    uint16_t torqueCommand;
    void timestamp();

    CAN_message_t var; // delete this when handleTick is fully functional
};

#endif /* DMOC_H_ */



