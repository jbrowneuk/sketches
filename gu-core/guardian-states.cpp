#include "Arduino.h"
#include "guardian-states.hpp"
#include "led-control.hpp"

void GuardianStateSleeping::update(StateController& controller) {
  if (controller.getMotionState()) {
    controller.setState(new GuardianStatePowerUp());
  }
}

void GuardianStatePowerUp::update(StateController& controller) {
  LedControl& blueLed = controller.getBlueLed();
  if (blueLed.atMaxOrMin()) {
    if (blueLed.getBrightness() == 0) {
      blueLed.fadeIn();
    } else {
      controller.setState(new GuardianStateActive());
    }
  }
}

GuardianStateActive::GuardianStateActive() {
  _lastTimestamp = 0;
}

void GuardianStateActive::update(StateController& controller) {
  LedControl& yellowLed = controller.getYellowLed();

  if (yellowLed.atMaxOrMin()) {
    yellowLed.fadeToOtherState();
  }

  const unsigned long currentTimestamp = millis();
  const bool stateEnded = currentTimestamp - _lastTimestamp > 4000;
  _lastTimestamp = currentTimestamp;

  if (stateEnded) {
    if(controller.getMotionState()) {
      _lastTimestamp = 0; // TODO: encapsulation
      controller.setState(new GuardianStateActive());
    } else {
      yellowLed.fadeOut();
      controller.setState(new GuardianStatePowerDown());
    }
  }
}

void GuardianStatePowerDown::update(StateController& controller) {
  LedControl& blueLed = controller.getBlueLed();
  if (blueLed.atMaxOrMin()) {
    if (blueLed.getBrightness() > 0) {
      blueLed.fadeOut();
    } else {
      controller.setState(new GuardianStateSleeping());
    }
  }
}
