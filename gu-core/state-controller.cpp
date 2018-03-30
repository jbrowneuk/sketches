#include "state-controller.hpp"
#include "guardian-states.hpp"

const int PIN_LED_BLUE = 5;
const int PIN_LED_YELLOW = 6;
const int PIN_SERVO = 9;

StateController::StateController() {
  _motionDetected = false;
  _state = new GuardianStateSleeping();

  _ledBlue.setup(PIN_LED_BLUE);
  _ledYellow.setup(PIN_LED_YELLOW);
  _headServo.setup(PIN_SERVO);

  _ledYellow.setFadeSpeed(8);
}

StateController::~StateController() {
  delete _state;
}

void StateController::update() {
  _state->update(*this);
  _ledBlue.update();
  _ledYellow.update();
  _headServo.update();
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

LedControl& StateController::getBlueLed() {
  return _ledBlue;
}

LedControl& StateController::getYellowLed() {
  return _ledYellow;
}

ServoControl& StateController::getHeadServo() {
  return _headServo;
}
