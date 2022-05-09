#pragma once

#include "state_defs.h"
#include <stateobj.h>

//Need to charge Phone !

class CPayloadState: public CStateObj {
public:
  CPayloadState() {
  Name("Payload");
    //add some items to the map    
    //availablesystems["IMU"] = true;
    setMaxTime(2*TIMEORBIT);
  };
  ~CPayloadState() {};
  void stateMsg(CMsg &msg){_statemsg=msg;};
  void setup() {CStateObj::init();setMaxTime(2*TIMEORBIT);};

  void enter();
  void exit();
  void start();


};
