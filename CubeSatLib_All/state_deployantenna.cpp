#include "state_deployantenna.h"
#include <portentafs.h>


CDeployAntennaState::CDeployAntennaState() {
  Name("DEPLOY");

/*
    #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
    CFS fs;
    //  fs.writeFile(100);
    //  fs.deleteFile();
    _burncount=fs.readFile();
    writeconsoleln(_burncount);

  #endif
*/
	};


void CDeployAntennaState::enter() { 
   
  CStateObj::enter();
  writeconsole("Enter Deploy Antenna"); 

  _burncount++;

  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
  CFS fs;
  //  fs.writeFile(100);
  //  fs.deleteFile();
  fs.writeFile(_burncount);
  
  #endif


  }
