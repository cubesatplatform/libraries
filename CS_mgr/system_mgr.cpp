#include  <string>
#include "system_mgr.h"
#include <messages.h>
#include "mgr_defs.h"
#include <powerup.h>
#include <ceps.h>
#include <radio.h>
#include <system_imu.h>

CSystemMgr::CSystemMgr(){
  Name("MGR");
  init();  
  }; 
         
CSystemMgr::~CSystemMgr(){}  
  
void CSystemMgr::init(){
  CSystemObject::init();
  setForever();
  setInterval(3000);
  initPins();
  initCommands();
  initI2CMap();

}
   

void CSystemMgr::SendCmdToScheduler(CMsg &msg){
  std::string str=msg.getACT(); 
  CMsg m;
  m.setSYS(Name());
  m.setINFO("SendCmdToScheduler: ");
  m.setCOMMENT(str);
  writeconsoleln(m.serializeout());
  std::list<CMsg> ml=Commands[str];
  for(auto m:ml){
    if(m.Parameters.size()){
      
      writeconsoleln(m.serializeout());
      Scheduler.push_back(m);
    }
  }
}

void CSystemMgr::setup(){
  init(); 
  setState("PLAY");
  }

void CSystemMgr::Output(CMsg &msg){
  std::string log, logfinal;
  for (auto m : Scheduler){
    log+=m.serialize();
    log+="\n";
  }

  for (auto c:log){
    if(c=='~')c='_';
    if(c==':')c='|';
    logfinal+=c;

  }
  CMsg cM;
  cM.setDATA(logfinal);
  cM.setTABLE("MGR");
  addTransmitList(cM);  
}

void CSystemMgr::loop(){

  //writeconsoleln("");  writeconsoleln("System Manager.   Scheduler Loop--------------------------------------------------------------------------------");
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {
    unsigned long start=0;
    unsigned long stop=0;  //Default to 0 so means run 1 time
    unsigned long last=0;
    unsigned long interval=10000;
    unsigned long currentTime=getTime();  
        
    start=m->getParameter("_START",start);
    stop=m->getParameter("_STOP",stop);
    interval=m->getParameter("_INTERVAL",interval);
    last=m->getParameter("_LAST",last);
    
    writeconsoleln(m->serialize());
    if(last==0){
      start=start+currentTime;
      stop=start+stop;     
      m->setParameter("_START",start);     
      m->setParameter("_STOP",stop);           
      }
     

    if(currentTime>(last+interval)){      
      last=currentTime;
      m->setParameter("_LAST",last);      
      addMessageList(*m);
    }

    if((currentTime>stop)||(start==stop)) {
      m = Scheduler.erase(m);   //Check to make sure this works			 
      continue;
    }
	}
  }


void CSystemMgr::addSchedule(CMsg &msg){
    CMsg m;
    m=msg;
    writeconsoleln("");
    writeconsoleln("System Manager.  Adding to Scheduler");
    m.setSYS(msg.getParameter("_SYS"));
    m.setACT(msg.getParameter("_ACT"));
    Scheduler.push_back(m);
  };

void CSystemMgr::newState(const char *str){
    CMsg msg;
    msg.setSYS("SAT");
    msg.setACT(str);
    addMessageList(msg);
  }
  


void CSystemMgr::controlPlan(){
        /*CSatellite *psat= getSatellite();
      CMsg msg;
      //_currentTime=getTime();
    
      if(!psat->_burncount&&psat-> deployantenna.stateCount()<2){
          msg.setSTATE("DEPLOY");
          addMessageList(msg);
      }
      else if(!psat->_detumblecount&psat->detumble.stateCount()<2){
          msg.setSTATE("DETUMBLE");
          addMessageList(msg);
      }
      
      else if((_currentTime>PHONE_DELAY)&&(_currentTime>+(_lastPhone+PHONE_DELAY_START))){
        msg.setSTATE("PHONE");
        _lastPhone=_currentTime;
        addMessageList(msg);        
      }
   
      else {
        msg.setSTATE("NORMAL");
        addMessageList(msg);        
      }
         */
}


 void CSystemMgr::callCustomFunctions(CMsg &msg){
  std::string act=msg.getACT(); 
  writeconsoleln("");
  writeconsole(" CallCustomFunctions :  ");
  writeconsoleln(act);  
  if(act=="CHKRADIO") chkRadio(msg);
  if(act=="CHKTEMPERATURE") chkTemperature(msg);
  if(act=="CHKBATTERY") chkBattery(msg);
  if(act=="CHKROTATION") chkRotation(msg);
  if(act=="CHKMESSAGES") chkMessages(msg);
  
  if(act=="SENDBEACON") sendBeacon(msg);
  if(act=="ADD")  addSchedule(msg);
  
  if(act=="ENABLEI2C") enableI2C();
  
  if(act=="SHOWTESTS") showTests();
  if(act=="SHOWCOMMANDS") showCommands();
  if(act=="SHOWSCHEDULER") showScheduler(msg);

  SendCmd(msg);
  SendCmdToScheduler(msg);
  writeconsoleln("");
}


void CSystemMgr::chkBattery(CMsg &msg){
  CEPS *pEPS=(CEPS *)getSystem("EPS");
  
  if(pEPS==NULL)
    return;
  CMsg m=pEPS->m;
  addTransmitList(m);  
}

void CSystemMgr::chkRadio(CMsg &msg){
  CMsg m;
  CRadio *pRadio=(CRadio *)getSystem("RADIO");
  CRadio *pRadio2=(CRadio *)getSystem("RADIO2");
  

  if(pRadio!=NULL){
    m.setParameter("RADIO_STATE",pRadio->State());
    m.setParameter("RADIO_INSTATE",pRadio->getRadioState());
    m.setParameter("RADIO_LASTTX",pRadio->getLastTransmit());
    m.setParameter("RADIO_LASTRX",pRadio->getLastReceive());    
    }

  if(pRadio2!=NULL){
    m.setParameter("RADIO2_STATE",pRadio2->State());
    m.setParameter("RADIO2_INSTATE",pRadio2->getRadioState());
    m.setParameter("RADIO2_LASTTX",pRadio2->getLastTransmit());
    m.setParameter("RADIO2_LASTRX",pRadio2->getLastReceive());    
    }    
  addTransmitList(m);  

}


void CSystemMgr::chkMessages(CMsg &msg){
  CMessages* pM=getMessages();
  CMsg m;

  m.setParameter("lastReceived",pM->getLastReceived());
  m.setParameter("lastMessage",pM->getLastMessage());
  m.setParameter("lastData",pM->getLastData());
  m.setParameter("lastTransmit",pM->getLastTransmit());

  addTransmitList(m);  
}



void CSystemMgr::chkRotation(CMsg &msg){
  CIMU *pIMU=(CIMU *)getIMU();
  if(pIMU!=NULL){
    CMsg m;
    pIMU->test(m);
    m=pIMU->Gyro.makeMessage("GYRO");
    addTransmitList(m);
  }

}


void CSystemMgr::sendBeacon(CMsg &msg){//Have the Satelite send this out  so this just messages it

}