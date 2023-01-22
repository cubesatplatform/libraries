#include <actuator_defs.h>
#include "state_detumble.h"


CDetumbleState::CDetumbleState() {  
  setMaxTime(3*TIMEORBIT);
};

CDetumbleState::~CDetumbleState() {};

void CDetumbleState::setup() {};


void CDetumbleState::enter() {

  resetStateSubSystems();
  
  CStateObj::enter();

  std::string mode=_m.get(_MODE,_DETUMBLE);
  
  if(mode==_DETUMBLE){
	  CMsg msg=getDataMap(_SATINFO); 

    int detumbles=msg.get(_DETUMBLE,0);
    detumbles++;
    addDataMap(_SATINFO,msg); 


    setState(_PLAY);
  
    CMsg m;
    m.set(_SYS,_MAGX);
    m.set(_MODE,_DETUMBLE);   //Reset MT and gets it restarting detumbling
    addMessageList(m);

    m.set(_SYS,_MAGY);
    m.set(_MODE,_DETUMBLE);   //Reset MT and gets it restarting detumbling
    addMessageList(m);
     
    m.set(_SYS,_MAGZ);
    m.set(_MODE,_DETUMBLE);   //Reset MT and gets it restarting detumbling
    addMessageList(m);
  }

  if(mode==_MANUAL){
    setState(_PLAY);
  
    CMsg m;
    m.set(_SYS,_MAGX);
    m.set(_MODE,_MANUAL);   //Reset MT and gets it restarting detumbling
    addMessageList(m);

    m.set(_SYS,_MAGY);
    m.set(_MODE,_MANUAL);   //Reset MT and gets it restarting detumbling
    addMessageList(m);
     
    m.set(_SYS,_MAGZ);
    m.set(_MODE,_MANUAL);   //Reset MT and gets it restarting detumbling
    addMessageList(m);
  }
}
  
void CDetumbleState::exit() { 

  CStateObj::exit();
  }
