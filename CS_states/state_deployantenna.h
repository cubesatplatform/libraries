#pragma once

#include <stateobj.h>


class CDeployAntennaState: public CStateObj {
public:
	
	CDeployAntennaState(){};
	~CDeployAntennaState() {};

	void enter() { 
		CStateObj::enter();
		resetStateSubSystems();

		enablePin(_PINBURNWIRE);
  		delay(1000);
		disablePin(_PINBURNWIRE);
		

		CMsg msg=getDataMap(_SATINFO); 

		int burns=msg.get(_BURNS,0);
		burns++;
		addDataMap(_SATINFO,msg); 
	
		}
	void exit()  {CStateObj::exit();}
};
