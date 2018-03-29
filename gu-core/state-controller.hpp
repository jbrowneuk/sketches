#ifndef HEADER_STATE_CONTROLLER
#define HEADER_STATE_CONTROLLER

class StateBase;

class StateController {
public:
  StateController();
  ~StateController();
  void update();
  void setState(StateBase*);
  void setMotionState(bool);
  bool getMotionState();

private:
  StateBase* _state;
  bool _motionDetected;
};

#endif