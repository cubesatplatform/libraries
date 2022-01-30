#include "state_deployantenna.h"
#include <portentafs.h>
#include "powerup.h"


CDeployAntennaState::CDeployAntennaState() {
  Name("DEPLOY");


	};


void CDeployAntennaState::enter() { 
   
  CStateObj::enter();


  _burncount++;

  CMsg m;
  m.setTABLE("LOG");
  m.setINFO("Enter DeployAntenna");  
  

  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    



  enableBurnWire();
  delay(10000);
  disableBurnWire();


  CFS fs;
  
  fs.deleteFile();
  fs.writeFile(_burncount);


  m.setSYS("BURN");
  m.setINFO("ACTIVATED");
  writeconsoleln(m.serializeout());
  
  addTransmitList(m);

  m.setSYS("SAT");
  m.setACT("LOWPOWER");
  addMessageList(m);

  
  #endif

  }


void CDeployAntennaState::exit() { 
   
  CStateObj::exit();
  
  CMsg m;
  m.setTABLE("LOG");
  m.setINFO("Exit DeployAntenna");  
  
  addTransmitList(m);
  writeconsoleln(m.serializeout()) ;

  

  }
