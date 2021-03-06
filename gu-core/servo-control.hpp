#ifndef HEADER_SERVO_CONTROL
#define HEADER_SERVO_CONTROL

#include <Servo.h>

class ServoControl {
public:
  ServoControl();
  void setup(int);
  void update();
  void setRotationSpeed(int);
  void sweepClockwise();
  void sweepCounterclockwise();
  void sweepToOtherEnd();
  void rotateTo(int);
  int getAngle();
  bool atEndOfTravel();
  bool atDesiredAngle();
  void enable();
  void disable();

private:
  int _desiredAngle;
  int _currentRotateSpeed;
  int _pin;

  Servo _servo;
};

#endif
