#ifndef HEADER_STATE_DEFINITIONS
#define HEADER_STATE_DEFINITIONS

#include "state-base.hpp"

class GuardianStateSleeping : public StateBase {
public:
  void update(StateController&, LedControl&, LedControl&, ServoControl&);
};

// The in-game decayed Guardian start up sequence involves its runes lighting up red while the following happens:
// - The head moves up (not possible in my build)
// - The head twitches randomly a few degrees left and right from 90
// - the eye then flickers on rapidly
class GuardianStatePowerMotor : public StateBase {
public:
  GuardianStatePowerMotor();
  void update(StateController&, LedControl&, LedControl&, ServoControl&);

private:
  unsigned long _angleTimestamp;
  int _selectedAngleIndex;
};

class GuardianStatePowerLed : public StateBase {
public:
  void update(StateController&, LedControl&, LedControl&, ServoControl&);
};

class GuardianStateSearching : public StateBase {
public:
  GuardianStateSearching();
  void update(StateController&, LedControl&, LedControl&, ServoControl&);

private:
  short _directionChangesRemaining;
  bool _requiresFirstMovement;
};

class GuardianStateActive : public StateBase {
public:
  GuardianStateActive();
  void update(StateController&, LedControl&, LedControl&, ServoControl&);

private:
  unsigned long _initialTimestamp;
};

class GuardianStatePowerDown : public StateBase {
public:
  void update(StateController&, LedControl&, LedControl&, ServoControl&);
};

#endif
