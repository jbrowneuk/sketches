#ifndef HEADER_STATE_CONTROLLER
#define HEADER_STATE_CONTROLLER

#include "led-control.hpp"
#include "servo-control.hpp"

class StateBase;

class StateController {
public:
  StateController();
  ~StateController();
  void update();
  void setState(StateBase*);
  void setMotionState(bool);
  bool getMotionState();
  LedControl& getBlueLed();
  LedControl& getYellowLed();
  ServoControl& getHeadServo();

private:
  StateBase* _state;
  bool _motionDetected;
  LedControl _ledBlue;
  LedControl _ledYellow;
  ServoControl _headServo;
};

#endif
