#include  <string>
#include "system_mgr.h"
#include "messages.h"
#include "defs.h"
#include "powerup.h"


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
  /*
  chkRadio();
  chkCurrentState();
  chkAllStates();
  chkTemperature();
  chkBattery();
  chkCoreSystems();
  chkMessages();
  
  sendHealth();
  sendBeacon();
  */
 
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

 void CSystemMgr::powerLevel(CMsg &msg){
    float bat=msg.getParameter("LEVEL",0.0);

    if(bat>60){
      controlPlan();
    }  
 }

 void CSystemMgr::callCustomFunctions(CMsg &msg){
  std::string act=msg.getACT(); 
  writeconsoleln("");
  writeconsole(" CallCustomFunctions :  ");
  writeconsoleln(act);
  if(act=="CHKCLOCKS") chkClocks();
  if(act=="CHKRADIO") chkRadio();
  if(act=="CHKCURRENTSTATE") chkCurrentState();
  if(act=="CHKALLSTATES") chkAllStates();
  if(act=="CHKTEMPERATURE") chkTemperature();
  if(act=="CHKBATTERY") chkBattery();
  if(act=="POWERLEVEL") powerLevel(msg);
  if(act=="CHKROTATION") chkRotation();
  if(act=="CHKCORESYSTEMS") chkCoreSystems();
  if(act=="CHKMESSAGES") chkMessages();
  
  if(act=="SENDHEALTH") sendHealth();
  if(act=="SENDBEACON") sendBeacon();
  if(act=="ADD")  addSchedule(msg);
  
  if(act=="ENABLEI2C") enableI2C();
  
  if(act=="SHOWTESTS") showTests();
  if(act=="SHOWCOMMANDS") showCommands();
  if(act=="SHOWSCHEDULER") showScheduler();

  SendCmd(msg);
  SendCmdToScheduler(msg);
  writeconsoleln("");
}
