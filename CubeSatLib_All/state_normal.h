#pragma once

#include "stateobj.h"


class CNormalState : public CStateObj {  //Normal let everything through
private:

public:
	CNormalState() {
		//add some items to the map
   Name("NORMAL");
	
  availablesystems["GPS"] = true;
  availablesystems["DATAREQUEST"] = true;
		
	};
	~CNormalState() {};

	void setup() {CStateObj::init();};
	void enter() { CStateObj::enter();writeconsoleln("Enter Normal"); setState("PLAY");}
	void exit() { CStateObj::exit();writeconsoleln("Exit Normal"); setState("PAUSE");}
	

};
