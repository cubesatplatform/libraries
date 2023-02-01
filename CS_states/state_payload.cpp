#include "state_payload.h"


void CPayloadState::enter() {    //Need to charge Phone for 10 minutes before we can use it.  Then turn off power to phone to anable Serial1 to work

  CStateObj::enter();
  enablePin(_PINPHONE);
  writeconsoleln("Enter Payload  Need to charge Phone"); 
  
  _obj._startTime = getTime()+ANDROID_POWERUP_DELAY;
	setState(_PAUSE);

  
}


void CPayloadState::exit() { 
  
  CStateObj::exit();
  disablePin(_PINPHONE);
    
}



void CPayloadState::start(){      //Then turn off power to phone to anable Serial1 to work
  if(getTime()>(startTime()+ANDROID_POWERUP_DELAY)){
    disablePin(_PINPHONE);
    setState(_PLAY);	      


    CMsg m;
    m.set(_SYS,_PHONE );
    m.set(_ACT,_SENDSERIAL);
    m.set(_VALUE,"CINIT");
    addMessageList(m);
  }  
}