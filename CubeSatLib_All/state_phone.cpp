 #include "state_phone.h"

 void CPhoneState::enter() { 
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
}
