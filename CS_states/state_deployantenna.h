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
  
  	#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
		CFS fs;
		
		fs.deleteFile();
		fs.writeFile(_burncount);  
  	#endif
  

  }

};
