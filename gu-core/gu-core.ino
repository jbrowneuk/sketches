#include "state-controller.hpp"

const int PIN_PIR = 7;
const int PIN_LED_BLUE = 5;
const int PIN_LED_YELLOW = 6;
const int PIN_SERVO = 9;

const int UPDATE_INTERVAL_MSEC = 25; // Approx 40fps

unsigned long lastTimestamp = 0;

StateController stateMachine;

LedControl ledBlue;
LedControl ledYellow;
ServoControl headServo;

/*
 * Initial setup
 */
void setup() {
  pinMode(PIN_PIR, INPUT);

  ledBlue.setup(PIN_LED_BLUE);
  ledYellow.setup(PIN_LED_YELLOW);
  headServo.setup(PIN_SERVO);

  ledYellow.setFadeSpeed(8);
  headServo.setRotationSpeed(4);
}

/*
 * Main loop
 */
void loop() {
  unsigned long currentTimestamp = millis();
  if (currentTimestamp - lastTimestamp < UPDATE_INTERVAL_MSEC) {
    return;
  }

  lastTimestamp = currentTimestamp;

  bool motionDetected = digitalRead(PIN_PIR) == HIGH;
  stateMachine.setMotionState(motionDetected);

  stateMachine.update(ledBlue, ledYellow, headServo);

  ledBlue.update();
  ledYellow.update();
  headServo.update();
}
