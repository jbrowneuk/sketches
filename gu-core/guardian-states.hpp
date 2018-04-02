#ifndef HEADER_STATE_DEFINITIONS
#define HEADER_STATE_DEFINITIONS

#include "state-base.hpp"

class GuardianStateSleeping : public StateBase {
public:
  void update(StateController&, LedControl&, LedControl&, ServoControl&);
};

class GuardianStatePowerUp : public StateBase {
public:
  GuardianStatePowerUp();
  void update(StateController&, LedControl&, LedControl&, ServoControl&);

private:
  unsigned long _angleTimestamp;
  int _selectedAngleIndex;
  bool _allowLedOn;
};

class GuardianStateActive : public StateBase {
public:
  GuardianStateActive();
  void update(StateController&, LedControl&, LedControl&, ServoControl&);

private:
  unsigned long _initialTimestamp;
  short _directionChangesRemaining;
  bool _requiresFirstMovement;
};

class GuardianStatePowerDown : public StateBase {
public:
  void update(StateController&, LedControl&, LedControl&, ServoControl&);
};

#endif
