#include "servo-control.hpp"
#include "led-control.hpp"

// Pin definitions
const int PIN_LED = 5;
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
LedControl mainLed;

/*
 * Initial setup
 */
void setup() {
  mainServo.setup(PIN_SERVO);
  mainLed.setup(PIN_LED);

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
  mainLed.update();
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

    // Reset LED control to fade in
    currentBrightnessDiff = DIFF_LED_BRIGHTNESS;
  }

  unsigned long currentTimestamp = millis();

  if (headRotationsRemaining > 0) {
    // Guardian active state
    currentMaxLedBrightness = 32; // todo: increase speed as opposed to brightness
    const int currentAngle = mainServo.getAngle();

    // Change when at extremities and decrease head rotation count
    if (mainServo.atEndOfTravel() && currentAngle != lastAngle) {
      mainServo.sweepToOtherEnd();
      lastAngle = currentAngle;
      headRotationsRemaining--;
    }
  } else {
    // Guardian sleeping state
    currentMaxLedBrightness = 255; // Magic number :/
    mainServo.rotateTo(90);
    preventRetrigger = false;
  }

  mainLed.setMaxBrightness(currentMaxLedBrightness);

  servoControl(currentTimestamp);
  ledFadeControl(currentTimestamp);
}
