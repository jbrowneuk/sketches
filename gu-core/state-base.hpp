#ifndef HEADER_STATE_BASE
#define HEADER_STATE_BASE

#include "state-controller.hpp"

class StateBase {
public:
  virtual void update(StateController&, LedControl&, LedControl&, ServoControl&) = 0;
};

#endif