#include "system_relay.h"

CRelay::CRelay(){
	Name("RELAY");
	init(); 
	setForever();	
	}

void CRelay::loop() {

}

void CRelay::setMsg(CMsg &msg) {
	_msgcount++;
	std::string name;
	name = RELAYKEY;
	name += "_";
	name += tostring(_msgcount);
	
	msg.setNAME(name);
	addDataMap(msg.getNAME(),msg);
}



void CRelay::callCustomFunctions(CMsg &msg){
	std::string act=msg.getACT();

	if((act=="")||(act=="SEND")) setMsg(msg);  	
}
