#include  <string>
#include "system_mgr.h"
#include <messages.h>
#include "mgr_defs.h"
#include <powerup.h>

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

  
void CSystemMgr::deleteSchedule(CMsg &msg){
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {
    if((m->getACT()==msg.getACT())&&(1)) {   //nEED SOME OTHER QUALIFIER
      m = Scheduler.erase(m);   //Check to make sure this works			 
      break;
    }
  }


};

void CSystemMgr::newState(const char *str){
    CMsg msg;
    msg.setSYS("SAT");
    msg.setACT(str);
    addMessageList(msg);
  }
  

  


void CSystemMgr::SendCmd(CMsg &msg) {
  std::string str=msg.getACT(); 
  PinName  n = Pins[str];
  char action = 'H';
  
  if ((str[0] == 'H') || (str[0] == 'L') || (str[0] == 'W')) {
    action = str[0];
    str.erase(0, 1);
    writeconsole(">");
  }

  n = Pins[str];



  if (str == "RADIO") {
    CMsg m;
    m.setSYS("Radio");
    m.setINFO("setup");
    CRadio *pradio=(CRadio *)getSystem("RADIO", "Radio");
    if(pradio!=NULL) pradio->setup();
    writeconsoleln(m.serializeout());
    addTransmitList(m);
  return;
  }


  if (action == 'H') {   digitalWrite(n, HIGH);    return;  }
  if (action == 'L') {   digitalWrite(n, LOW);    return;  }

  if (action == 'W'){
    writeconsoleln("PWM");
    if (pwmPins.find(str) != pwmPins.end()) {
    
      #if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
        analogWriteResolution(n,PIN_RESOLUTION);   
      #endif
      for (int count=0;count<4000;count+=100){
        analogWrite(n,count);
        writeconsole("PWM: ");
        writeconsole(count);
        delay(200);
      }

    }  
    return;    
  }
 writeconsoleln("");
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

