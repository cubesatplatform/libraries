#pragma once

#include "stateobj.h"


class CNormalState : public CStateObj {  //Normal let everything through
private:

public:
	CNormalState() {
		//add some items to the map
   	Name("NORMAL");
	setMaxTime(3*TIMEORBIT);
  	availablesystems["GPS"] = true;
  	availablesystems["DATAREQUEST"] = true;
		
	};
	~CNormalState() {};

	void setup() {CStateObj::init();setMaxTime(3*TIMEORBIT);};
	void enter() { CStateObj::enter();writeconsoleln("Enter Normal"); resetSubSystems();setState("PLAY");}
	void exit() { CStateObj::exit();writeconsoleln("Exit Normal"); setState("PAUSE");}
	

};
