#pragma once

#include <stateobj.h>

//Need to charge Phone !

class CPayloadState: public CStateObj {
public:
  CPayloadState() {  
    //add some items to the map    

    setMaxTime(2*TIMEORBIT);
  };
  ~CPayloadState() {};

  void setup() {CStateObj::init();setMaxTime(2*TIMEORBIT);};

  void enter();
  void exit();
  void start();


};
