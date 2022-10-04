#include  <string>
#include "system_mgr.h"
#include <messages.h>
#include "mgr_defs.h"
#include <powerup.h>

#include <radio.h>
#include <system_imu.h>




CSystemMgr::CSystemMgr(){
  Name("MGR");
  setForever();
  setInterval(3000);
  }; 
         
CSystemMgr::~CSystemMgr(){}  
  
void CSystemMgr::init(){  
  initPins();
  initCommands();
  initI2CMap();
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
  addTransmitList(cM);  
}

void CSystemMgr::loop(){
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {    
    unsigned long start=0;
    unsigned long stop=0;  //Default to 0 so means run 1 time
    unsigned long last=0;
    unsigned long interval=10000;
    unsigned long currentTime=getTime();  
    int tmpPause=m->getParameter("PAUSE",0);

    
    if(tmpPause)
      continue;
        
    start=m->getParameter("START",start);
    stop=m->getParameter("STOP",stop);
    interval=m->getParameter("INTERVAL",interval);
    last=m->getParameter("LAST",last);

    
    
    if(last==0){
      start=start+currentTime;
      if(stop<STOPTASKLIMIT)
        stop=start+stop;     
      else 
        stop=STOPTASKMAX;
      m->setParameter("START",start);     
      m->setParameter("STOP",stop);           
      m->setParameter("LAST",start);     
      }   
     

    if(currentTime>(last+interval)){      
      last=currentTime;
      m->setParameter("LAST",last);      
      m->writetoconsole();
      //writeconsoleln(m->serializeout());
      addMessageList(*m);
    }

    
    if((currentTime>stop)||(start==stop)) {
      writeconsoleln("----------------Erasing   Scheduler Loop--------------------------------------------------------------------------------");
      writeconsoleln(currentTime);
      writeconsoleln(start);
      writeconsoleln(stop);
      m->writetoconsole();
      m = Scheduler.erase(m);   //Check to make sure this works			 
      continue;
    }
	}
  }


void CSystemMgr::addTask(CMsg &msg){
    CMsg m;
    m=msg;
    writeconsoleln("");
    writeconsoleln("--------------------System Manager.  Adding to Scheduler---------------------------");
    m.setSYS(msg.getParameter("_SYS"));
    m.setACT(msg.getParameter("_ACT"));
    m.setREFID();
    Scheduler.push_back(m);
  };

  
void CSystemMgr::deleteTask(CMsg &msg){
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {
    if(m->getREFID()==msg.getREFID()) {   //nEED SOME OTHER QUALIFIER
      m = Scheduler.erase(m);   //Check to make sure this works			 
      return;
    }
  }
};


void CSystemMgr::pauseTask(CMsg &msg){
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {
    if(m->getREFID()==msg.getREFID()) {   //nEED SOME OTHER QUALIFIER
      m->setParameter("PAUSE",1);   //Check to make sure this works			 
      return;
    }
  }
}

void CSystemMgr::unpauseTask(CMsg &msg){
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {
    if(m->getREFID()==msg.getREFID()) {   //nEED SOME OTHER QUALIFIER
      m->setParameter("PAUSE",0);   //Check to make sure this works			 
      return;
    }
  }
}




void CSystemMgr::SendCmdToScheduler(CMsg &msg){
  std::string str=msg.getParameter("CMD"); 
  std::list<CMsg> ml=Commands[str];
  for(auto m:ml){
    if(m.Parameters.size()){
      
      writeconsoleln(m.serializeout());
      Scheduler.push_back(m);
    }
  }
}



















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

