#pragma once

#include <vector>
#include <list>
#include <map>
#include "msg.h"

#include "funcs.h"
#include "consoleio.h"
#include <algorithm>
#include "systemobject.h"
#include "state_defs.h"
#include <powerup.h>

class CStateObj:public CSystemObject {  	
	
	long _statecount=0;
	unsigned long _enter_time;
	unsigned long _exit_time;	
	unsigned long _starttimeoffset = 0;
	unsigned long _lastcleanuptime=0;


  	
protected:	  	
	CMsg _statemsg;

public:
	std::list<CSystemObject  *> subsystems;
	std::map<std::string, bool> availablesystems;
	std::map<std::string, bool> onEnter;
	std::map<std::string, bool> onExit;
	CStateObj() { init(); }
	virtual ~CStateObj() {}

	virtual void init();
	virtual void de_init() {};
	void loop();

	
  	void addSystem(CMsg &msg);
	void addSystem(CSystemObject* psys);
	virtual void stateMsg(CMsg &msg){_statemsg=msg;};
	virtual void enter();
	virtual void exit();
	long stateCount(){return _statecount;};
	
	CMsg stats();
	virtual void start();
	void Cleanup();  
	bool outOfTime();
	void Output(CMsg &msg);
	void resetSubSystems();
	void callCustomFunctions(CMsg &msg);
	CSystemObject* FindNameInSubsystems(std::string str);
	CSystemObject* FindCIDInSubsystems(std::string str);
};
