#ifndef HEADER_LED_CONTROL
#define HEADER_LED_CONTROL

class LedControl {
public:
  LedControl();
  void setup(int);
  void update();
  void setFadeSpeed(int);
  void setMinBrightness(int);
  void setMaxBrightness(int);
  void fadeTo(int);
  void fadeIn();
  void fadeOut();
  void fadeToOtherState();
  int getBrightness();
  bool atMaxOrMin();

private:
  int _minBrightness;
  int _maxBrightness;
  int _desiredBrightness;
  int _fadeSpeed;
  int _pin;
  int _currentBrightness;
};

#endif
