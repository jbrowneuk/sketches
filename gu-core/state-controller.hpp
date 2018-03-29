#ifndef HEADER_STATE_CONTROLLER
#define HEADER_STATE_CONTROLLER

class StateBase;

class StateController {
  friend class StateBase;

public:
  StateController();
  ~StateController();
  void update();

private:
  StateBase* _state;
};

#endif