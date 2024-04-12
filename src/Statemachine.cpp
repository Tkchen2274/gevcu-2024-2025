#include "Statemachine.h"

State extern_curr_state = S0;

Statemachine::Statemachine() { 
	// curr_state = S0;    
  curr_state = S0;   
  extern_curr_state = S0;
}

// std::string Statemachine::getState() { return curr_state; }
// Statemachine::Statemachine() { 
// 	curr_state = S0; 
			       
// }

State Statemachine::getState() { return curr_state; }

void Statemachine::setup() {

  tickHandler.detach(this);
  tickHandler.attach(this, CFG_TICK_INTERVAL_HEARTBEAT);
}

void Statemachine::handleTick() {
  // Logger::console("\nthis is the statemachine");
  
  if (extern_curr_state == S0) {
    extern_curr_state = S1;
    SerialUSB.print('0');

  } else if (extern_curr_state == S1) {
    extern_curr_state = S2;
    SerialUSB.print('1');

  } else if (extern_curr_state == S2) {
    extern_curr_state = S0;
    SerialUSB.print('2');

  }
}
