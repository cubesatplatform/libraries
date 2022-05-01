 #include "state_phone.h"
 #include <powerup.h>


void CPhoneState::start(){
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



void CPhoneState::enter() { 
  enablePhone();
  CStateObj::enter();
  writeconsoleln("Enter Phone  Need to charge Phone"); 
  
  _startTime = getTime();
	setState("PAUSE");

  CSystemObject * psys=getSystem("PHONE");
 // startTime(getTime()+PHONE_DELAY_START);
  startTime(getTime()+10000);
  
  if (psys!=NULL){
   
    psys->startTime(startTime()+10000);
    CMsg msg;
    std::string str="CPHOTO(F,90%,100,100,AUTO)";
    
    msg.setSYS("PHONE");
    msg.setACT(str);
    psys->addMessageList(msg);
    }        
  CMsg m;
  m.setTABLE("LOG");
  m.setINFO("Enter Phone");  
  writeconsoleln(m.serializeout()) ;
  addTransmitList(m);
 
}


void CPhoneState::exit() { 
  disablePhone();
  CStateObj::exit();
    
  CMsg m;
  m.setTABLE("LOG");
  m.setINFO("Exit Phone");  
  writeconsoleln(m.serializeout()) ;
  addTransmitList(m);
}
