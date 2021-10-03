#pragma once
#include <vector>
#include <list>
#include <map>
#include "msg.h"

#include "funcs.h"
#include "consoleio.h"
#include <algorithm>
#include "systemobject.h"

class CStateObj {
  
	bool _forever = false;
	unsigned long _enter_time;
	unsigned long _exit_time;
	unsigned long _starttime = getTime();
	unsigned long _starttimeoffset = 0;
	unsigned long _createdTime = getTime();
	unsigned long _timestamp = getTime();
	unsigned long _currentTime = getTime();
	unsigned long _maxTime = 500000;
	unsigned long _minTime = 0;
  	unsigned long _lcount=0;
	unsigned long _lastcleanuptime=0;
	
protected:
	void processMsg(CMsg &msg);
  	std::string _name;
	CMsg _statemsg;

public:
	std::list<CSystemObject  *> subsystems;
	std::map<std::string, bool> availablesystems;
	CStateObj() { _enter_time = 0;  _exit_time; }
	virtual ~CStateObj() {}

	virtual void init() {};
	virtual void de_init() {};
	void loop();
	virtual void setup() {};
	void newMsg(CMsg &msg) {filterMsg(msg);	};
	void filterMsg(CMsg &msg);	

  	void addSystem(CMsg &msg);
	virtual void stateMsg(CMsg &msg){_statemsg=msg;};
	virtual void enter();
	virtual void exit();
	CMsg stats();
  	std::string Name(){return _name;}
	void Cleanup();  
	bool outOfTime();
	CSystemObject* FindName(std::string str);
	CSystemObject* FindCID(std::string str);
};
