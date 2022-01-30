 #include "state_phone.h"
 #include "powerup.h"

 void CPhoneState::enter() { 
  enablePhone();
  CStateObj::enter();
  writeconsoleln("Enter Phone  Need to charge Phone"); 
  CSystemObject * psys=getSystem("PHONE");
  if (psys!=NULL){
    psys->startTime(getTime()+PHONE_DELAY_START);
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
