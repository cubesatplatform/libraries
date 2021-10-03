#include "stateobj.h"



void CStateObj::loop() {	
	_currentTime = getTime();
	for (auto  psys:subsystems) {
	
		psys->nextState();
	}
	//if(getTime()>lastcleanuptime+10000)		Cleanup();
	
}

void CStateObj::enter(){
	_starttime = getTime();
	for (auto  psys:subsystems) {
	
		psys->on();
	}
}
void CStateObj::exit(){
	
	for (auto  psys:subsystems) {
	writeconsoleln("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx State Exit  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

		psys->off();
	}
}

void CStateObj::Cleanup(){
	writeconsoleln("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Cleanup  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	_lastcleanuptime=getTime();
	for (auto it = subsystems.begin(); it != subsystems.end(); it++) {
		CSystemObject* psys;
		psys = *it;	

		if (psys->isComplete()&&!psys->Forever()) {
			
			CSystemObject *pcoresys=getSystem(psys->Name().c_str());
			if (pcoresys==NULL){							
					writeconsole(psys->Name());
					writeconsoleln("Deleted");

					writeconsoleln("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  Deleting from State Systems  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
					delete psys;	
					it = subsystems.erase(it);   //Check to make sure this works			 
					break;						//Ends loop on a delete   Will continue on next cycle. Faster than doing all cleanup here
					}
			else{	
				//writeconsoleln("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  Not deleting memory of Heap Systems  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");		
				}	
				
			}
			
		}

}


CSystemObject* CStateObj::FindName(std::string str) {
	if (str.size() == 0)  return nullptr;

	for (auto  psys:subsystems) {
		if (psys->Name() == str) {
			return psys;
		}
	}
	return nullptr;
}



CSystemObject* CStateObj::FindCID(std::string str) {
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
	
	

	CSystemObject* psys = nullptr;

	writeconsoleln(msg.serialize());
	writeconsole(_name);
	writeconsole(" CStateObj Msg Recd for: ");
	writeconsole(sys);


	if(psys == nullptr) psys = FindName(sys);
	if (psys == nullptr) psys = FindCID(CMDID);
	if ((psys == nullptr) && (Name()=="CORE"))psys = getSystem(sys.c_str());
	if (psys == nullptr) return;

	
	psys->newMsg(msg);   //Send the message to the SubSytem (pointed to from the find operation)


	
};

bool CStateObj :: outOfTime() {
	if(_forever)  return false;
	_currentTime=getTime();
  	if ( ((_currentTime - _createdTime) > _maxTime)&&((_currentTime - _createdTime) > _minTime)) {   //Play ->Out of Time
    	return true;
  	}
  return false;
}



 CMsg CStateObj::stats(){   //XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXXXXXXXXXX  Doesnt work yet!!!  First messages not sent  
  
  for (auto  psys:subsystems) {		
		psys->stats();
	}
	CMsg m;


  m.setParameter("table","stats");  
  m.setParameter("name",_name);
  m.setParameter("createdT",_createdTime);
  m.setParameter("currentT",_currentTime);

  m.setParameter("maxT", _maxTime);
  m.setParameter("minT",_minTime);
  return m;

 }