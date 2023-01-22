#pragma once

#include <stateobj.h>


class CLowPowerState: public CStateObj {
public:
	
	CLowPowerState(){};
	~CLowPowerState() {};

	void enter() { 
		CStateObj::enter();
		resetStateSubSystems();	
		}
	void exit()  {CStateObj::exit();}
	void loop(){ CStateObj::loop();delay(5000);}
};
