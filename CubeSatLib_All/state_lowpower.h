#pragma once
#include "stateobj.h"

//delay will put Portenta in lower power mode, down .15W
class CLowPowerState : public CStateObj {
public:
	CLowPowerState() {
    Name("LOWPOWER");
		//add some items to the map
		
		//availablesystems["IMU"] = true;
	};
	~CLowPowerState() {};

	void setup() {CStateObj::init();};
	void enter() { CStateObj::enter();writeconsoleln("Enter Low Power"); }
  	void exit() { CStateObj::exit();writeconsoleln("Exit Low Power"); }

};
