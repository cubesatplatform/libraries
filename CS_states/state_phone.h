#pragma once

#include "state_defs.h"
#include <stateobj.h>

//Need to charge Phone !

class CPhoneState: public CStateObj {
public:
  CPhoneState() {
  Name("PHONESTATE");
    //add some items to the map    
    //availablesystems["IMU"] = true;
    setMaxTime(2*TIMEORBIT);
  };
  ~CPhoneState() {};
  void stateMsg(CMsg &msg){_statemsg=msg;};
  void setup() {CStateObj::init();setMaxTime(2*TIMEORBIT);};

  void enter();
  void exit();
  void start();


};
