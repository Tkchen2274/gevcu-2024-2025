#include "Statemachine.h"

<<<<<<< HEAD
State extern_curr_state = S0;

Statemachine::Statemachine() { 
	// curr_state = S0;    
  curr_state = S0;   
  extern_curr_state = S0;
}

// std::string Statemachine::getState() { return curr_state; }
=======
Statemachine::Statemachine() { 
	curr_state = S0; 
			       
}

std::string Statemachine::getState() { return curr_state; }
>>>>>>> 4d8799480cdf1d39d9fc84fa8616d65cfae3ccc3

void Statemachine::setup() {
  tickHandler.detach(this);
  tickHandler.attach(this, CFG_TICK_INTERVAL_HEARTBEAT);
}

void Statemachine::handleTick() {
  Logger::console("\n*************SYSTEM MENU *****************");
  // asdf
  //
  // this is wehre the state machine goes
  //
<<<<<<< HEAD
  // if (curr_state = S0) {

  // } else if (curr_state = S1) {

  // } else if (curr_state = S2) {

  // }
=======
  if (curr_state = S0) {

  } else if (curr_state = S1) {

  } else if (curr_state = S2) {

  }
>>>>>>> 4d8799480cdf1d39d9fc84fa8616d65cfae3ccc3
}
