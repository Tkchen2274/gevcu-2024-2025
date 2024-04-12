#include "Logger.h"
#include "TickHandler.h"
#include "config.h"
#include "sys_io.h"
#include <string>

// #define S0 "S0"
// #define S1 "S1"
// #define S2 "S2"

// #define S0 0
// #define S1 1
// #define S2 2

enum State {
    S0,
    S1,
    S2
};

#define CFG_TICK_INTERVAL_HEARTBEAT                 2000000

extern State extern_curr_state; // Declaration of a global variable

// #define S0 "S0"
// #define S1 "S1"
// #define S2 "S2"
// #define CFG_TICK_INTERVAL_HEARTBEAT                 2000000
// struct RawSignalData {
//     int32_t input1; // e.g. pot #1 or the signal from a can bus throttle
//     int32_t input2; // e.g. pot #2 (optional)
//     int32_t input3; // e.g. pot #3 (optional)
// };

class Statemachine : public TickObserver {
public:
  Statemachine(); // constructor
  void handleTick();
  State getState();
  void updateState(State);
  void setup();

private:
  int32_t input1;
  State curr_state;
  // analog value from the pedal
  // digital signal form button
  // tsms value which is digital
};
