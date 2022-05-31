
#include "state_defs.h"
#include "state_detumble.h"
#include <portentafs.h>
#include <powerup.h>


CDetumbleState::CDetumbleState() {
  Name("DETUMBLE");
  setMaxTime(3*TIMEORBIT);

};

CDetumbleState::~CDetumbleState() {};

void CDetumbleState::setup() {};

void CDetumbleState::stateMsg(CMsg &msg){_statemsg=msg;};

void CDetumbleState::enter() {
  enable65V();
  resetSubSystems();
  _detumblecount++;
  CStateObj::enter();
  
    #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
    CFS fs;

    fs.setFilename(DETUMBLE_FILE);
    fs.deleteFile();
    fs.writeFile(_detumblecount);    
    #endif
  setState("PLAY");
  CSystemObject *pMT=getSystem("MT");
  if (pMT!=NULL) pMT->setState("");  //Reset MT and gets it restarting detumbling
  }
  
void CDetumbleState::exit() { 
  disable65V();
  CStateObj::exit();
  }
