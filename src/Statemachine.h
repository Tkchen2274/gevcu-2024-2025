#include "Logger.h"
#include "TickHandler.h"
#include "config.h"
#include "sys_io.h"
#include <string>

<<<<<<< HEAD
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

=======
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

extern State extern_curr_state; // Declaration of a global variable               2000000
>>>>>>> 4d8799480cdf1d39d9fc84fa8616d65cfae3ccc3
// struct RawSignalData {
//     int32_t input1; // e.g. pot #1 or the signal from a can bus throttle
//     int32_t input2; // e.g. pot #2 (optional)
//     int32_t input3; // e.g. pot #3 (optional)
// };

class Statemachine : public TickObserver {
public:
  Statemachine(); // constructor
  void handleTick();
<<<<<<< HEAD
  std::string getState();
=======
  std::string getState();
>>>>>>> 4d8799480cdf1d39d9fc84fa8616d65cfae3ccc3
  void setup();

private:
  int32_t input1;
<<<<<<< HEAD
  // std::string curr_state;
  State curr_state;
=======
  // std::string curr_state;
>>>>>>> 4d8799480cdf1d39d9fc84fa8616d65cfae3ccc3
  // analog value from the pedal
  // digital signal form button
  // tsms value which is digital
};
