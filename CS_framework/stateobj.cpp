#include "stateobj.h"
//#include "powerup.h"

#define ECHOINTERVAL 2000

void CStateObj::start(){
	if(getTime()>startTime()){
		setState("PLAY");	
	}  
}




void CStateObj::loop() {	
	_currentTime = getTime();
	if (State()!="PLAY"){		
		start();
	}

	for (auto  psys:subsystems) {
		if(_currentTime>_lastDebug+ECHOINTERVAL){
			if(Name()!="SAT") { writeconsole(psys->Name()); writeconsole("  Errors:"); writeconsole(psys->getRetryCount()); writeconsole("  State:"); writeconsole(psys->State()); writeconsole("  Forever:"); writeconsole(psys->getForever());
			
			writeconsole("  Start:"); writeconsole(psys->startTime());writeconsole("  Time:"); writeconsoleln(getTime());
			}
		}		
		
		if((psys->getRetryCount()>=5)||(psys->State()=="STOP")){
			subsystems.remove(psys);
			psys->clearRetryCount();
			
			CMsg m;
			m.setSYS(psys->Name());
			m.setEVENT("REMOVE");
			m.setTABLE("STATUS");
			m.setParameter("RETRYCOUNT",psys->getRetryCount());
			m.setParameter("STATE",psys->State());
			m.setCOMMENT("REMOOOOOOOOOOOOOOOOOOOOOOOOOVVVVVVVVVVVVVVVVVIIIIIIIIIING : "); 
			
			psys->setState(""); //Reset state for next time
			writeconsoleln(m.serializeout());
			addTransmitList(m);
			return;
		}
		if(psys->isNextCycle())      	
			psys->Run();				
	}
	if(_currentTime>_lastDebug+2000){
		_lastDebug=_currentTime;
	}	
}


void CStateObj::resetSubSystems(){
	
	for (auto it = subsystems.begin(); it != subsystems.end(); it++) {
	CSystemObject* psys;
	psys = *it;	
	psys->setState("");	
	}

}

void CStateObj::cleanup(){
	_lastcleanuptime=getTime();
	for (auto it = subsystems.begin(); it != subsystems.end(); it++) {
	CSystemObject* psys;
	psys = *it;	

	if (psys->isComplete()) {
		
		CSystemObject *pcoresys=getSystem(psys->Name().c_str(),"void CStateObj::Cleanup()");
		if (pcoresys==NULL){							
			CMsg m;
			m.setSYS(psys->Name());
			m.setEVENT("REMOVE");
			m.setTABLE("STATUS");
			m.setINFO("CStateObj::Cleanup()");
			m.setCOMMENT("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  Deleting from State Systems  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			writeconsoleln(m.serializeout());
			
			psys->setState(""); //Reset state for next time
			addTransmitList(m);

			if(!psys->getForever()) delete psys;	
			it = subsystems.erase(it);   //Check to make sure this works			 
			break;						//Ends  on a delete   Will continue on next cycle. Faster than doing all cleanup here
			}
		else{	
			
			}	
			
		}
		
	}

}


CSystemObject* CStateObj::FindNameInSubsystems(std::string str) {
	if (str.size() == 0)  return nullptr;

	for (auto  psys:subsystems) {
		if (psys->Name() == str) {
			return psys;
		}
	}
	return nullptr;
}



CSystemObject* CStateObj::FindCIDInSubsystems(std::string str) {
	if (str.size() == 0)  return nullptr;

	for (auto  psys:subsystems) {
		if (psys->getCID() == str) {
			return psys;
		}
	}
	return nullptr;
}


void CStateObj::callCustomFunctions(CMsg &msg){
	std::string sys = msg.getSYS();
	std::string act = msg.getACT();
	
	if ((Name()!="CORE") && (act == "ADDSYSTEM")) { addSystem(msg) ; return;}
	if (act == "ENTER")  enter();
	if (act == "EXIT")  exit();
	if (act == "RESETSUBSYSTEMS")  resetSubSystems();
	if (act == "CLEANUP")  cleanup();
	if (act == "INIT")  init();
};


void CStateObj::addSystem(CSystemObject* psys){
    subsystems.push_back(psys);      
	CMsg m;
	m.setSYS(Name()+">"+psys->Name());
	m.setEVENT("ADD");
	m.setTABLE("STATUS");
	m.setINFO("addSystem(CSystemObject* psys)   Success  push back");
	writeconsoleln(m.serializeout()); 

 }

void CStateObj::addSystem(CMsg &msg){
  writeconsoleln("------------------------- AddSYstem -------------------");
  std::string sys = msg.getVALUE();  
  CSystemObject* psys=nullptr;
  psys=FindNameInSubsystems(sys);
  if(psys!=nullptr){
    CMsg m;
	m.setSYS(Name()+">"+psys->Name());
	m.setEVENT("ADD");
	m.setTABLE("STATUS");
	m.setINFO("addSystem(CSystemObject* psys)   Already exists");
	writeconsoleln(m.serializeout());

	
     psys->start();
     return;
  }

  if(subsystems.size()>100) {
		CMsg m;
		m.setSYS(Name()+">"+psys->Name());
		m.setEVENT("ADD");
		m.setTABLE("STATUS");
		m.setINFO("addSystem(CSystemObject* psys)   Error  Too many objects");
		writeconsoleln(m.serializeout());
		
      return; //Prevents from too many things running and blowing up system memory
    }
  
  psys=getSystem(sys.c_str(),"CStateObj::addSystem(CMsg &msg)");

  if (psys!=NULL) 	addSystem(psys); 
}


bool CStateObj :: outOfTime() {
	if(getForever())  return false;
	_currentTime=getTime();
  	if ( ((_currentTime - _startTime) > _maxTime)&&((_currentTime - _startTime) > _minTime)) {   //Play ->Out of Time
    	return true;
  	}
  return false;
}


 CMsg CStateObj::stats(){   //XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXXXXXXXXXX  Doesnt work yet!!!  First messages not sent  
  CMsg m;
  std::string str;
  for (auto  psys:subsystems) {		
		str+=psys->Name();
		str+="_";
		str+=psys->State();
		str+=";";
	}


  m.setParameter(Name()+"_SYS",str);  
  m.setParameter(Name()+"_crdT",_createdTime);
  m.setParameter(Name()+"_curT",_currentTime);
  m.setParameter(Name()+"_maxT", _maxTime);
  m.setParameter(Name()+"_minT",_minTime);
  return m;

 }

 void CStateObj::Output(CMsg &msg){
  std::string log, logfinal;
for (auto  psys:subsystems) {		
    log+="SYS:"+psys->Name();
    log+='\n';
  }

  for (auto c:log){
    if(c=='~')c='\n';
    if(c==':')c='|';
    logfinal+=c;

  }
  CMsg cM;
  cM.setDATA(logfinal);
  cM.setTABLE(Name());
  addTransmitList(cM);  
}

  void CStateObj::init(){ 
	
	_statecount=0;
	_enter_time=0;
	_exit_time=0;
	_starttimeoffset = 0;	
	_lastcleanuptime=0;

	setMaxTime(3*TIMEORBIT);
		
  	
	_statemsg.Parameters.clear();
  }

 
void CStateObj::enter(){
	_statecount++;
	_startTime = getTime();	

	if(onEnter["DISABLEMBLOGIC"]) disableMBLogic();
	if(onEnter["DISABLE65V"]) disable65V();
	if(onEnter["DISABLEBURNWIRE"]) disableBurnWire();
	if(onEnter["DISABLESENSORS"]) disableSensors();
	if(onEnter["DISABLEPHONE"]) disablePhone();

	if(onEnter["ENABLEMBLOGIC"]) enableMBLogic();
	if(onEnter["ENABLE65V"]) enable65V();
	if(onEnter["ENABLEBURNWIRE"]) enableBurnWire();
	if(onEnter["ENABLESENSORS"]) enableSensors();
	if(onEnter["ENABLEPHONE"]) enablePhone();

	setState("PLAY");
	CMsg m;
    m.setTABLE("LOG");
    m.setParameter("ENTERSTATE",Name());	
    addTransmitList(m);
    writeconsoleln(m.serializeout()) ;
}
void CStateObj::exit(){
	_stopTime = getTime();

	if(onExit["DISABLEMBLOGIC"]) disableMBLogic();
	if(onExit["DISABLE65V"]) disable65V();
	if(onExit["DISABLEBURNWIRE"]) disableBurnWire();
	if(onExit["DISABLESENSORS"]) disableSensors();
	if(onExit["DISABLEPHONE"]) disablePhone();

	if(onExit["ENABLEMBLOGIC"]) enableMBLogic();
	if(onExit["ENABLE65V"]) enable65V();
	if(onExit["ENABLEBURNWIRE"]) enableBurnWire();
	if(onExit["ENABLESENSORS"]) enableSensors();
	if(onExit["ENABLEPHONE"]) enablePhone();

	setState("PAUSE");
	CMsg m;
    m.setTABLE("LOG");
	m.setParameter("EXITSTATE",Name());    
    addTransmitList(m);
    writeconsoleln(m.serializeout()) ;
}
 	