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
  initCommands();

}
   


  

void CSystemMgr::SendCmdToScheduler(std::string str){
  writeconsoleln("");
  writeconsoleln("SendCmdToScheduler: ");
  writeconsoleln(str);
  std::list<CMsg> ml=Commands[str];
  for(auto m:ml){
    if(m.Parameters.size()){
      writeconsole("CMD>");
      writeconsoleln(m.serialize());
      Scheduler.push_back(m);
    }
  }
writeconsoleln("");
}

void CSystemMgr::setup(){
  init(); 
  setState("PLAY");
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
      writeconsoleln("Step> A     last==0");
      start=start+currentTime;
      stop=start+stop;     
      m->setParameter("_START",start);     
      m->setParameter("_STOP",stop);           
      }
     

    if(currentTime>(last+interval)){      
      writeconsoleln("Step> B     currentTime>(last+interval)");
      last=currentTime;
      m->setParameter("_LAST",last);      
      addMessageList(*m);
    }

    if((currentTime>stop)||(start==stop)) {
      writeconsoleln("Step> C  Erasing   (currentTime>stop)||(start==stop)");       
      m = Scheduler.erase(m);   //Check to make sure this works			 
			//break;		
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
  //writeconsoleln("System Manager.   Scheduler Loop  End--------------------------------------------------------------------------------");
    
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

  SendCmd(act);
  SendCmdToScheduler(act);
  writeconsoleln("");
}
