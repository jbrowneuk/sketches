#include "state-controller.hpp"

const int PIN_PIR = 7;
const int UPDATE_INTERVAL_MSEC = 25; // Approx 40fps

unsigned long lastTimestamp = 0;

StateController stateMachine;

/*
 * Initial setup
 */
void setup() {
  pinMode(PIN_PIR, INPUT);
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

  stateMachine.update();
}
