#include "Arduino.h"
#include "guardian-states.hpp"

/*
class GuardianStateSleeping : StateBase { };
class GuardianStatePowerUp : StateBase { };
class GuardianStateActive : StateBase { };
class GuardianStatePowerDown : StateBase { };
*/

int counter = 0;

void GuardianStateSleeping::update(StateController& controller) {
  Serial.println("Sleeping");
  counter += 1;

  if (counter > 8) {
    counter = 0;
    setState(controller, new GuardianStatePowerUp());
  }
}

void GuardianStatePowerUp::update(StateController& controller) {
  Serial.println("PowerUp");
  counter += 1;

  if (counter > 4) {
    counter = 0;
    setState(controller, new GuardianStateActive());
  }
}

void GuardianStateActive::update(StateController& controller) {
  Serial.println("Active");
  counter += 1;

  if (counter > 16) {
    counter = 0;
    setState(controller, new GuardianStatePowerDown());
  }
}

void GuardianStatePowerDown::update(StateController& controller) {
  Serial.println("Power Down");
  setState(controller, new GuardianStateSleeping());
}
