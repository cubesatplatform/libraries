
#include "state_defs.h"
#include "state_detumble.h"
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
  
  CStateObj::enter();
  
	CMsg msg=getDataMap(std::string("SAT")); 

  int detumbles=msg.getParameter("DETUMBLES",0);
  detumbles++;
  addDataMap(std::string("SAT"),msg); 


  setState("PLAY");
  CSystemObject *pMT=getSystem("MT");
  if (pMT!=NULL) pMT->setState("");  //Reset MT and gets it restarting detumbling
  }
  
void CDetumbleState::exit() { 
  disable65V();
  CStateObj::exit();
  }
