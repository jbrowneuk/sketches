#include "state-controller.hpp"
#include "guardian-states.hpp"

StateController::StateController() {
  _motionDetected = false;
  _state = new GuardianStateSleeping();
}

StateController::~StateController() {
  delete _state;
}

void StateController::update() {
  _state->update(*this);
}

void StateController::setState(StateBase* state) {
  StateBase* original = _state;
  _state = state;
  delete original;
}

void StateController::setMotionState(bool detected) {
  _motionDetected = detected;
}

bool StateController::getMotionState() {
  return _motionDetected;
}