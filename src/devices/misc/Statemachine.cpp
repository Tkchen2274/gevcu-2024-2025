#include "Statemachine.h"

State extern_curr_state = S0;  // Define and initialize the variable here

StatemachineDevice::StatemachineDevice():Device() {
    commonName = "Statemachine";
    shortName = "SM";
}

State StatemachineDevice::getState() { return extern_curr_state; }

void StatemachineDevice::updateState(State x){extern_curr_state = x;}

void StatemachineDevice::earlyInit()
{
    prefsHandler = new PrefHandler(StatemachineID);
}

void StatemachineDevice::setup() {
    tickHandler.detach(this);
    Logger::info("add device: StatemachineDevice (id: %X, %X)", StatemachineID, this);

    Device::setup(); // run the parent class version of this function

    setAttachedCANBus(0);
    //Relevant BMS messages are 0x300 - 0x30F
    attachedCANBus->attach(this, 0x310, 0x000, false);
    tickHandler.attach(this, StatemachineTickInt);

    // set flags
    dash_send_flag = 1;
    dash_val_msg = 0;

    // Constructed message to dashboard
    buzz_msg.len = 2;
    buzz_msg.id = 0x109;
    buzz_msg.buf[0] = 0x1;
    buzz_msg.buf[1] = 0x30;
}

/*For all multibyte integers the format is MSB first, LSB last
*/
void StatemachineDevice::handleCanFrame(const CAN_message_t &frame) {
    if(Logger::isDebug()){
        Logger::debug("Statemachine id=%X len=%X data=%X,%X,%X,%X,%X,%X,%X,%X",
                      frame.id, frame.len, 
                      frame.buf[0], frame.buf[1], frame.buf[2], frame.buf[3],
                      frame.buf[4], frame.buf[5], frame.buf[6], frame.buf[7]);
    }
    if(frame.id == 0x110){
      if(frame.buf[0] == 0x1 && frame.buf[1] == 0x2)
        dash_val_msg = 1; 
    }
}
DeviceId StatemachineDevice::getId() {
    return (StatemachineID);
}

DeviceType StatemachineDevice::getType() {
    return (DEVICE_MISC);
}

void StatemachineDevice::handleTick() {

  bool brake = systemIO.getDigitalIn(1);
  bool tsms = systemIO.getDigitalIn(4);
  bool r2d = systemIO.getDigitalIn(5);
  
  // Logger::console("DIN1: %d, DIN4: %d, DIN5: %d", brake, tsms, r2d);

  if (extern_curr_state == S0) {
    // Logger::console("\nI am in state S0");
    // Logger::console("", brake);
    // Logger::console(tsms);
    // Logger::console(r2d);
    // extern_curr_state = S1;

    if(brake && tsms && r2d){
      updateState(S1);
    } else {
      updateState(S0);
    }
    
    
    SerialUSB.print('0');

  } else if (extern_curr_state == S1) {

    /*
      As long as the tsms && brake && r2d are all valid
      Then proceeed to S2, else we'll have to replay this again

      It assumes that you have the brakes depressed in state 2, you might 
      beable to get rid of it

      Note: I might need a timer on the redundancy and count some cycles 
      before returning to s0
    */

    if (dash_send_flag == 1) {
      dash_send_flag = 0; 
      attachedCANBus->sendFrame(buzz_msg);
    }

    if(tsms && brake && dash_val_msg){
      updateState(S2);
    } else {
      updateState(S0);
    }

    updateState(S2);                         // this is for debugging 

    /*
      TODO: Send message to the dash and wait for a  
      message check. 
    */

    // extern_curr_state = S2;
    // Logger::console("\nI am in state S1");
    // SerialUSB.print('1');

  } else if (extern_curr_state == S2) {
    // gevcu is connected to low voltage

    if(tsms){
      updateState(S2);
    } else {
      updateState(S0);
    }

    // extern_curr_state = S0;
    // Logger::console("\nI am in state S2");
    SerialUSB.print('2');
  }
}
//testDevice test_device;
StatemachineDevice statemachine_device;
