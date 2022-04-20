#pragma once

#include <systemobject.h>


class CDelay :public CSystemObject {
	
public:
	CDelay():CSystemObject() {Name("DELAY");init(); }
	~CDelay() {}
  	void init(){CSystemObject::init();  setForever();setInterval(15000);}
	void callCustomFunctions(CMsg &msg){}
	
	void loop(){delay(SLEEPTIME);}

 };
