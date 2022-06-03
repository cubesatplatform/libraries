#include "framework_defs.h"
#include "systemobject.h"
#include "messages.h"


int CSystemObject::sid=0;


CSystemObject::CSystemObject() {
  sid++;
  _sid=sid;
  init();
  }

void CSystemObject::init(){
  success=false;

  _currentTime=getTime();
  _createdTime=getTime();
  _modifiedTime=getTime();
  _currentTime=getTime();
  _prevTime=0;
  
  _maxTime=TIMEORBIT;
  _minTime=1000;

  _startTime=_createdTime;
  _stopTime=_startTime+_maxTime;
  
  _minTime=0;

  
  _loopCount=0;
  _procCount=0;

  _ostate="";   //"" || "START","PLAY","PAUSE","STOP","ERROR" 
  _olaststate="";  
   
  _lastStateTime=0;  
  _lastUse=0;
  _retryCount=0;

  //_interval=0;// Don't clear the Mode on INIT !!!loop time interval in MicroSeconds,  0 means no delay
  //_mode="";   // Don't clear the Mode on INIT !!!This determines what you are doing   Set to IDLE when done and nothing left to do  When u set new mode need to get it out of IDLE
}

CSystemObject *getSystem(const char *sys,const char *comment){ 
  std::string sysstr=sys;
  std::string tmpstr=sys;
  std::string cstr;
  //if(tmpstr=="SAT") return NULL;
  CSystemObject * psys= SysMap[sysstr]; 
  char c='a';
  CMsg m;
    if (psys==NULL) {
      m.setINFO("Can't find system  (getSystem): ");      
      m.setCOMMENT(comment);

      for(auto x:SysMap){
        cstr=c;
        m.setParameter(cstr,x.first);
        c++;
      }
      m.setSYS(sys);
      for(int count=0;count<10;count++){
        cstr=c;
        m.setParameter(cstr,"BUG");
        c++;
        }
      return NULL;
    }
    return psys;   //Due to polymorphism, will return whatever system you want
}

CSystemObject *getIMU(const char *tmp){
  CSystemObject *pIMU=getSystem(tmp,"CSystemObject *getIMU(const char *tmp)");

  if(pIMU==NULL){
    pIMU=getSystem("IMUI2C");
  }

  if(pIMU==NULL){
  }
  return(pIMU);  
}

void CSystemObject::transmitError(const char *tmp){
  writeconsole("Critical Error: ");  writeconsoleln(tmp);
  CMsg msg;
  msg.setACT("TRANSMITDATA");
  msg.Parameters["ERROR"]=tmp;    

  addTransmitList(msg); 
} 




void CSystemObject::Name(std::string s) {
  /*
  if(_name.size()){
    auto it=SysMap.find(_name);
    if(it != SysMap.end()) SysMap.erase (it);   //May causing problem!!!
  }
  */
    
    _name = s;    
    CSystemObject *ptr=this; 
    if(s.size()&&(ptr!=NULL))
      SysMap[s]=ptr;        
  }
  

 void CSystemObject::fillMetaMSG(CMsg *m){
    _currentTime = getTime(); 
    if((m->getSYS()=="")&&(m->getSYS()!="RADIO"))m->setSYS(Name());
    if(m->getFROM()=="") m->setFROM(getIAM());
    if(m->getTO()=="") m->setTO("ALL");
    if(_cid.size())m->setCID(_cid);
 }

void CSystemObject::newMode(CMsg &msg){
  _cmsg=msg;
  _mode=msg.getACT();
  initMode();
}

void CSystemObject::stats(CMsg &msg){    
  CMsg m;

  m.setParameter("table","stats");

  m.setParameter("ostate",_ostate);
  m.setParameter("name",_name);
  m.setParameter("cid",_cid);
  m.setParameter("sid",_sid);
  m.setParameter("lastStateT",_lastStateTime);
  m.setParameter("createdT",_createdTime);
  m.setParameter("currentT",_currentTime);

  m.setParameter("prevT",_prevTime);
  m.setParameter("maxT", _maxTime);
  m.setParameter("minT",_minTime);
  m.setParameter("interval",_interval);
  m.setParameter("lastUse",_lastUse);

  m.setParameter("startT",_startTime);

  m.setParameter("loopCount",_loopCount);
  m.setParameter("procCount",_procCount);

  writeconsoleln(m.serializeout());

  addTransmitList(m);
}


void CSystemObject::newMsg(CMsg &msg){
 
  std::string act=msg.getACT();

  writeconsole(Name());writeconsoleln(":  Message received");
  if ((act!="MODEM") &&(act.substr(0,4)=="MODE")) { newMode(msg); return;} //Updates a parameter in the Subsystem
  //if (act == "SETMODE") { newMode(msg); return;} //Updates a parameter in the Subsystem

  if (act == "PLAY") {play();return;}
  if (act == "START") {start();return;}
  if (act == "OFF") {off();return;}
  if (act == "STATS") {stats(msg);return;}

  if ((Name()!="RADIO")&&(Name()!="RADIO2")){
    if (act == "PAUSE") {pause();return;}
    if (act == "STOP") {stop();return;}  
  }

  if(act=="SETUP") {setup();return;}
  if(act=="RUN") {Run(msg);return;}
  if(act=="INIT") {init();return;}
            
  if(act=="STATE") {State(msg);return;}
  if(act=="UPDATE") {Update(msg);return;}
  if(act=="CONFIG") {config(msg);return;}

  if(act=="ADDDATAMAP") {std::string key=msg.getKEY(); addDataMap(key,msg);return;}
  if(act=="ADDTRANSMITLIST") {addTransmitList(msg);return;}

  writeconsoleln("cllCustomFunctions");
  callCustomFunctions(msg);
}


void CSystemObject::Update(CMsg &msg){
  
  _forever = (bool) msg.getParameter("FOREVER",(int)_forever);
  _maxTime= msg.getParameter("MAXTIME",(unsigned long)_maxTime);
  _minTime= msg.getParameter("MINTIME",(unsigned long)_minTime);
  _interval= msg.getParameter("INTERVAL",(unsigned long)_interval);
  _startTime= msg.getParameter("STARTTIME",(unsigned long)_startTime);
  _stopTime= msg.getParameter("STOPTIME",(unsigned long)_stopTime);
  _ostate= msg.getParameter("OSTATE",_ostate);
  _mode= msg.getParameter("MODE",_mode);
  
}

void CSystemObject::setState(std::string str) { 
  _olaststate=_ostate;
  _ostate = str; 
  _lastStateTime=getTime(); 
  if(str=="ERROR") _retryCount++;  
  if(str=="PLAY") _retryCount=0;
  if (!((str=="")||(str=="START")||(str=="STOP")||(str=="PLAY")||(str=="ERROR")||(str=="PAUSE")||(str=="DONE"))){
    for (int count=0;count<20;count++){
      writeconsole("BAD STATE ");      writeconsoleln(str);
      }
  }
};      


std::string CSystemObject::outputStatus(long val) {
  CMsg m;
  std::string str;
  if (val==0) val=1;
  if (_procCount%val==0){
    
    m.setParameter("Proc",_sid);    
    m.setParameter("Name",_name);
    m.setParameter("Forever",_forever);

    m.setParameter("Interval",_interval);
    m.setParameter("Time",_currentTime);
        
    m.setParameter("State",_ostate);
    str=m.serializeout();     
    
  }
  return str;     
}
  

void CSystemObject::Run(long runtime){
  if(runtime>MAXRUNTIME){
    writeconsoleln("Runtime exceeds maxtime.  Exiting");
    return;
  }
  unsigned long endtime=getTime()+runtime;
  
  while(getTime()<endtime){    
    if((_currentTime+_interval)<getTime())
      Run();        
    
  }
}  

void CSystemObject::Run() {
    _procCount++;
    _currentTime = getTime();   
       

  if (_currentTime<_startTime) return;   //Do nothing as it shouldnt be activated yet
//timeSinceStateChange()  // Can use this to progress states
if (_ostate == "ERROR") {  //if ERROR, have it ertry a few times
    if(_interval<570) {
         _interval+=5;
      } 
    else {
        _ostate ="";     
        writeconsoleln(outputStatus(0));
    }        
    return;
  }

  if ((_ostate == "START")||(_ostate == "")) {  //None->On     
    start();
    return;
  }
 
  if (_ostate == "PLAY") { // Play->Stop
    play();
    return;
  }

  if (_ostate == "PAUSE") { // Pause    
    return;
  }

  if (_ostate == "STOP") { // Pause    
    off();
    return;
  }

  if (_ostate == "DONE") { // Do Nothing    
    return;
  }
}

bool CSystemObject :: outOfTime() {
  if (_forever)
    return false;
  if (((_currentTime - _createdTime) > _maxTime)&&((_currentTime - _createdTime) > _minTime)) {   //Play ->Out of Time
    writeconsole("Forever: ");
    writeconsoleln(_forever);
    CMsg m;
    stats(m);
    return true;
  }
  return false;

}

void CSystemObject::timeStamp() { 
  _currentTime = getTime(); 
  _loopCount++; 
}

void CSystemObject::sendError() { 
  CMsg m;
  setState("ERROR");
  m.setERROR(Name());
  m.setTIME(getTime());
  m.setCOMMENT("ERROR");
  m.setParameter("STATE",State());
  
  m.writetoconsole();
  addTransmitList(m);
}

bool CSystemObject::isNextCycle() {
  if (State()=="STOP")
    return false;  
  timeStamp();
  if (outOfTime()) {
    CMsg m;
    m.setSYS(Name()); 
    m.setINFO("Out of time.  Stopping.");
    writeconsoleln(m.serializeout());
    setState("STOP");
    return false;
  }
  
  if(State()!="PLAY"){
    if(_retryCount>30)return false;
    return true;
  }

  if(getTime()<_startTime){
    return false;
  }

  if (_currentTime >= _prevTime + _interval) {
    _prevTime = _currentTime;
    return true;
  }
  if(_interval==0)return true;
  return false;
}




void CSystemObject::State(CMsg &m) { 
  std::string s=m.getParameter("STATE");
  if ((_name=="RADIO")||(_name=="RADIO1")||(_name=="RADIO2")||(_name=="RADIO3")){
    writeconsoleln("-------------- NO STATE UPDATES TO RADIO -----------------------");    
  }
  else {
    setState(s); 
  }  
}

unsigned long CSystemObject::getReceivedTimestamp(){
  CMessages* pM = getMessages();return pM->getLastReceived();
  }



  
void CSystemObject::addTransmitList(CMsg &m ){
  CMessages* pM = getMessages();  
  fillMetaMSG(&m);    
  pM->addTransmitList(m);
  
}

void CSystemObject::addDataMap(std::string key,CMsg &m){
  CMessages* pM = getMessages();  
  if(key.size()>1)    
    pM->addDataMap(key,m);
  
}

CMsg CSystemObject::getDataMap(std::string key){
  CMessages* pM = getMessages();  
  CMsg m;
  if(key.size()>1)    
    m=pM->DataMap[key];
  return m;
}

void CSystemObject::addMessageList(CMsg &m ){
  CMessages* pM = getMessages();  
  pM->addMessageList(m);
  
}

void CSystemObject::addReceivedList(CMsg &m ){
  CMessages* pM = getMessages();  
  pM->addReceivedList(m,getIAM());
  pM->setReceivedTimestamp();

}

void CSystemObject::goLowPowerState(){
  if(getTime()>_lastLowPowerMsg+LOWPOWERDELAY){
    _lastLowPowerMsg=getTime();
    CMsg m;
    m.setSYS("SAT");
    m.setACT("LOWPOWER");
    addMessageList(m);
  }
}
