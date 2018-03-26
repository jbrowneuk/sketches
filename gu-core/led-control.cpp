#include "led-control.hpp"
#include "Arduino.h"

const int DEFAULT_FADE_SPEED = 4;

LedControl::LedControl() {
  _minBrightness = 0;
  _maxBrightness = MaxBrightness;
  _fadeSpeed = DEFAULT_FADE_SPEED;
}

void LedControl::setup(int pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void LedControl::update() {
  // TODO next line requires min value too
  const int safeDesiredBrightness = _desiredBrightness >= _maxBrightness ? _maxBrightness : _desiredBrightness;
  if (_currentBrightness == safeDesiredBrightness) {
    return;
  }

  // Add tolerances to prevent flickering
  const int lowerBound = safeDesiredBrightness - _fadeSpeed;
  const int upperBound = safeDesiredBrightness + _fadeSpeed;

  if (_currentBrightness < lowerBound) {
    _currentBrightness = _currentBrightness + _fadeSpeed;
  } else if (_currentBrightness > upperBound) {
    _currentBrightness = _currentBrightness - _fadeSpeed;
  } else {
    _currentBrightness = safeDesiredBrightness;
  }

  analogWrite(_pin, _currentBrightness);
}

int constrainBrightnessPositive(int value) {
  return constrain(abs(value), 0, LedControl::MaxBrightness);
}

void LedControl::setFadeSpeed(int speed) {
  _fadeSpeed = constrainBrightnessPositive(speed);
}

void LedControl::setMinBrightness(int minBrightness) {
  _minBrightness = constrainBrightnessPositive(minBrightness);
}

void LedControl::setMaxBrightness(int minBrightness) {
  _maxBrightness = constrainBrightnessPositive(minBrightness);
}

void LedControl::fadeTo(int brightness) {
  _desiredBrightness = constrainBrightnessPositive(brightness);
}

void LedControl::fadeIn() {
  _desiredBrightness = _maxBrightness;
}

void LedControl::fadeOut() {
  _desiredBrightness = _minBrightness;
}

void LedControl::fadeToOtherState() {
  const int range = _maxBrightness - _minBrightness;
  if (_currentBrightness < range / 2) {
    fadeIn();
  } else {
    fadeOut();
  }
}

int LedControl::getBrightness() {
  return _currentBrightness;
}

bool LedControl::atMaxOrMin() {
  const int lowerBound = _minBrightness + _fadeSpeed;
  const int upperBound = _maxBrightness - _fadeSpeed;
  return _currentBrightness < lowerBound || _currentBrightness > upperBound;
}
