#pragma once

#include <stateobj.h>


class CADCSState : public CStateObj {
public:
	CADCSState();
	~CADCSState();

	void setup();
  
  void enter();
  void exit();
	
};
