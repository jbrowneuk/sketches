#include "Arduino.h"
#include "guardian-states.hpp"

void GuardianStateSleeping::update(StateController& controller) {
  Serial.println("Sleeping");

  if (controller.getMotionState()) {
    controller.setState(new GuardianStatePowerUp());
  }
}

void GuardianStatePowerUp::update(StateController& controller) {
  Serial.println("PowerUp");
  controller.setState(new GuardianStateActive());
}

void GuardianStateActive::update(StateController& controller) {
  Serial.println("Active");

  if (!controller.getMotionState()) {
    controller.setState(new GuardianStatePowerDown());
  }
}

void GuardianStatePowerDown::update(StateController& controller) {
  Serial.println("Power Down");
  controller.setState(new GuardianStateSleeping());
}
