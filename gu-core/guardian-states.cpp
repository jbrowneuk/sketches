#include "Arduino.h"
#include "guardian-states.hpp"
#include "led-control.hpp"

const int DEFAULT_ANGLE = 90;

/*
 * The default state when no movement is detected
 */
void GuardianStateSleeping::update(StateController& controller, LedControl& blueLed, LedControl& yellowLed, ServoControl& headServo) {
  headServo.disable();

  if (controller.getMotionState()) {
    headServo.enable();
    controller.setState(new GuardianStatePowerMotor());
  }
}

/*
 * The state that "powers on" the servo motor
 */
GuardianStatePowerMotor::GuardianStatePowerMotor() {
  _angleTimestamp = millis();
  _selectedAngleIndex = 0;
}

void GuardianStatePowerMotor::update(StateController& controller, LedControl& blueLed, LedControl& yellowLed, ServoControl& headServo) {
  // I can't quite pin down the exact movement pattern so I've approximated it here by going a few degrees either side of 90.
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
      controller.setState(new GuardianStatePowerLed());
    }
  }
}

/*
 * The state that powers on the blue LED
 */
void GuardianStatePowerLed::update(StateController& controller, LedControl& blueLed, LedControl& yellowLed, ServoControl& headServo) {
  if (!blueLed.atMaxOrMin()) {
    return;
  }

  if (blueLed.getBrightness() == 0) {
    blueLed.fadeIn();
  } else {
    controller.setState(new GuardianStateActive());
  }
}

/*
 * The state that controls the "searching" head movement
 */
GuardianStateSearching::GuardianStateSearching() {
  _directionChangesRemaining = 1;
  _requiresFirstMovement = true;
}

void GuardianStateSearching::update(StateController& controller, LedControl& blueLed, LedControl& yellowLed, ServoControl& headServo) {
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

  if (_directionChangesRemaining == 0 && headServo.getAngle() == DEFAULT_ANGLE) {
    controller.setState(new GuardianStateActive());
  }
}

/*
 *
 */
GuardianStateActive::GuardianStateActive() {
  _initialTimestamp = 0;
}

void GuardianStateActive::update(StateController& controller, LedControl& blueLed, LedControl& yellowLed, ServoControl& headServo) {
  const bool stateEnded = millis() - _initialTimestamp > 4000;

  if (!stateEnded) {
    return;
  }

  if(controller.getMotionState()) {
    controller.setState(new GuardianStateSearching());
  } else {
    yellowLed.fadeOut();
    controller.setState(new GuardianStatePowerDown());
  }
}

/*
 *
 */
void GuardianStatePowerDown::update(StateController& controller, LedControl& blueLed, LedControl& yellowLed, ServoControl& headServo) {
  // Sanity check
  if (!headServo.getAngle() == DEFAULT_ANGLE) {
    headServo.rotateTo(DEFAULT_ANGLE);
  }

  if (blueLed.atMaxOrMin() && headServo.getAngle() == DEFAULT_ANGLE) {
    if (blueLed.getBrightness() > 0) {
      blueLed.fadeOut();
    } else {
      controller.setState(new GuardianStateSleeping());
    }
  }
}
