#include "stateobj.h"
//#include "powerup.h"

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
		if(_currentTime>_lastDebug+2000){
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
			psys->nextState();				
	}
	if(_currentTime>_lastDebug+2000){
		_lastDebug=_currentTime;
	}	
}

void CStateObj::enter(){
	_statecount++;
	_startTime = getTime();	
	setState("PLAY");
}
void CStateObj::exit(){
	_stopTime = getTime();
	setState("PAUSE");
}


void CStateObj::resetSubSystems(){
	
	for (auto it = subsystems.begin(); it != subsystems.end(); it++) {
	CSystemObject* psys;
	psys = *it;	
	psys->setState("");	
	}

}

void CStateObj::Cleanup(){
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
			break;						//Ends loop on a delete   Will continue on next cycle. Faster than doing all cleanup here
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

void CStateObj::newMsg(CMsg &msg) {
	filterMsg(msg);	
	};

void CStateObj::filterMsg(CMsg &msg){		
	std::string str = msg.getSYS();
	if(str==Name())			//Only want messages for this State
    	processMsg(msg);   
	}


void CStateObj::processMsg(CMsg &msg) { 
	std::string sys = msg.getSYS();
	std::string act = msg.getACT();

	//writeconsoleln(Name()+"> A    CStateObj::processMsg(CMsg &msg) ");
	if ((Name()!="CORE") && (act == "START")) {
		//writeconsoleln(Name()+">  B   CStateObj::processMsg(CMsg &msg) ");	
    	addSystem(msg) ;		
		return;
	}
	CSystemObject::newMsg(msg);
	//writeconsoleln(Name()+"> C   CStateObj::processMsg(CMsg &msg) ");	
};


void CStateObj::addSystem(CSystemObject* psys){
      subsystems.push_back(psys);       
 }

void CStateObj::addSystem(CMsg &msg){
  std::string sys = msg.getSYS();
  std::string act = msg.getACT();
  std::string CID = msg.getCID();
  std::string ID = msg.getID();
  CSystemObject* psys=nullptr;
  psys=FindNameInSubsystems(sys);
  if(psys!=nullptr){
    CMsg m;
	m.setSYS(Name()+">"+psys->Name());
	m.setEVENT("ADD");
	m.setTABLE("STATUS");
	m.setINFO("addSystem(CSystemObject* psys)   Already exists");
	writeconsoleln(m.serializeout());
	//addTransmitList(m);
	
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
		//addTransmitList(m);
      return; //Prevents from too many things running and blowing up system memory
    }
  


  psys=getSystem(sys.c_str(),"CStateObj::addSystem(CMsg &msg)");

  if (psys!=NULL) {
	  subsystems.push_back(psys); 
		CMsg m;
		m.setSYS(Name()+">"+psys->Name());
		m.setEVENT("ADD");
		m.setTABLE("STATUS");
		m.setINFO("addSystem(CSystemObject* psys)   Success  push back");
		writeconsoleln(m.serializeout());
		//addTransmitList(m);
  }
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

	
	
  	
	_statemsg.Parameters.clear();
  }

 