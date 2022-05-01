#pragma once

#include "state_defs.h"
#include <stateobj.h>


class CDeployAntennaState: public CStateObj {
public:
	int _burncount=0;
	CDeployAntennaState();
	~CDeployAntennaState() {};
	void stateMsg(CMsg &msg){_statemsg=msg;};
	void setup() {CStateObj::init();setMaxTime(DEPLOYMAXTIME);};

	void enter(); 
	void exit();

};
