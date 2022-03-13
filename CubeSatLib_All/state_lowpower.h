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
	void enter() { 
		CStateObj::enter();
		CMsg m;
		m.setTABLE("LOG");
		m.setINFO("Enter LowPower");  
		writeconsoleln(m.serializeout()) ;
		addTransmitList(m);
		setState("PLAY");
		}

  	void exit() { 
		CStateObj::exit();
		CMsg m;
		m.setTABLE("LOG");
		m.setINFO("Exit LowPower");  
		writeconsoleln(m.serializeout()) ;
		addTransmitList(m);
		}

};
