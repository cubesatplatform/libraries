
#include "systemobject.h"
#include "messages.h"
 #include <arduino.h>
//stateNone->switchOn()->stateOn|StateError
//stateOn->setup()->->stateReady|StateError
//stateReady->switchPlay()->statePlay|StateError
//statePlay->loop()
//

#define MAXLISTSIZE 100
int CSystemObject::sid=0;


CSystemObject::CSystemObject() {
  sid++;
  _sid=sid;
  _ostate = "NONE"; success = false; _createdTime = 0; _currentTime = 0;  _INTERVAL = 0; _loopCount = 0; _prevTime = 0; _maxTime = 200000; _transmitINTERVAL = 5000; _transmitPREVTIME = 0;
  }

 void CSystemObject::fillMetaMSG(CMsg *m){
    _currentTime = getTime(); 
    m->CID(_cid);
 }

void CSystemObject::newMode(CMsg &msg){
  _cmsg=msg;
  _Mode=msg.getParameter("MODE");
  initMode();
}

 void CSystemObject::stats(){
    
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
  m.setParameter("INTERVAL",_INTERVAL);
  m.setParameter("lastUse",_lastUse);

  m.setParameter("startT",_startTime);
  m.setParameter("tINTERVAL",_transmitINTERVAL);
  m.setParameter("tPREVT",_transmitPREVTIME);
  m.setParameter("loopCount",_loopCount);
  m.setParameter("procCount",_procCount);
  
  addTransmitList(m,true);

 }


void CSystemObject::newMsg(CMsg &msg){
  std::string callback=msg.getParameter("CALLBACK");
  std::string callbackoff=msg.getParameter("CALLBACKOFF");

  if(callback.size()) Callback[callback]="1";
  if(callbackoff.size()) Callback[callbackoff]="0";
 
  std::string act=msg.getACT();
  writeconsole("System received message:");
  writeconsoleln(Name());

if (act == "SETMODE") { newMode(msg); return;} //Updates a parameter in the Subsystem

if (act == "OUTPUT") {Output(msg); return;}
if (act == "PLAY") {play();return;}
if (act == "ON") {on();return;}
if (act == "RUN") {run();return;}
if (act == "READY") {ready();return;}
if (act == "STATS") {stats();return;}


if ((Name()!="RADIO")&&(Name()!="RADIO2")){
  if (act == "IDLE") {idle();return;}
  if (act == "PAUSE") {pause();return;}
  if (act == "STOP") {stop();return;}
  if (act == "OFF") {off();return;}
}

          
if(act=="STATE") {State(msg);return;}
if(act=="UPDATE") {Update(msg);return;}
if(act=="STATUSUPDATE") {statusUpdate(msg);return;}
if(act=="ADDDATALIST") {addDataList(msg);return;}
if(act=="ADDTRANSMITLIST") {addTransmitList(msg);return;}


callNewFunction(msg);
}


void CSystemObject::Update(CMsg &msg){
  _forever = (bool) msg.getParameter("FOREVER",(int)_forever);
  _maxTime= msg.getParameter("MAXTIME",(unsigned long)_maxTime);
  _minTime= msg.getParameter("MINTIME",(unsigned long)_minTime);
  _INTERVAL= msg.getParameter("INTERVAL",(unsigned long)_INTERVAL);
  _startTime= msg.getParameter("STARTTIME",(unsigned long)_startTime);
}



std::string CSystemObject::outputStatus(long val) {
  CMsg m;
  std::string str;
  if (val==0) val=1;
  if (_procCount%val==0){
    

    m.setParameter("Proc",_sid);    
    m.setParameter("Name",_name);
    m.setParameter("Forever",_forever);
   // writeconsole("; MaxTime: ");
   // writeconsole( _maxTime);
   // writeconsole("; MinTime: ");
   // writeconsole( _minTime);
    
    m.setParameter("Interval",_INTERVAL);
    m.setParameter("Time",_currentTime);
        
    m.setParameter("State",_ostate);
    //writeconsole("; procCount: ");
    //writeconsoleln( _procCount);  
    str=m.serialize();     
    
  }
  return str;     
}
  
//"NONE","INIT","READY","PLAY","PAUSE","STOP","ERROR"
void CSystemObject::nextState() {
    _procCount++;
    _currentTime = getTime();   
    std::string str=outputStatus();
    if(str.size()) writeconsoleln();
  if (_currentTime<_startTime) return;   //Do nothing as it shouldnt be activated yet

//timeSinceStateChange()  // Can use this to progress states
if (_ostate == "ERROR") {  
    writeconsoleln(outputStatus(0));
    return;
  }
  if ((_ostate == "NONE")||(_ostate == "")) {  //None->On     
    none();
    return;
  }

  if (_ostate == "ON") {   //On->Ready (Setup)    
    on();
    return;
  }

  if (_ostate == "READY") { // Ready->Play       
    ready();
    return;
  }

  if (_ostate == "PLAY") { // Play->Complete    
    if(isNextCycle()) play();
    return;
  }


  if (_ostate == "PAUSE") { // Pause    
    pause();   
    isNextCycle();
    return;
  }

  if (_ostate == "COMPLETE") { // Should be removed by other process
    complete();    
    return;
  }

if (_ostate == "OFF") { // Should be removed by other process
    off();    
    return;
  }  

 if (_ostate == "IDLE") { // Do nothing   Like pause
    isNextCycle();
    return;
  }  
   
  
}

bool CSystemObject :: outOfTime() {
  if (!_forever && ((_currentTime - _createdTime) > _maxTime)&&((_currentTime - _createdTime) > _minTime)) {   //Play ->Out of Time
    return true;
  }
  return false;

}

void CSystemObject::timeStamp() { 

  _currentTime = getTime(); 
  _loopCount++; 
}

bool CSystemObject::isNextCycle() {
  timeStamp();
  if (outOfTime()) {
    setState("COMPLETE");
    return false;
  }
  if (_currentTime > _prevTime + _INTERVAL) {
    _prevTime = _currentTime;
    return true;
  }
  return false;
}

bool CSystemObject::isTransmitTime() {
  timeStamp();
  
  if (_currentTime > _transmitPREVTIME + _transmitINTERVAL) {
    _transmitPREVTIME = _currentTime;
    return true;
  }
  return false;
}

bool CSystemObject::isTransmitTimeNoUpdate() {
  timeStamp();
  
  if (_currentTime > _transmitPREVTIME + _transmitINTERVAL) {    
    return true;
  }
  return false;
}


bool CSystemObject::addMessageList(CMsg &m){
  CMessages* pM = getMessages(); 
  while(pM->MessagesList.size()>MAXLISTSIZE){pM->MessagesList.pop_front();}
  pM->MessagesList.push_back(m);
  return true;

}


bool CSystemObject::addTransmitList(CMsg &m,bool override){
  if (isTransmitTime()||override) {  
    CMessages* pM = getMessages();
    while(pM->TransmitList.size()>MAXLISTSIZE){pM->TransmitList.pop_front(); }
    pM->TransmitList.push_back(m);
    return true;
  }
  return false;
}

bool CSystemObject::addMessageList(const char* s) {
    CMsg m(s);
    return(addMessageList(m));
}

bool CSystemObject::addTransmitList(const char* s,bool override) {
    CMsg m(s);
    fillMetaMSG(&m);
    return(addTransmitList(m,override));
}


bool CSystemObject::addTransmitList(std::vector<CMsg> &M ,bool override)
{
    bool ready=false;
    if (isTransmitTime()||override) {
      ready=true;
      CMessages* pM = getMessages();
      pM->displayList(1);                                  //Debugging
      for(int count=0;count<M.size();count++){
        CMsg m=M[count];
        fillMetaMSG(&m);
         while(pM->TransmitList.size()>MAXLISTSIZE){pM->TransmitList.pop_front(); }
        pM->TransmitList.push_back(m);  
      }          
      pM->displayList(1);
     return ready;
    }  
    
}


bool CSystemObject::addDataList(const char* s) {  
  CMsg m(s);
  fillMetaMSG(&m);
  return addDataList(m);
}




bool CSystemObject::addDataList(CMsg &m) {
  CMessages* pM = getMessages();
   while(pM->DataList.size()>MAXLISTSIZE){pM->DataList.pop_front(); }
  pM->DataList.push_back(m);
  
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


void CSystemObject::statusUpdate(CMsg &m){  //NEED TO HANDLE MULTIPLE CALLBACKS HERE
  //writeconsole("----- STATUS UPDATE: ");
  //writeconsoleln(m.serialize());
  for (auto it : m.Parameters  ){
    if((it.first=="CALLBACK")||(it.first=="CALLBACKOFF")){
      if(it.first=="CALLBACK")
        Callback[it.second]= "1";
      else
        Callback[it.second]= "0";
    }
    else{
      _cmsg.Parameters[it.first]= it.second;
    }
  }
}


void CSystemObject::respondCallBack(CMsg &m){
  
  for (auto it :Callback){   
    std::string strsystem;
     strsystem=it.first;
     
    if(it.second=="1"){     //Send if the system wants a callback      
             
      std::string timestr=Name();
      
      timestr+="_TIME";      
      m.setSYS(strsystem);      
      m.setACT("STATUSUPDATE");            
      //m.Parameters["ACT"]="STATUSUPDATE"; 
      //m.Parameters[Name()]=sval;      
      m.setParameter(timestr,getTimeString());
      addMessageList(m);
      //writeconsole("--------------------------------- Respond CallBack -----------------------------------");    
      //writeconsoleln(m.serialize());
    }       
  }  
}