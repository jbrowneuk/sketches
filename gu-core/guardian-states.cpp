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

void GuardianStateActive::update(StateController& controller) {
  LedControl& yellowLed = controller.getYellowLed();

  bool yellowLedIsOn = yellowLed.atMaxOrMin() && yellowLed.getBrightness() > 0;
  if (!yellowLedIsOn) {
    yellowLed.fadeIn();
  }

  if (!controller.getMotionState() && yellowLedIsOn) {
    yellowLed.fadeOut();
    controller.setState(new GuardianStatePowerDown());
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
