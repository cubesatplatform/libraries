#pragma once

#include <systemobject.h>

#define DELAYTIME 20000

class CDelay :public CSystemObject {
	
public:
	CDelay():CSystemObject() {Name("DELAY");init(); }
	~CDelay() {}
  	void init(){CSystemObject::init();  setForever();setInterval(DELAYTIME+10000);}	
	
	void loop(){delay(DELAYTIME);}

	
 };
