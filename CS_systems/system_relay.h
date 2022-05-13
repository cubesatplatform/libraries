#pragma once

#include "framework_defs.h"
#include <messages.h>
#include <systemobject.h>

//
//This moves a certain type of data to TransmitList
//
class CRelay :public CSystemObject {
	CMessageList RelayList; 
public:
	CRelay();
	~CRelay() {}
  	void init(){CSystemObject::init();  setForever();setInterval(5000);}
	void loop();
	void setMsg(CMsg &msg);
	void getMsg(CMsg &msg);
	void callCustomFunctions(CMsg &msg);
};
