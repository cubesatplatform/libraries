#pragma once

#include "state_defs.h"
#include <portentafs.h>
#include <powerup.h>


#include <stateobj.h>


class CDeployAntennaState: public CStateObj {
public:
	int _burncount=0;
	CDeployAntennaState(){};
	~CDeployAntennaState() {};

	void enter() { 
		CStateObj::enter();
		resetSubSystems();
		_burncount++;
	
		CMsg m;
		m.setSYS("SAT");
		m.setACT("WRITECOUNTS");
		addMessageList(m);
		}

};
