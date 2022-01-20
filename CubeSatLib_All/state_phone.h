#pragma once
#include "stateobj.h"

//Need to charge Phone !

class CPhoneState: public CStateObj {
public:
  CPhoneState() {
  Name("PHONESTATE");
    //add some items to the map    
    //availablesystems["IMU"] = true;
  };
  ~CPhoneState() {};
  void stateMsg(CMsg &msg){_statemsg=msg;};
  void setup() {CStateObj::init();};

  void enter();
  void exit();


};
