#include "state-controller.hpp"
#include "guardian-states.hpp"

StateController::StateController() {
  _state = new GuardianStateSleeping();
}

StateController::~StateController() {
  delete _state;
}

void StateController::update() {
  _state->update(*this);
}