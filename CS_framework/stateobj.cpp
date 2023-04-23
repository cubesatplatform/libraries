
#include "stateobj.h"

#define ECHOINTERVAL 5000


void CStateObj::init(){ 
	
	_statecount=0;
	_obj._enter_time=0;
	_obj._exit_time=0;
	_obj._starttimeoffset = 0;	
	_obj._lastcleanuptime=0;
	setInterval(0);
	setState(_PLAY);

	setMaxTime(3*CYCLETIME);
		
	_m.Parameters.clear();
  }


void CStateObj::enter(){
	writeconsole("CStateObj::enter...");  writeconsoleln(name());
	init();
	
	_statecount++;
	_obj._startTime = getTime();	


	
	if(onEnter[_PINADCSI2C]){ disablePin(_PINADCSI2C); }
	if(onEnter[_PIN65V]){  disablePin(_PIN65V); }
	if(onEnter[_PINBURNWIRE]){  disablePin(_PINBURNWIRE); }
	if(onEnter[_PINOBCI2C]) {  disablePin(_PINOBCI2C); }
	if(onEnter[_PINPHONE]) {  disablePin(_PINPHONE); }

	
	if(onEnter[_PINADCSI2C]) { enablePin(_PINADCSI2C); }
	if(onEnter[_PIN65V]) { enablePin(_PIN65V); }
	if(onEnter[_PINBURNWIRE]) {enablePin(_PINBURNWIRE); }
	if(onEnter[_PINOBCI2C]) {enablePin(_PINOBCI2C); }
	if(onEnter[_PINPHONE]) {enablePin(_PINPHONE); }

	
	CMsg m;    
    m.set(_ENTERSTATE,name());	

	for(auto x:subsystems){
		x->enter();
	}    

	writeconsole("End         CStateObj::enter..."); writeconsoleln(name());
}


void CStateObj::exit(){
	writeconsole("CStateObj::exit...");  writeconsoleln(name());

	for(auto x:subsystems){
		x->exit();
	}    
	

	_obj._stopTime = getTime();


	if(onExit[_PINADCSI2C]){ disablePin(_PINADCSI2C); }
	if(onExit[_PIN65V]){  disablePin(_PIN65V); }
	if(onExit[_PINBURNWIRE]){  disablePin(_PINBURNWIRE); }
	if(onExit[_PINOBCI2C]) {  disablePin(_PINOBCI2C); }
	if(onExit[_PINPHONE]) {  disablePin(_PINPHONE); }

	
	if(onExit[_PINADCSI2C]) { enablePin(_PINADCSI2C); }
	if(onExit[_PIN65V]) { enablePin(_PIN65V); }
	if(onExit[_PINBURNWIRE]) {enablePin(_PINBURNWIRE); }
	if(onExit[_PINOBCI2C]) {enablePin(_PINOBCI2C); }
	if(onExit[_PINPHONE]) {enablePin(_PINPHONE); }

	

	setState(_PAUSE);
	CMsg m;    
	m.set(_EXITSTATE,name());    

	writeconsole("End         CStateObj::exit...");  writeconsoleln(name());
}



CSystemObject* CStateObj::FindNameInSubsystems(std::string str) {
	if (str.size() == 0)  return nullptr;

	for (auto  psys:subsystems) {
		if (psys->name() == str) {
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



void CStateObj::setup() {
	setState(_PLAY);
}	


void CStateObj::loop() {		
	
	//echoOff();
	bool bflag=false;
	timeStamp();

	
	if (state()!=_PLAY){		
		start();
	}	



	for (auto  psys:subsystems) {
		if(psys==NULL){
			writeconsoleln("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  NULL LOOP  XXXXXXXXXXXXXXXXXXX");
			continue;
		}


		
		
		if(_obj._currentTime>_obj._lastDebug+ECHOINTERVAL){			
			unsigned int n=(unsigned int)psys;
			writeconsole("Loop "); writeconsole(name());  writeconsole("     ");writeconsole((long) n);writeconsole("     ");writeconsole(psys->name());
			writeconsole("  State:"); writeconsole(psys->state()); 	writeconsole("  Mode:"); writeconsoleln(psys->Mode());
			bflag=true;	
		}		


		
		/*
		if((psys->getRetryCount()>=5)||(psys->state()==_STOP)){
			subsystems.remove(psys);
			psys->clearRetryCount();
			
			CMsg m;
			m.set(_NAME,psys->name());
			m.set(_EVENT,_REMOVE);			
			m.set(_RETRYCOUNT,psys->getRetryCount());
			m.set(_STATE,psys->state());
			m.set(_COMMENT,"REMOVIING : "); 
			
			psys->setState(_BLANK); //Reset state for next time						
			return;
		}
		*/

		if(psys->isNextCycle())      	
			psys->run();						
	}

	
	if(bflag){
		_obj._lastDebug=_obj._currentTime;		
		bflag=false;
		writeconsoleln(IAM());
	}
	
	//echoOn();
}


void CStateObj::resetStateSubSystems(){
	
	for (auto it = subsystems.begin(); it != subsystems.end(); it++) {
	CSystemObject* psys;
	psys = *it;	
	psys->setState(_BLANK);	
	}

}




 void CStateObj::output(CMsg &msg){
  	std::string log, logfinal;
	for (auto  psys:subsystems) {		
		log+="SYS:"+psys->name();
		log+='\n';
		}

	for (auto c:log){
		if(c=='~')c='\n';
		if(c==':')c='|';
		logfinal+=c;
		}
		
	CMsg cM;
	cM.set(_DATA,logfinal);  
	cM.set(_NAME,name());
	addTransmitList(cM);  
  	CSystemObject::output(msg);
}


void CStateObj::addSystem(CSystemObject* psys){
	if (psys == nullptr) return;
    subsystems.push_back(psys);      
	CMsg m;
	m.set(_SYS,name()+">"+psys->name());
	m.set(_EVENT,"ADD");	
	m.set(_INFO,"addSystem(CSystemObject* psys)   Success  push back");
	m.writetoconsole(); 

 }


void CStateObj::addSystem(CMsg &msg){
  writeconsoleln("AddSystem");
  std::string sys = msg.get(_VALUE);  
  CSystemObject* psys=nullptr;
  psys=FindNameInSubsystems(sys);
  if(psys!=nullptr){
    CMsg m;
	m.set(_SYS,name()+">"+psys->name());
	m.set(_EVENT,"ADD");	
	m.set(_INFO,"addSystem Already exists");
	writeconsoleln(m.serializeout());	
    psys->start();
    return;
  }

  if(subsystems.size()>100) {
	CMsg m;
	m.set(_SYS,name()+">"+psys->name());
	m.set(_EVENT,"ADD");		
	m.set(_INFO,"addSystem Error  Too many objects");
	writeconsoleln(m.serializeout());
	
    return; //Prevents from too many things running and blowing up system memory
    }
  
  psys=getSystem(sys.c_str(),"CStateObj::addSystem(CMsg &msg)");

  if (psys!=NULL) 	addSystem(psys); 
}

 

 void CStateObj::deleteSystem(CMsg &msg){
  writeconsoleln("deleteSystem    Sets state to STOP, remove ");
  std::string sys = msg.get(_VALUE);  
  CSystemObject* psys=nullptr;
  psys=FindNameInSubsystems(sys);
  if(psys!=nullptr){
	psys->stop();
  }
 }






void CStateObj::cleanup(){
	_obj._lastcleanuptime=getTime();
	for (auto it = subsystems.begin(); it != subsystems.end(); it++) {
	CSystemObject* psys;
	psys = *it;	

	if (psys->isComplete()) {
		
		CSystemObject *pcoresys=getSystem(psys->name().c_str(),"void CStateObj::Cleanup()");
		if (pcoresys==NULL){							
			CMsg m;
			m.set(_NAME,psys->name());
			m.set(_EVENT,_REMOVE);			
			m.set(_INFO,"CStateObj::Cleanup()");
			m.set(_COMMENT,"Deleting from State Systems");
			writeconsoleln(m.serializeout());
			
			psys->setState(_BLANK); //Reset state for next time
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


void CStateObj::callCustomFunctions(CMsg &msg){
	writeconsoleln("CStateObj::callCustomFunctions(CMsg &msg)  ");
	std::string act=msg.get(_ACT);
	
	
	mapcustom(enter)
	mapcustom(exit)
	mapcustom(resetStateSubSystems)
	mapcustom(cleanup)
	mapcustom(init)
	mapcustommsg(addSystem)
	mapcustommsg(deleteSystem)
	
	
	CSystemObject::callCustomFunctions(msg);
};

