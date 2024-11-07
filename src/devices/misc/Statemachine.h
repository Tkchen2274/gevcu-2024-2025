#ifndef Statemachine_H_
#define Statemachine_H_

#include <Arduino.h>
#include "../Device.h"
#include "../../DeviceManager.h"
#include "../../CanHandler.h"


#define StatemachineID 0x103
#define StatemachineTickInt 1000000

enum State {
    S0,
    S1,
    S2
};

extern State extern_curr_state;

class StatemachineDevice: public Device, CanObserver{
public:
    StatemachineDevice();  //called nearly immediately to initialize your own variables
    void setup();          //called only if the device is actually enabled
    void earlyInit();      //called early and whether or not the device is enabled. Just used to setup configuration
    void handleTick(); 
    void handleCanFrame(const CAN_message_t &frame);
    DeviceId getId();
    DeviceType getType();

    State getState();
    void updateState(State);

private:
    // State curr_state;        
    int8_t dash_send_flag;    // controls when to send message gevcu to dash 
    int8_t dash_val_msg;      // set when we recieve the check from car
    CAN_message_t buzz_msg;   // the constructed msg to activate buzzer_msg 
    uint32_t counter_timer;   // control when to send another buzzer message
                              // NOTE: there needs to be a check in the dash 
                              //    that it'll only buzz once when recieved for the first time
                              //    any more messages after should be ignored
    
};
#endif
