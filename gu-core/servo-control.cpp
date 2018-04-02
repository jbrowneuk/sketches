#include "servo-control.hpp"
#include "Arduino.h"

const int MAX_ANGLE = 180;
const int DEFAULT_ROTATION_SPEED = 2;

ServoControl::ServoControl() {
  _desiredAngle = 90;
  _currentRotateSpeed = DEFAULT_ROTATION_SPEED;
}

void ServoControl::setup(const int pin) {
  _servo.attach(pin);
  _servo.write(_desiredAngle);
}

void ServoControl::update() {
  const int currentAngle = getAngle();
  if (currentAngle == _desiredAngle) {
    return;
  }

  // Speed can be a large number, so a tolerance is needed to
  // prevent twitching back and forth
  const int lowerBound = _desiredAngle - _currentRotateSpeed;
  const int upperBound = _desiredAngle + _currentRotateSpeed;

  int calculatedAngle;
  if (currentAngle < lowerBound) {
    calculatedAngle = currentAngle + _currentRotateSpeed;
  } else if (currentAngle > upperBound) {
    calculatedAngle = currentAngle - _currentRotateSpeed;
  } else {
    calculatedAngle = _desiredAngle;
  }

  _servo.write(calculatedAngle);
}

void ServoControl::setRotationSpeed(int speed) {
  _currentRotateSpeed = abs(speed); // negative speed will break logic
}

void ServoControl::sweepClockwise() {
  _desiredAngle = MAX_ANGLE; // TODO check this and below
}

void ServoControl::sweepCounterclockwise() {
  _desiredAngle = 0; // TODO as with above, check
}

void ServoControl::sweepToOtherEnd() {
  const int angle = getAngle();
  if (angle < MAX_ANGLE / 2) {
    rotateTo(MAX_ANGLE);
  } else {
    rotateTo(0);
  }
}

void ServoControl::rotateTo(int angle) {
  _desiredAngle = angle;
}

int ServoControl::getAngle() {
  return _servo.read();
}

bool ServoControl::atEndOfTravel() {
  const int angle = getAngle();
  return angle < _currentRotateSpeed || angle > MAX_ANGLE - _currentRotateSpeed;
}

bool ServoControl::atDesiredAngle() {
  return getAngle() == _desiredAngle;
}

