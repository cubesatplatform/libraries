#pragma once

#include "stateobj.h"


class CCoreState : public CStateObj {
  public:
  
  CCoreState() { Name("CORE"); setForever();};
  ~CCoreState() {};


  void setup();
  
};


   
