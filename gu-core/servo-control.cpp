#include "servo-control.hpp"
#include "Arduino.h"

const int MAX_ANGLE = 180;
const int DEFAULT_ROTATION_SPEED = 2;

ServoControl::ServoControl(int pin) {
  _servo = Servo();
  _servo.attach(pin);

  _currentRotateSpeed = DEFAULT_ROTATION_SPEED;
}

void ServoControl::update() {
  const int currentAngle = getAngle();

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
  // Sanity check
  if (speed > MAX_ANGLE) {
    return;
  }

  _currentRotateSpeed = abs(speed); // negative speed will break logic
}

void ServoControl::sweepClockwise() {
  _desiredAngle = 0; // TODO check this and below
}

void ServoControl::sweepCounterclockwise() {
  _desiredAngle = MAX_ANGLE; // TODO as with above, check
}

void ServoControl::sweepToOtherEnd() {
  const angle = getAngle();
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
  return angle <= 0 || angle >= MAX_ANGLE;
}
