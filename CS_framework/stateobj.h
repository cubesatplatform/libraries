#pragma once

#include <list>
#include <map>

#include "systemobject.h"



class CStateObj:public CSystemObject {  	
	
	long _statecount=0;

public:
	std::list<CSystemObject  *> subsystems;
	std::map<std::string, bool> onEnter;
	std::map<std::string, bool> onExit;



	CStateObj() { init(); }
	virtual ~CStateObj() {}

	virtual void init();
	virtual void de_init() {};
	
	long stateCount(){return _statecount;};

	virtual void enter();
	virtual void exit();
	virtual void setup();
	virtual void loop();

	void output(CMsg &msg);

	void resetStateSubSystems();

	CSystemObject* FindNameInSubsystems(std::string str);
	CSystemObject* FindCIDInSubsystems(std::string str);

	void addSystem(CMsg &msg);
	void addSystem(CSystemObject* psys);
	void deleteSystem(CMsg &msg);

	void cleanup();  	
	void callCustomFunctions(CMsg &msg) override;	
};

