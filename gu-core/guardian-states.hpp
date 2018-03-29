#ifndef HEADER_STATE_DEFINITIONS
#define HEADER_STATE_DEFINITIONS

#include "state-base.hpp"

class GuardianStateSleeping : public StateBase {
public:
  void update(StateController&);
};

class GuardianStatePowerUp : public StateBase {
public:
  void update(StateController&);
};

class GuardianStateActive : public StateBase {
public:
  void update(StateController&);
};

class GuardianStatePowerDown : public StateBase {
public:
  void update(StateController&);
};

#endif
