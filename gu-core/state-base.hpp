#ifndef HEADER_STATE_BASE
#define HEADER_STATE_BASE

#include "state-controller.hpp"

class StateBase {
public:
  virtual void update(StateController&) = 0;

protected:
  void setState(StateController& controller, StateBase* state) {
    StateBase* original = controller._state;
    controller._state = state;
    delete original;
  }
};

#endif