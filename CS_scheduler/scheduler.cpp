
#include <messages.h>
#include "scheduler.h"



CScheduler::CScheduler(){
  Name("SCHEDULER");
  setForever();
  setInterval(3000);
  }; 
         
CScheduler::~CScheduler(){}  
  
   

void CScheduler::setup(){  
  setState("PLAY");
  }

void CScheduler::showCommands(){
  writeconsoleln("");
  writeconsoleln("ShowCommands xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");  
  
  std::list<CMsg> ml;
  std::string namestr;
  for (auto it = Commands.begin(); it != Commands.end(); ++it) {    
    namestr = it->first;
    writeconsoleln(namestr);
    ml=it->second;  
    for(auto m:ml){    
      m.writetoconsole();  
    }
  }
  writeconsoleln("");
}

void CScheduler::Output(CMsg &msg){
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

void CScheduler::loop(){
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


void CScheduler::addTask(CMsg &msg){
    CMsg m;
    m=msg;
    writeconsoleln("");
    writeconsoleln("--------------------System Manager.  Adding to Scheduler---------------------------");
    m.setSYS(msg.getParameter("_SYS"));
    m.setACT(msg.getParameter("_ACT"));
    m.setREFID();
    Scheduler.push_back(m);
  };

  
void CScheduler::deleteTask(CMsg &msg){
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {
    if(m->getREFID()==msg.getREFID()) {   //nEED SOME OTHER QUALIFIER
      m = Scheduler.erase(m);   //Check to make sure this works			 
      return;
    }
  }
};


void CScheduler::pauseTask(CMsg &msg){
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {
    if(m->getREFID()==msg.getREFID()) {   //nEED SOME OTHER QUALIFIER
      m->setParameter("PAUSE",1);   //Check to make sure this works			 
      return;
    }
  }
}

void CScheduler::unpauseTask(CMsg &msg){
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {
    if(m->getREFID()==msg.getREFID()) {   //nEED SOME OTHER QUALIFIER
      m->setParameter("PAUSE",0);   //Check to make sure this works			 
      return;
    }
  }
}




void CScheduler::SendCmdToScheduler(CMsg &msg){
  std::string str=msg.getParameter("CMD"); 
  std::list<CMsg> ml=Commands[str];
  for(auto m:ml){
    if(m.Parameters.size()){
      
      writeconsoleln(m.serializeout());
      Scheduler.push_back(m);
    }
  }
}




void CScheduler::showScheduler(CMsg &msg){
  writeconsoleln("");
  writeconsoleln("ShowScheduler xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");  
  for(auto m:Scheduler){    
    m.writetoconsole();  
    addTransmitList(m);
  }
   writeconsoleln("");
}


void CScheduler::scheduleData(CMsg &msg){   //Schedules geting data from map
  std::string sys=msg.getVALUE();
  long interval=msg.getParameter("INTERVAL",5000L);
  if (interval<1000) interval=1000;

  long stop=msg.getParameter("INTERVAL",60000L);

  if (sys.size()<=0)
    return;

  CMsg m;  
  std::list<CMsg> ml;
  m.setSYS(sys);
//  m.setACT("ECHODATA");
  m.setParameter("START",0L);
  m.setParameter("STOP",stop);
  m.setParameter("INTERVAL",interval);   
  ml.push_back(m);
  Commands[std::string("CMD_ECHO")+sys]=ml;
   
}





void CScheduler::controlPlan(){
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

void CScheduler::initSat(){  //SYS:MGR~ACT:COMMAND~CMD:CMD_BEACON
  CMsg m;
  std::list<CMsg> ml;
  
  ml.clear();

  m.clear();  
  m.setSYS("GPS");
  m.setACT("START");
  ml.push_back(m);

  m.clear();  
  m.setSYS("GPS");
  m.setACT("OUTPUT");
  m.setParameter("START",3000L);
  m.setParameter("STOP",30000L);
  m.setParameter("INTERVAL",1000000L);  
  ml.push_back(m);

  m.clear();  
  m.setSYS("GPS");
  m.setACT("STOP");  
  ml.push_back(m);

  Commands["CMD_GPS"]=ml;
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  ml.clear();

  m.clear();  
  m.setSYS("IRX1");
  m.setACT("START");
  ml.push_back(m);

  m.clear();  
  m.setSYS("IRX1");
  m.setACT("OUTPUT");
  m.setParameter("START",3000L);
  m.setParameter("STOP",300000L);
  m.setParameter("INTERVAL",10000L);   
  ml.push_back(m);

  m.clear();  
  m.setSYS("IRX1");
  m.setACT("STOP");
 
  ml.push_back(m);
  Commands["CMD_IRX1"]=ml;

  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  ml.clear();

  m.clear();  


  m.setSYS("MGR");
  m.setACT("ADDTASK");
  m.setParameter("_SYS","SAT");
  m.setParameter("_ACT","BEACON");   
  m.setParameter("INTERVAL",60000);
  m.setParameter("START",0);
  m.setParameter("STOP",(long) STOPTASKMAX);
  m.setREFID();
  addMessageList(m);
}

void CScheduler::callCustomFunctions(CMsg &msg){
  CSystemObject::callCustomFunctions(msg);
  CMessages* pMSG= getMessages();
  std::string act=msg.getACT(); 
  if(act=="ADDTASK")  {addTask(msg); return; }
  if(act=="DELETETASK")  {deleteTask(msg);   return; }
  if(act=="PAUSETASK")  {pauseTask(msg);   return; }
  if(act=="UNPAUSETASK")  {unpauseTask(msg);   return; }
  if(act=="SCHEDULEDATA") {scheduleData(msg); return; }
  if(act=="SHOWSCHEDULER") {showScheduler(msg); return; }
  if(act=="SHOWCOMMANDS") {showCommands(); return; }

  if(act=="COMMAND") {SendCmdToScheduler(msg);return;}    //Calls a predefined Command group    
}
