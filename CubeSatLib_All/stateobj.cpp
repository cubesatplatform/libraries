#include "stateobj.h"


void CStateObj::loop() {	
	_currentTime = getTime();
	for (auto  psys:subsystems) {
	//  if(Name()!="SAT") { writeconsole(psys->Name()); writeconsole("");  writeconsoleln(psys->State());}
	//	if(Name()!="SAT") { writeconsole(psys->Name()); writeconsole("  "); writeconsole(psys->getRetryCount()); writeconsole("  "); writeconsoleln(psys->State());}
		//writeconsoleln("");	writeconsole("State :");	writeconsole(Name());writeconsoleln("---------------------------------------------------------------------------------------------------------------------------------");
		if(psys->isNextCycle())      	
			psys->nextState();
		else {
			if((psys->getRetryCount()>30)||(psys->State()=="STOP")){
				subsystems.remove(psys);
				psys->clearRetryCount();
				
			 	writeconsole("REMOOOOOOOOOOOOOOOOOOOOOOOOOVVVVVVVVVVVVVVVVVIIIIIIIIIING : "); 
			 	writeconsoleln(psys->Name());

				CMsg m;
				m.setSYS(Name());
				m.setParameter("REMOVING",true);
				m.setParameter("RETRYCOUNT",psys->getRetryCount());
				m.setParameter("STATE",psys->State());
				psys->setState(""); //Reset state for next time
				addTransmitList(m);
				return;
			}
		}
		//writeconsoleln("");	writeconsole("End State :");writeconsole(Name());	writeconsoleln("---------------------------------------------------------------------------------------------------------------------------------");
	}
	//if(getTime()>lastcleanuptime+10000)		Cleanup();
	
}

void CStateObj::enter(){
	_statecount++;
	_starttime = getTime();

}
void CStateObj::exit(){
	_endtime = getTime();
}

void CStateObj::Cleanup(){
	writeconsoleln("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Cleanup  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	_lastcleanuptime=getTime();
	for (auto it = subsystems.begin(); it != subsystems.end(); it++) {
	CSystemObject* psys;
	psys = *it;	

	if (psys->isComplete()) {
		
		CSystemObject *pcoresys=getSystem(psys->Name().c_str(),"void CStateObj::Cleanup()");
		if (pcoresys==NULL){							
				writeconsole(psys->Name());
				writeconsoleln("Deleted");

				writeconsoleln("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  Deleting from State Systems  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				if(!psys->Forever()) delete psys;	
				it = subsystems.erase(it);   //Check to make sure this works			 
				break;						//Ends loop on a delete   Will continue on next cycle. Faster than doing all cleanup here
				}
		else{	
			//writeconsoleln("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  Not deleting memory of Heap Systems  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");		
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
		if (psys->CID() == str) {
			return psys;
		}
	}
	return nullptr;
}

void CStateObj::filterMsg(CMsg &msg){		
	std::string str = msg.getSYS();
	//if (availablesystems[str])	processMsg(msg); 
    //else writeconsoleln("FILTERED");
    processMsg(msg);   
	}




void CStateObj::processMsg(CMsg &msg) { 
	std::string sys = msg.getSYS();
	std::string act = msg.getACT();
	std::string CMDID = msg.getCMDID();
	std::string ID = msg.getID();

	if ((Name()!="CORE") && (act == "START")) {	
    	addSystem(msg) ;
		return;
	}
	/*

	CSystemObject* psys = nullptr;

	writeconsole("State ProcessMsg: ");
	writeconsoleln(msg.serialize());

	if (psys == nullptr) psys = FindNameInSubsystems(sys);
	if (psys == nullptr) psys = FindCIDInSubsystems(CMDID);
	if ((psys == nullptr) && (Name()=="CORE"))psys = getSystem(sys.c_str(),"void CStateObj::processMsg(CMsg &msg) { ");
	if (psys == nullptr) return;
	
	psys->newMsg(msg);   //Send the message to the SubSytem (pointed to from the find operation)
	if (act != "START")		msg.setParameter("PROCESSED","1");
	*/
};




void CStateObj::addSystem(CSystemObject* psys){
      subsystems.push_back(psys);       
 }

void CStateObj::addSystem(CMsg &msg){
  writeconsole("Adding System: ");
  std::string sys = msg.getSYS();
  std::string act = msg.getACT();
  std::string CMDID = msg.getCMDID();
  std::string ID = msg.getID();
  CSystemObject* psys=nullptr;
  psys=FindNameInSubsystems(sys);
  if(psys!=nullptr){
     writeconsole("Error: Object ALREADY EXISTS    Turning existong one ON()");
     psys->start();
     return;
  }

  writeconsoleln(sys);  writeconsoleln("");
  if(subsystems.size()>100) {
      writeconsole("Error: Too Many Objects    Not creating new one.");
      return; //Prevents from too many things running and blowing up system memory
    }
  

  writeconsoleln("void CStateObj::addSystem(CMsg &msg)");
  psys=getSystem(sys.c_str(),"CStateObj::addSystem(CMsg &msg)");
  writeconsoleln("---------------------------END void CStateObj::addSystem(CMsg &msg)");

  if (psys!=NULL) subsystems.push_back(psys); 
}



bool CStateObj :: outOfTime() {
	if(_forever)  return false;
	_currentTime=getTime();
  	if ( ((_currentTime - _createdTime) > _maxTime)&&((_currentTime - _createdTime) > _minTime)) {   //Play ->Out of Time
    	return true;
  	}
  return false;
}



 void CStateObj::stats(){   //XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXXXXXXXXXX  Doesnt work yet!!!  First messages not sent  
  CMsg m;
  for (auto  psys:subsystems) {		
		psys->stats(m);
	}



  m.setParameter("table","stats");  
  m.setParameter("name",Name());
  m.setParameter("createdT",_createdTime);
  m.setParameter("currentT",_currentTime);

  m.setParameter("maxT", _maxTime);
  m.setParameter("minT",_minTime);
  addTransmitList(m);

 }

  void CStateObj::init(){ 
	
	_statecount=0;
	_enter_time=0;
	_exit_time=0;
	_starttime = getTime();
	_endtime;
	_starttimeoffset = 0;
	_createdTime = getTime();
	_timestamp = getTime();
	_currentTime = getTime();
	_maxTime = 500000;
	_minTime = 0;
  	_lcount=0;
	_lastcleanuptime=0;
	
  	
	_statemsg.Parameters.clear();
  }

 