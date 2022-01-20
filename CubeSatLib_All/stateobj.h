#pragma once
#include <vector>
#include <list>
#include <map>
#include "msg.h"

#include "funcs.h"
#include "consoleio.h"
#include <algorithm>
#include "systemobject.h"

class CStateObj:public CSystemObject {  	
	
	long _statecount=0;
	unsigned long _enter_time;
	unsigned long _exit_time;	
	unsigned long _starttimeoffset = 0;
	unsigned long _lastcleanuptime=0;

	//bool _forever = false;
	//unsigned long _timestamp = getTime();
	//unsigned long _lcount=0;
	//unsigned long _createdTime = getTime();
	//unsigned long _starttime = getTime();
	//unsigned long _endtime;
	//unsigned long _currentTime = getTime();
	//unsigned long _maxTime = 500000;
	//unsigned long _minTime = 0;
  	
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
	//virtual void setup() {};
	//unsigned long startTime(){return _starttime;}
	void newMsg(CMsg &msg); 
	void filterMsg(CMsg &msg);	
	
  	void addSystem(CMsg &msg);
	void addSystem(CSystemObject* psys);
	virtual void stateMsg(CMsg &msg){_statemsg=msg;};
	virtual void enter();
	virtual void exit();
	long stateCount(){return _statecount;};
	
	void stats();
	void Cleanup();  
	bool outOfTime();
	void Output(CMsg &msg);
	CSystemObject* FindNameInSubsystems(std::string str);
	CSystemObject* FindCIDInSubsystems(std::string str);
};
