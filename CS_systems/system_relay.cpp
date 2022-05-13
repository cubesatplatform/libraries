#include "system_relay.h"

CRelay::CRelay(){
	Name("RELAY");
	init(); 
	setForever();	
	}

void CRelay::loop() {

}

void CRelay::setMsg(CMsg &msg) {
	 RelayList.push_back(msg);
}

void CRelay::getMsg(CMsg &msg) {
	std::string to=msg.getto();
	CMsg m=RelayList.findwRemove(to,"to");
	if(m.getVALUE().size())
		addTransmitList(m);
}

void CRelay::callCustomFunctions(CMsg &msg){
	std::string act=msg.getACT();

	if((act=="")||(act=="SEND")) setMsg(msg);  
	if((act=="GET")) getMsg(msg);  
}
