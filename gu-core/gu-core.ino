#include <Servo.h>

const int ledPin = 5;
const int servoPin = 9;
const int buttonPin = 7;

const int fadeAmount = 2;
const int servoAmount = 2;

const int maxRotationChanges = 6;
const int maxRotation = 180;
const int maxBrightness = 255;

const long ledInterval = 25;
const long servoInterval = 15;

unsigned long ledLastMsec = 0;
unsigned long servoLastMsec = 0;

int brightness = fadeAmount;
int currentFadeAmount = fadeAmount;

int servoPosition = 0;
int currentMovementAmount = servoAmount;

int buttonState = 0;
short rotateCount = 0;

bool locked = false;

Servo mainServo;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  mainServo.attach(servoPin);
  servoPosition = mainServo.read();
}

/*
 *
 */
void ledFadeControl(unsigned long currentMillis) {
  if (currentMillis - ledLastMsec < ledInterval) {
    return;
  }

  ledLastMsec = currentMillis;

  analogWrite(ledPin, brightness);
  brightness += currentFadeAmount;

  if (brightness < fadeAmount || brightness > maxBrightness - fadeAmount) {
    currentFadeAmount = -currentFadeAmount;
  }
}

/*
 *
 */
void servoControl(unsigned long currentMillis) {
  if (currentMillis - servoLastMsec < servoInterval) {
    return;
  }

  servoLastMsec = currentMillis;

  mainServo.write(servoPosition);
  servoPosition += currentMovementAmount;

  if (servoPosition < servoAmount || servoPosition > maxRotation - servoAmount) {
    currentMovementAmount = -currentMovementAmount;
  }
}

/*
 *
 */
void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH && !locked) {
    rotateCount = maxRotationChanges;
    brightness = maxBrightness;
    currentFadeAmount = -fadeAmount;
    analogWrite(ledPin, brightness);
    locked = true;
  }

  unsigned long currentMillis = millis();
  if (rotateCount > 0) {
    servoControl(currentMillis);
  } else {
    ledFadeControl(currentMillis);
    locked = false;
  }
}
