#include "Statemachine.h"

Statemachine::Statemachine() { 
	curr_state = S0; 
			       
}

std::string Statemachine::getState() { return curr_state; }

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
  if (curr_state = S0) {

  } else if (curr_state = S1) {

  } else if (curr_state = S2) {

  }
}
