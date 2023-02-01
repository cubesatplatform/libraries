
#include "systemobject.h"
#include "messages.h"

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  #include "mbed.h"
#endif


int CSystemObject::sid=0;
long CSystemObject::_lastLowPowerMsg=0;
bool CSystemObject::_mirrorCloud=false;

CMessages CSystemObject::MMM;

std::map<std::string,CSystemObject *> CSystemObject::SysMap;

std::map<std::string, PinName> CSystemObject::Pins;
std::map<std::string, PinName> CSystemObject::pwmPins;

std::string CSystemObject::_IAM="";  //SYSNAME
std::string CSystemObject::_defaultTO="ADR1";



bool myCompare(std::string a,const char *f){
  String _a=a.c_str();
  String _f=f;
  _a.toUpperCase(); 
  _f.toUpperCase(); 
  if (_a==_f)
    return true;
  return false;
}



CSystemObject::CSystemObject() {
  sid++;
  _sid=sid;
  init();
  }

void CSystemObject::init(){
  success=false;

  _obj._createdTime=getTime();
  _obj._modifiedTime=getTime();
  _obj._currentTime=getTime();
  _obj._prevTime=0;
  
  _obj._maxTime=10*CYCLETIME;
  
  _obj._minTime=0;

  _obj._startTime=_obj._createdTime;
  _obj._stopTime=_obj._startTime+_obj._maxTime;
  

  
  _loopCount=0;
  _procCount=0;


   
  _lastStateTime=0;  
  _obj._lastUse=0;
  _retryCount=0;

  //_interval=0;// Don't clear the Mode on INIT !!!loop time interval in MicroSeconds,  0 means no delay
  //_mode=_BLANK;   // Don't clear the Mode on INIT !!!This determines what you are doing   Set to IDLE when done and nothing left to do  When u set new mode need to get it out of IDLE
}


void CSystemObject::setMsg(CMsg &msg){
  std::string reset=msg.get(_RESET);
  if(reset.size()>0)
    msg.clear();
  _m=msg;
  
  }

CSystemObject *CSystemObject::getSystem(const char *sys,const char *comment){ 
  std::string sysstr=sys;
  std::string tmpstr=sys;
  std::string cstr;
  //if(tmpstr=="SAT") return NULL;
  CSystemObject * psys= SysMap[sysstr]; 
  char c='a';
  CMsg m;
    if (psys==NULL) {
      m.set(_INFO,"Can't find system  (getSystem): ");      
      m.set(_COMMENT,comment);

      for(auto x:SysMap){
        cstr=c;
        m.set(cstr,x.first);
        c++;
      }
      m.set(_SYS,sys);
      for(int count=0;count<10;count++){
        cstr=c;
        m.set(cstr,"BUG");
        c++;
        }
      return NULL;
    }
    return psys;   //Due to polymorphism, will return whatever system you want
}



void CSystemObject::transmitError(const char *tmp){
  writeconsole("Critical Error: ");  writeconsoleln(tmp);
  CMsg msg;  
  msg.set(_ERROR,tmp);    

  addTransmitList(msg); 
} 




void CSystemObject::name(std::string s) {    //Never ever ever run in a construction!!@#!@#!@#  Call later
  
  _name = s;      
  CSystemObject *ptr=this;   
  if(s.size()&&(ptr!=NULL)){
    SysMap[s]=ptr;            
  }  
}
  

 void CSystemObject::fillMetaMSG(CMsg *m){  
  _obj._currentTime = getTime();    
  if((m->get(_SYS)==std::string(_BLANK))&&(m->get(_SYS)!=std::string(_RADIO))&&(m->get(_SYS)!=std::string(_RADIO1)))
    m->set(_SYS,name());
      
  if(m->get(_FROM)==_BLANK) m->set(_FROM,getIAM());    
  if(m->get(_TO)==_BLANK) m->set(_TO,defaultTO());        
  if(_cid.size())m->set(_MCID,_cid);
}

void CSystemObject::newMode(CMsg &msg){
  writeconsoleln("newMode");
  std::string mode=msg.get(_VALUE);
  _m.set(_MODE,mode);  
}

void CSystemObject::stats(CMsg &msg){    
  CMsg m;

  m=_m;
  
  m.set(_NAME,_name);
  m.set(_MCID,_cid);
  m.set(_SID,_sid);
  m.set(_LASTSTATET,_lastStateTime);  
  m.set(_CURRENTT,_obj._currentTime);

  m.set(_PREVT,_obj._prevTime);
  m.set(_MAXT, _obj._maxTime);
  m.set(_MINT,_obj._minTime);
  m.set(_INTERVAL,_interval);
  m.set(_LASTUSE,_obj._lastUse);

  m.set(_STARTTIME,_obj._startTime);

  m.set(_LOOPCOUNT,_loopCount);
  m.set(_PROCCOUNT,_procCount);


  addTransmitList(m);
}

void CSystemObject::mode(CMsg &msg){
  std::string tmp=msg.get(_MODE,_m.get(_MODE));
  //std::string b=_m.get(_MODE);
 _m.set(_MODE, tmp);  
 }

void CSystemObject::newMsg(CMsg &msg){
 
  std::string act=msg.get(_ACT);
  std::string val=msg.get(_VALUE);
  
  writeconsole("Msg receieved by ");writeconsole(name()); writeconsole(": ");writeconsoleln(act);

  mapcustommsg(newMode);
  mapcustom(play);
  mapcustom(start);
  mapcustom(off);
  mapcustom(restart);
  mapcustommsg(stats);
  mapcustom(tic);
  
  
  if ((name()!=_RADIO)&&(name()!=_RADIO2)&&(name()!=_CORE)){ 
    mapcustom(pause);
    mapcustom(stop);
  }

  mapcustom(setup)
  mapcustommsg(run)
  mapcustom(init)
  mapcustom(loop)
  mapcustommsg(test)
  mapcustommsg(state)
  mapcustommsg(update)
  mapcustommsg(config)

  mapcustommsg(goState)

  mapcustom(clearRetryCount)
  
  //mapcustom(goLowPowerState)
  mapcustommsg(sendData)
  mapcustommsg(setMsg)


  
  if(act=="SETSTATE") {std::string val=msg.get(_VALUE);setState(val);return;}
  if(act=="DATAMAP") {msg.remap();std::string key=msg.get(_NAME); addDataMap(key,msg);return;}
  if(act=="ADDTRANSMITLIST") {msg.remap();addTransmitList(msg);return;}
  if(act=="ADDMESSAGELIST") {msg.remap();addMessageList(msg);return;}  
  if(act=="ADDRECEIVEDLIST") {msg.remap();addReceivedList(msg);return;}    
  if(act=="SENDDATACLOUD") {msg.set(_TO,_CLOUD);sendData(msg);return;}  
  if(act=="GOSTATE") {goState(val);return;}  

  if(act==_ENABLEPIN) {enablePin(val);return;}
  if(act==_DISABLEPIN) {disablePin(val);return;}


  callCustomFunctions(msg);
}

void CSystemObject::sendData(CMsg &msg){

  std::string name=msg.get(_NAME);
  CMsg m=getDataMap(name);
  if (msg.get(_TO)==_CLOUD) addReceivedList(msg);
  else addTransmitList(m);    
}

/*
double CSystemObject::getDataValue(const char *name,char _axis){
  std::string sname="IMUI_"+ std::string(name);
  
  std::string saxis;
  saxis+=_axis;
  CMsg msg=getDataMap(sname);

  if(msg.Parameters.size()<1){
    sname="IMUS_"+ std::string(name);
    msg=getDataMap(sname);
  }
  double val=0.0;
  val=msg.get(saxis,0.0);
  return val;
}
*/

void CSystemObject::setDataString(std::string sname,std::string sfield, std::string sval){  
  CMsg msg=getDataMap(sname);
  
  
  msg.get(sfield,sval);
  setDataMap(msg);

}

std::string CSystemObject::getDataString(std::string sname,std::string sfield){  
  CMsg msg=getDataMap(sname);
  
  std::string val;
  val=msg.get(sfield,val);
  return val;
}


double CSystemObject::getDataValue(std::string sname,std::string sfield){  
  CMsg msg=getDataMap(sname);
  
  double val=0.0;
  val=msg.get(sfield,val);
  return val;
}

void CSystemObject::update(CMsg &msg){
  
  _forever = (bool) msg.get(_FOREVER,(int)_forever);
  _obj._maxTime= msg.get(_MAXTIME,_obj._maxTime);
  _obj._minTime= msg.get(_MINTIME,_obj._minTime);
  _interval= msg.get(_INTERVAL,_interval);
  _obj._startTime= msg.get(_STARTTIME,_obj._startTime);
  _obj._stopTime= msg.get(_STOPTIME,_obj._stopTime);
  
  _m.set(_MODE,msg.get(_MODE));
  _m.set(_STATE,msg.get(_STATE,_m.get(_STATE)));
  
}

void CSystemObject::setState(std::string str) { 
  if (!((str==_BLANK)||(str==_START)||(str==_STOP)||(str==_PLAY)||(str==_ERROR)||(str==_PAUSE)||(str==_DONE))){
    for (int count=0;count<10;count++){
      writeconsole("BAD STATE ");      writeconsoleln(str);
      }
    return;
  }
  _m.set(_STATELAST,_m.get(_STATE));
  _m.set(_STATE,str);
  
  _lastStateTime=getTime(); 
  if(str==_ERROR) _retryCount++;  
  if(str==_PLAY) _retryCount=0;
  
};      


std::string CSystemObject::outputStatus(long val) {
  CMsg m;
  std::string str;
  if (val==0) val=1;
  if (_procCount%val==0){
    
    m.set(_PROC,_sid);    
    m.set(_NAME,_name);
    m.set(_FOREVER,_forever);

    m.set(_INTERVAL,_interval);
    m.set(_TIME,_obj._currentTime);
        
    m.set(_STATE,_m.get(_STATE));
    str=m.serializeout();     
    
  }
  return str;     
}


void CSystemObject::reboot(){
  while(1){}
}

void CSystemObject::run(long runtime){
  if(runtime> CYCLETIME*10){
    writeconsoleln("Runtime exceeds maxtime.  Exiting");
    return;
  }
  long endtime=getTime()+runtime;
  
  while(getTime()<endtime){    
    if((_obj._currentTime+_interval)<getTime()){
      #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
        //mbed::Watchdog::get_instance().kick();   
      #endif
      run();        
    }
    
  }
}  

void CSystemObject::run() {
_procCount++;
_obj._currentTime = getTime();   

    
if (_obj._currentTime<_obj._startTime) return;   //Do nothing as it shouldnt be activated yet

if (_m.get(_STATE) == _ERROR) {  //if ERROR, have it retry a few times
    writeconsole(name());writeconsole(_BLANK);writeconsoleln(state());
    if(_interval<250) {
         _interval+=5;
      } 
    else {
        _errorCount++;
        if(_errorCount>_errorThreshold){          
          _m.set(_STATE,_STOP);     
        }
        //writeconsoleln(outputStatus(0));
    }
    writeconsole(name());writeconsole(" ");writeconsoleln(state());        
    return;
  }

  if ((_m.get(_STATE) == _START)||(_m.get(_STATE) == _BLANK)) {  //None->On     
    
    start();
    
    return;
  }
 
  if (_m.get(_STATE) == _PLAY) { // Play->Stop
    
    play();
    
    return;
  }

  if (_m.get(_STATE) == _PAUSE) { // Pause    
    
    return;
  }

  if (_m.get(_STATE) == _STOP) { // Pause    
    
    off();
    
    return;
  }

  if (_m.get(_STATE) == _DONE) { // Do Nothing    
    
    return;
  }
}

bool CSystemObject :: outOfTime() {
  if (_forever)
    return false;
  if (((_obj._currentTime - _obj._startTime) > _obj._maxTime)&&((_obj._currentTime - _obj._startTime) > _obj._minTime)) {   //Play ->Out of Time    
    CMsg m;
    stats(m);
    return true;
  }
  return false;

}

void CSystemObject::timeStamp() { 
  _obj._currentTime = getTime(); 
  _loopCount++; 
}

void CSystemObject::sendError() { 
  CMsg m;
  setState(_ERROR);
  m.set(_ERROR,name());
  m.set(_TIME,getTime());
  m.set(_COMMENT,_ERROR);
  m.set(_STATE,state());
  

  addTransmitList(m);
}

bool CSystemObject::isNextCycle() {
  timeStamp();
  
  if (state()==_STOP)
    return false;    
  
  if (outOfTime()) {
  
    CMsg m;
    m.set(_SYS,name()); 
    m.set(_INFO,"Out of time.  Stopping.");
    m.writetoconsole();
    setState(_STOP);
    return false;
  }
  
  
  if(state()!=_PLAY){  
    if(_retryCount>30)return false;  
    return true;
  }

  if(getTime()<_obj._startTime){
    
    return false;
  }

  

  if (_obj._currentTime >= _obj._prevTime + _interval) {  
    _obj._prevTime = _obj._currentTime;
    return true;
  }
  
  if(_interval==0)return true;
  return false;
}




void CSystemObject::state(CMsg &m) { 
  std::string s=m.get(_STATE);
  if ((_name==_RADIO)||(_name==_RADIO1)){
    writeconsoleln("NO STATE UPDATES TO RADIO");    
  }
  else {
    setState(s); 
  }  
}



  
void CSystemObject::addTransmitList(CMsg &m ){
 m.writetoconsole();
 MMM.addTransmitList(m);
}



void CSystemObject::addDataMap(std::string key,CMsg &m){
  m.writetoconsole();
  MMM.addDataMap(key,m);
}

void CSystemObject::addDataMap(CMsg &m){
  m.writetoconsole();
  std::string key=m.get(_NAME);
  MMM.addDataMap(key, m);  
}

CMsg CSystemObject::getDataMap(std::string key){
  
 
  return MMM.getDataMap(key);
}

void CSystemObject::addMessageList(CMsg &m ){
  m.writetoconsole();
  MMM.addMList(m);
  
}

void CSystemObject::addReceivedList(CMsg &m ){
  m.writetoconsole();
  MMM.addRList(m,getIAM());
  

}

void CSystemObject::goState(CMsg &msg){
  
  msg.set(_SYS,getIAM());
  msg.set(_ACT,_CHANGESTATE);
  //msg.set(_VALUE,val);
  std::string val=msg.get(_VALUE);
  if(val.size())
    addMessageList(msg);
}



void CSystemObject::goState(std::string val){
  CMsg m;
  m.set(_SYS,getIAM());
  m.set(_ACT,_CHANGESTATE);
  m.set(_VALUE,val);
  addMessageList(m);
}


void CSystemObject::transmitResult(std::string act,std::string result){
  CMsg msg;
  msg.set(_NAME,name());
  msg.set(act,result);
  
  addTransmitList(msg);
}

void CSystemObject::transmitResult(std::string act,CMsg msg){
  msg.set(_VALUE,act);
  
  addTransmitList(msg);
}


void CSystemObject::enablePin(std::string str){
  writeconsoleln("void CSystemObject::enablePin(std::string str)");
  PinName pin=getPin(str);
  if(pin){
    writeconsole("Enable Pin  ");writeconsoleln(str);
    digitalWrite(pin, HIGH);  
    delay(100);
  }
}


void CSystemObject::disablePin(std::string str){
  PinName pin=getPin(str);
  if(pin){
    writeconsole("Disable Pin  ");writeconsoleln(str);
    digitalWrite(pin, LOW);  
    delay(100);
  }
}

/*

CSystemObject *getIMU(std::string name){
  CSystemObject * pIMUI=getSystem("IMUI");
  CSystemObject * pIMUS=getSystem("IMUS");
  
  
  if(pIMUI!=NULL) {
    std::string s=pIMUI->inUse();
    if(s==name) return pIMUI;
  
    if(s.length()==0){ 
      pIMUI->setInUse(name);
      return pIMUI;
    }
  }

  if(pIMUS!=NULL) {
    std::string s=pIMUS->inUse();
    if(s==name) return pIMUS;
  
    if(s.length()==0){ 
      pIMUS->setInUse(name);
      return pIMUS;
    }
  }
  
  return NULL;
}
 
*/


