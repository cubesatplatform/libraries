 #include "state_payload.h"
 #include <powerup.h>


void CPayloadState::start(){
  if(State()=="POWERUPDELAY"){
    if(getTime()>(startTime()+ANDROID_POWERUP_DELAY)){
      setState("PLAY");	      
    }  
  }	
	else if(getTime()>startTime()){
		setState("POWERUPDELAY");	
    disablePhone();    
	}  
}



void CPayloadState::enter() { 
  
  CStateObj::enter();
  enablePhone();
  writeconsoleln("Enter Payload  Need to charge Phone"); 
  
  _startTime = getTime();
	setState("PAUSE");

  CSystemObject * psys=getSystem("PHONE");
 
  startTime(getTime()+10000);
  
  if (psys!=NULL){
   
    psys->startTime(startTime()+10000);
    CMsg msg;
    std::string str="CPHOTO(F,90%,100,100,AUTO)";
    
    msg.setSYS("PHONE");
    msg.setACT(str);
    psys->addMessageList(msg);
    }        
 
 
}


void CPayloadState::exit() { 
  disablePhone();
  CStateObj::exit();
    
}
