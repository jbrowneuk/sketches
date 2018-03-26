#include "servo-control.hpp"
#include "led-control.hpp"

// Pin definitions
const int PIN_LED_BLUE = 5;
const int PIN_LED_YELLOW = 6;
const int PIN_SERVO = 9;
const int PIN_BUTTON = 7;

// Maximum values
const int MAX_ROTATION_CHANGES = 6;

// Time intervals (in msec) between updating output values
const long LED_UPDATE_INTERVAL = 25;
const long SERVO_UPDATE_INTERVAL = 15;

// Last update timestamps for updating output values
unsigned long lastTimestampLed = 0;
unsigned long lastTimestampServo = 0;

// Current servo control values
short headRotationsRemaining = 0;
int lastAngle;

// Button input control value
int buttonState = 0;
bool preventRetrigger = false;

ServoControl mainServo;
LedControl ledBlue;
LedControl ledYellow;

/*
 * Initial setup
 */
void setup() {
  mainServo.setup(PIN_SERVO);
  ledBlue.setup(PIN_LED_BLUE);
  ledYellow.setup(PIN_LED_YELLOW);

  pinMode(PIN_BUTTON, INPUT);
}

/*
 * Controls the fading of the LED, up to a maximum value
 *
 * @param currentTimestamp: unsigned long - current timestamp, in msec
 * @param maximumBrightness: int - maximum brightness, 0 - 255
 */
void ledFadeControl(unsigned long currentTimestamp) {
  if (currentTimestamp - lastTimestampLed < LED_UPDATE_INTERVAL) {
    return;
  }

  lastTimestampLed = currentTimestamp;
  ledBlue.update();
  ledYellow.update();
}

/*
 * Controls the rotation of the servo
 *
 * @param currentTimestamp: unsigned long - current timestamp, in msec
 */
void servoControl(unsigned long currentTimestamp) {
  // Bail if elapsed time is smaller than interval
  if (currentTimestamp - lastTimestampServo < SERVO_UPDATE_INTERVAL) {
    return;
  }

  lastTimestampServo = currentTimestamp;
  mainServo.update();
}

/*
 * Main loop
 */
void loop() {
  buttonState = digitalRead(PIN_BUTTON);

  // Only trigger if button pressed and not currently triggered
  if (buttonState == HIGH && !preventRetrigger) {
    preventRetrigger = true;
    headRotationsRemaining = MAX_ROTATION_CHANGES;
    mainServo.sweepClockwise();
  }

  unsigned long currentTimestamp = millis();

  int ledYellowBrightness = 0;
  int ledBlueBrightness = 0;

  if (headRotationsRemaining > 0) {
    // Guardian active state
    const int currentAngle = mainServo.getAngle();

    ledYellowBrightness = LedControl::MaxBrightness;
    ledBlueBrightness = 0;

    // Change when at extremities and decrease head rotation count
    if (mainServo.atEndOfTravel() && currentAngle != lastAngle) {
      mainServo.sweepToOtherEnd();
      lastAngle = currentAngle;
      headRotationsRemaining--;
    }
  } else {
    // Guardian sleeping state
    mainServo.rotateTo(90);

    ledYellowBrightness = 0;
    ledBlueBrightness = LedControl::MaxBrightness;

    preventRetrigger = false;
  }

  ledYellow.setMaxBrightness(ledYellowBrightness);
  ledBlue.setMaxBrightness(ledBlueBrightness);

  if (ledYellowBrightness > 0) {
    ledYellow.fadeIn();
  }

  if (ledBlueBrightness > 0) {
    ledBlue.fadeIn();
  }

  servoControl(currentTimestamp);
  ledFadeControl(currentTimestamp);
}
