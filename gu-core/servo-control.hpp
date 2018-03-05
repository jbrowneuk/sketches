#ifndef HEADER_SERVO_CONTROL
#define HEADER_SERVO_CONTROL

#include <Servo.h>

class ServoControl {
public:
  ServoControl(int pin);
  void update();
  void setRotationSpeed(int);
  void sweepClockwise();
  void sweepCounterclockwise();
  void sweepToOtherEnd();
  void rotateTo(int);
  int getAngle();
  bool atEndOfTravel();

private:
  int _desiredAngle;
  int _currentRotateSpeed;
  Servo _servo;
};

#endif
