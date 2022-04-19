#pragma once

#include <vector>
#include <list>
#include <map>
#include "msg.h"

#include "funcs.h"
#include "consoleio.h"
#include "powerup.h"
#include <algorithm>
#include "systemobject.h"

class CStateObj:public CSystemObject {  	
	
	long _statecount=0;
	unsigned long _enter_time;
	unsigned long _exit_time;	
	unsigned long _starttimeoffset = 0;
	unsigned long _lastcleanuptime=0;


  	
protected:
	void processMsg(CMsg &msg);
  	
	CMsg _statemsg;

public:
	std::list<CSystemObject  *> subsystems;
	std::map<std::string, bool> availablesystems;
	CStateObj() { _enter_time = 0;  _exit_time=0; }
	virtual ~CStateObj() {}

	virtual void init();
	virtual void de_init() {};
	void loop();

	void newMsg(CMsg &msg); 
	void filterMsg(CMsg &msg);	
	
  	void addSystem(CMsg &msg);
	void addSystem(CSystemObject* psys);
	virtual void stateMsg(CMsg &msg){_statemsg=msg;};
	virtual void enter();
	virtual void exit();
	long stateCount(){return _statecount;};
	
	void stats();
	virtual void start();
	void Cleanup();  
	bool outOfTime();
	void Output(CMsg &msg);
	void resetSubSystems();
	CSystemObject* FindNameInSubsystems(std::string str);
	CSystemObject* FindCIDInSubsystems(std::string str);
};
