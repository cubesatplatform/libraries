#pragma once
#include "stateobj.h"


class CDetumbleState : public CStateObj {
public:
  int _detumblecount=0;
  CDetumbleState();
  ~CDetumbleState();

  void setup();
  void stateMsg(CMsg &msg);
  void enter();
  void exit();
  
};
