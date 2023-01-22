#pragma once

#include <stateobj.h>

#define DETUMBLES "DETUMBLES"

class CDetumbleState : public CStateObj {
public:
  int _detumblecount=0;
  CDetumbleState();
  ~CDetumbleState();

  void setup();
  
  void enter();
  void exit();
  
};
