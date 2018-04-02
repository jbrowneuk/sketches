#ifndef HEADER_STATE_CONTROLLER
#define HEADER_STATE_CONTROLLER

#include "led-control.hpp"
#include "servo-control.hpp"

class StateBase;

class StateController {
public:
  StateController();
  ~StateController();
  void update(LedControl&, LedControl&, ServoControl&);
  void setState(StateBase*);
  void setMotionState(bool);
  bool getMotionState();

private:
  StateBase* _state;
  bool _motionDetected;
};

#endif
