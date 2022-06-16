
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
  
  CMsg m;
  m.setSYS("SAT");
  m.setACT("WRITECOUNTS");
  addMessageList(m);

  setState("PLAY");
  CSystemObject *pMT=getSystem("MT");
  if (pMT!=NULL) pMT->setState("");  //Reset MT and gets it restarting detumbling
  }
  
void CDetumbleState::exit() { 
  disable65V();
  CStateObj::exit();
  }
