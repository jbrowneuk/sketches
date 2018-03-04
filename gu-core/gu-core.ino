#include <Servo.h>

// Pin definitions
const int PIN_LED = 5;
const int PIN_SERVO = 9;
const int PIN_BUTTON = 7;

// Constants used to change physical output values
const int DIFF_LED_BRIGHTNESS = 2;
const int DIFF_SERVO_ROTATION = 2;

// Maximum values
const int MAX_ROTATION_CHANGES = 6;
const int MAX_SERVO_ANGLE = 180;
const int MAX_LED_BRIGHTNESS = 255;

// Time intervals (in msec) between updating output values
const long LED_UPDATE_INTERVAL = 25;
const long SERVO_UPDATE_INTERVAL = 15;

// Last update timestamps for updating output values
unsigned long lastTimestampLed = 0;
unsigned long lastTimestampServo = 0;

// Current LED control values
int currentBrightness = DIFF_LED_BRIGHTNESS;
int currentBrightnessDiff = DIFF_LED_BRIGHTNESS;

// Current servo control values
int currentServoAngle = 0;
int currentServoAngleDiff = DIFF_SERVO_ROTATION;
short headRotationsRemaining = 0;

// Button input control value
int buttonState = 0;
bool preventRetrigger = false;

Servo mainServo;

/*
 * Initial setup
 */
void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);

  mainServo.attach(PIN_SERVO);
  currentServoAngle = mainServo.read();
}

/*
 * Controls the fading of the LED, up to a maximum value
 *
 * @param currentTimestamp: unsigned long - current timestamp, in msec
 * @param maximumBrightness: int - maximum brightness, 0 - 255
 */
void ledFadeControl(unsigned long currentTimestamp, int maximumBrightness) {
  if (currentTimestamp - lastTimestampLed < LED_UPDATE_INTERVAL) {
    return;
  }

  lastTimestampLed = currentTimestamp;

  currentBrightness += currentBrightnessDiff;
  analogWrite(PIN_LED, currentBrightness);

  // Set difference sign when over extremities
  if (currentBrightness < DIFF_LED_BRIGHTNESS) {
    currentBrightnessDiff = DIFF_LED_BRIGHTNESS;
  } else if (currentBrightness > maximumBrightness - DIFF_LED_BRIGHTNESS) {
    currentBrightnessDiff = -DIFF_LED_BRIGHTNESS;
  }
}

/*
 * Controls the rotation of the servo
 *
 * @param currentTimestamp: unsigned long - current timestamp, in msec
 */
void servoControl(unsigned long currentTimestamp) {
  if (currentTimestamp - lastTimestampServo < SERVO_UPDATE_INTERVAL) {
    return;
  }

  lastTimestampServo = currentTimestamp;

  currentServoAngle += currentServoAngleDiff;
  mainServo.write(currentServoAngle);

  // Flip difference sign when at extremities and decrease head rotation count
  if (currentServoAngle < DIFF_SERVO_ROTATION || currentServoAngle > MAX_SERVO_ANGLE - DIFF_SERVO_ROTATION) {
    currentServoAngleDiff = -currentServoAngleDiff;
    headRotationsRemaining--;
  }
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

    // Reset LED control to fade in
    currentBrightnessDiff = DIFF_LED_BRIGHTNESS;
  }

  unsigned long currentTimestamp = millis();
  int currentMaxLedBrightness;

  if (headRotationsRemaining > 0) {
    // Guardian active state"
    currentMaxLedBrightness = MAX_LED_BRIGHTNESS;
    servoControl(currentTimestamp);
  } else {
    // Guardian sleeping state
    currentMaxLedBrightness = MAX_LED_BRIGHTNESS / 8;
    preventRetrigger = false;
  }

  ledFadeControl(currentTimestamp, currentMaxLedBrightness);
}
