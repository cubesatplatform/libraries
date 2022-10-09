#pragma once

#include "state_defs.h"
#include <powerup.h>


#include <stateobj.h>


class CDeployAntennaState: public CStateObj {
public:
	
	CDeployAntennaState(){};
	~CDeployAntennaState() {};

	void enter() { 
		CStateObj::enter();
		resetSubSystems();

		enableBurnWire();
  		delay(10000);
		disableBurnWire();
		

		CMsg msg=getDataMap(std::string("SAT")); 

		int burns=msg.getParameter("BURNS",0);
		burns++;
		addDataMap(std::string("SAT"),msg); 
	
		}
	void exit()  {}
};
