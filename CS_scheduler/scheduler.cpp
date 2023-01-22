#include "scheduler.h"


/*
Ussage:
SYS:SCHEDULER~ACT:ADDTASK~S_SYS:IMUI~S_ACT:SENDDATA~N:IMUI_GYRO~S_STOP:2000000
*/



CScheduler::CScheduler(){
  setForever();
  setInterval(300);
  }; 
         
CScheduler::~CScheduler(){}  
  

   

void CScheduler::setup(){  
  setState(_PLAY);
  }



void CScheduler::output(CMsg &msg){
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
  cM.set(_DATA,logfinal);  
  addTransmitList(cM);  
}

void CScheduler::loop(){
  if(Scheduler.size()<1){
    return;
  }

  CMsg msat=getDataMap(_SATINFO);
  std::string satstate=msat.get(_STATE);
  
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {        
    std::string mstate=m->get(_STATE);
    if((mstate.length()>1)&& (satstate.length()>1)){    
      if(mstate!=satstate){   //Only send to appropriate state if defined
        writeconsoleln("Schedule States Different   Not Sending");
        writeconsoleln(mstate);
        writeconsoleln(satstate);
        continue;
      }
    }
    
    long start=0;
    long stop=0;  //Default to 0 so means run 1 time
    long last=0;
    long interval=10000;
    long currentTime=getTime();  
    int tmpPause=m->get(_PAUSE,0);

    
    if(tmpPause)
      continue;
        
    
    start=m->get(_START,start);
    stop=m->get(_STOP,stop);
    interval=m->get(_INTERVAL,interval);
    last=m->get(_LAST,last);

    
    
    if(last==0){      
      start=start+currentTime;
      last=start;
      if(stop<STOPTASKLIMIT)
        stop=start+stop;     
      else 
        stop=STOPTASKMAX;
      m->set(_START,start);     
      m->set(_STOP,stop);           
      m->set(_LAST,last);                 
      }   


    
    if(currentTime>(last+interval)){      
      last=currentTime;
      m->set(_LAST,last);      
                
      CMsg mm=convertTasktoMessage(*m);    
      addMessageList(mm);      
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

CMsg CScheduler::convertTasktoMessage(CMsg &msg){
  CMsg m;  
  for(auto x:msg.Parameters){
    std::string key=x.first;
    if (key.length()>2){
      if (key.substr(0,2)=="S_"){
        m.set(key.substr(2,key.length()),x.second);
      }
    }

  }
  return m;
}


CMsg CScheduler::convertMessagetoTask(CMsg &msg){
  CMsg m;  
  for(auto x:msg.Parameters){
    std::string key=x.first;
    if (key.length()>2){
      if (key.substr(0,2)!="S_"){
        key="S_"+key;
        m.set(key,x.second);
      }
    }

  }
  return m;
}


void CScheduler::addTask(CMsg &msg){

    Scheduler.push_back(msg);
  };

  
void CScheduler::deleteTask(CMsg &msg){
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {
    if(m->get(_REFID)==msg.get(_REFID)) {   //nEED SOME OTHER QUALIFIER
      m = Scheduler.erase(m);   //Check to make sure this works			 
      return;
    }
  }
};

void CScheduler::deleteAll(CMsg &msg){
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {    
    m = Scheduler.erase(m);   //Check to make sure this works			     
  }
};



void CScheduler::pauseTask(CMsg &msg){
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {
    if(m->get(_REFID)==msg.get(_REFID)) {   //nEED SOME OTHER QUALIFIER
      m->set(_PAUSE,1);   //Check to make sure this works			 
      return;
    }
  }
}

void CScheduler::unpauseTask(CMsg &msg){
  for (auto m = Scheduler.begin(); m != Scheduler.end(); m++) {
    if(m->get(_REFID)==msg.get(_REFID)) {   //nEED SOME OTHER QUALIFIER
      m->set(_PAUSE,0);   //Check to make sure this works			 
      return;
    }
  }
}








void CScheduler::showScheduler(CMsg &msg){
  writeconsoleln("");  
  for(auto m:Scheduler){    
    m.writetoconsole();  
    addTransmitList(m);
  }
   writeconsoleln("");
}








void CScheduler::schedule(std::string sys,std::string act, long interval, long start, long stop){
  CMsg m;
  m.set(_SYS,_SCHEDULER);
  m.set(_ACT,_ADDTASK);  
  m.set(_INTERVAL,interval);
  m.set(_START,start);
  m.set(_STOP,stop);
  m.set(S_SYS,sys);
  m.set(S_ACT,act);   
  m.setREFID();
  addTask(m);
}

void CScheduler::schedule(CMsg &msg, long interval, long start, long stop){
  CMsg m=convertMessagetoTask(msg);
  m.set(_SYS,_SCHEDULER);
  m.set(_ACT,_ADDTASK);  
  m.set(_INTERVAL,interval);
  m.set(_START,start);
  m.set(_STOP,stop); 
  m.setREFID();
  addTask(m);
}




void CScheduler::callCustomFunctions(CMsg &msg){
  
  std::string act=msg.get(_ACT); 

  mapcustom(initSat)
  
  mapcustommsg(addTask)
  mapcustommsg(deleteTask)
  mapcustommsg(deleteAll)
  mapcustommsg(pauseTask)
  mapcustommsg(unpauseTask)
  mapcustommsg(showScheduler)
  
  CSystemObject::callCustomFunctions(msg);
}






void CScheduler::initSat(){


  
  long interval=10000;

/*
  schedule(_MANAGER,_SENDBEACON,1*interval);
  schedule(_MANAGER,_CHKBATTERY,5*interval);
 
  schedule(_MANAGER,_CHKMAGFIELD,5*interval);
  schedule(_MANAGER,_CHKRADIO,15*interval);
  schedule(_MANAGER,_CHKMESSAGES,5*interval);
  
  
  schedule(_MANAGER,_CHKROTATION,5*interval); 
*/  
  schedule(_MANAGER,_RANDOMSTATE,3*interval);
}


void CScheduler::controlPlan(){
  /*      
        CSatellite *psat= getSatellite();
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

