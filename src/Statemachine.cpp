#include "Statemachine.h"
#include "sys_io.h"


/*
 * Ready to Drive StateMachine 
 * 
 *
 */


State extern_curr_state = S0;

void Statemachine::setup() {

  tickHandler.detach(this);
  tickHandler.attach(this, CFG_TICK_INTERVAL_HEARTBEAT);
}

Statemachine::Statemachine() {    
  extern_curr_state = S0;
}

State Statemachine::getState() { return extern_curr_state; }

void Statemachine::updateState(State x){extern_curr_state = x;}

void Statemachine::handleTick() {
   
  // Temp Variables
  bool threshold_brake;

  float raw_brake1 = systemIO.getAnalogIn(0);  // referece pothrottle.cpp for mapping
  float raw_brake2 = systemIO.getAnalogIn(4);

  threshold


  
  bool tsms = systemIO.getDigitalIn(4);   // reads the digital switch value
  bool r2d = systemIO.getDigitalIn(5);    // reads the digital r2d 

  // Logger::console("DIN1: %d, DIN4: %d, DIN5: %d", brake, tsms, r2d);

  if (extern_curr_state == S0) {

     if(tsms && r2d && threshold_brake){
      updateState(S1);
    } else {
      updateState(S0);
    }
     
    SerialUSB.print('0');

  } 

  else if (extern_curr_state == S1) {

    if(tsms && brake && r2d){
      updateState(S2);
    } else {
      updateState(S0);
    }
    // extern_curr_state = S2;
    updateState(S2);
    // Logger::console("\nI am in state S1");
    SerialUSB.print('1');

  } 

  else if (extern_curr_state == S2) {
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
  
  


    // case S0:
    //   updateState(S1);
    //   Logger::console("\nI am in state S0 ");
      
      
    //   // SerialUSB.print('0');
    // case S1:
    //   updateState(S2);
    //   Logger::console("\nI am in state S1 ");
    //   // SerialUSB.print('1');
    // case S2:
    //   updateState(S0);
    //   Logger::console("\nI am in state S2 ");
      // SerialUSB.print('2');
  
  // if (extern_curr_state == S0) {
  //   // extern_curr_state = S1;
  //   updateState(S1);
  //   Logger::console("\nI am in state S0");
  //   SerialUSB.print('0');

  // } else if (extern_curr_state == S1) {
  //   // extern_curr_state = S2;
  //   updateState(S2);
  //   Logger::console("\nI am in state S1");
  //   SerialUSB.print('1');

  // } else if (extern_curr_state == S2) {
  //   // extern_curr_state = S0;
  //   updateState(S0);
  //   Logger::console("\nI am in state S2");
  //   SerialUSB.print('2');

  // }

  /*
    State 0: Idle car
    State 1: Signal to car to prepare for driving
    State 2: Motor responds to APPS
    if (State 0 and Ready to drive button){
      send signal to dash
      update to State 1
    }
    if (State 1 and NOT Tractive system master switch)
    {
      update to state 0
    }
    if (State 1 and Tracktive system master switch)
    {
      update to state 2
    }
    if (State 2 and Not tractive system Master Switch)
    {
      update to state 1
    }
    */
}
