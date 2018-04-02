#include "Arduino.h"
#include "guardian-states.hpp"
#include "led-control.hpp"

const int DEFAULT_ANGLE = 90;

void GuardianStateSleeping::update(StateController& controller, LedControl& blueLed, LedControl& yellowLed, ServoControl& headServo) {
  headServo.disable();

  if (controller.getMotionState()) {
    headServo.enable();
    controller.setState(new GuardianStatePowerUp());
  }
}

GuardianStatePowerUp::GuardianStatePowerUp() {
  _angleTimestamp = millis();
  _selectedAngleIndex = 0;
  _allowLedOn = false;
}

void GuardianStatePowerUp::update(StateController& controller, LedControl& blueLed, LedControl& yellowLed, ServoControl& headServo) {
  // The in-game decayed Guardian start up sequence involves its runes lighting up red while the following happens:
  // - The head moves up and
  // - The head twitches randomly a few degrees left and right from 90.
  //   I can't quite pin down the exact movement pattern so I've approximated it here by going a few degrees either side of 90.
  // - the eye then switches on rapidly before searching begins

  const int numberOfAngles = 4;
  const int angles[numberOfAngles] = {80, 90, 100, 90};
  const int angle = angles[_selectedAngleIndex];
  headServo.rotateTo(angle);

  if (headServo.getAngle() == angle) {

    const bool canUpdateAngle = millis() - _angleTimestamp > 500;
    if (canUpdateAngle) {
      _selectedAngleIndex += 1;
      _angleTimestamp = millis();
    }

    if (_selectedAngleIndex >= numberOfAngles) {
      // Quit breaking shit
      _selectedAngleIndex = numberOfAngles - 1;
      _allowLedOn = true;
    }
  }

  if (!_allowLedOn) {
    return;
  }

  // Break into a second state
  if (blueLed.atMaxOrMin()) {
    if (blueLed.getBrightness() == 0) {
      blueLed.fadeIn();
    } else {
      controller.setState(new GuardianStateActive());
    }
  }
}

GuardianStateActive::GuardianStateActive() {
  _initialTimestamp = 0;
  _directionChangesRemaining = 1;
  _requiresFirstMovement = true;
}

void GuardianStateActive::update(StateController& controller, LedControl& blueLed, LedControl& yellowLed, ServoControl& headServo) {
  if (_requiresFirstMovement) {
    headServo.rotateTo(0);
    _requiresFirstMovement = false;
  }

  if (yellowLed.atMaxOrMin()) {
    yellowLed.fadeToOtherState();
  }

  if (headServo.atEndOfTravel()) {
    if (_directionChangesRemaining > 0) {
      headServo.sweepToOtherEnd();
      _directionChangesRemaining -= 1;
    } else {
      headServo.rotateTo(DEFAULT_ANGLE);
    }
  }

  // Starting to look like two states here...
  if (_directionChangesRemaining == 0 && headServo.getAngle() == DEFAULT_ANGLE && _initialTimestamp == 0) {
    _initialTimestamp = millis();
  }

  if (_initialTimestamp == 0) {
    return;
  }

  const bool stateEnded = millis() - _initialTimestamp > 4000;

  if (stateEnded) {
    if(controller.getMotionState()) {
      controller.setState(new GuardianStateActive());
    } else {
      yellowLed.fadeOut();
      controller.setState(new GuardianStatePowerDown());
    }
  }
}

void GuardianStatePowerDown::update(StateController& controller, LedControl& blueLed, LedControl& yellowLed, ServoControl& headServo) {
  if (blueLed.atMaxOrMin() && headServo.getAngle() == DEFAULT_ANGLE) {
    if (blueLed.getBrightness() > 0) {
      blueLed.fadeOut();
    } else {
      controller.setState(new GuardianStateSleeping());
    }
  }
}
