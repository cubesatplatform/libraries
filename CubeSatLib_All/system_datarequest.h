#pragma once
#include <systemobject.h>

//
//This moves a certain type of data to TransmitList
//
class CDataRequest :public CSystemObject {
	
public:
	CDataRequest():CSystemObject() {Name("DATAREQUEST");init(); }
	~CDataRequest() {}
  	void init(){CSystemObject::init();  setForever();setInterval(5000);}
	void loop();
	void moveData();
 
};
