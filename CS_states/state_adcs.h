#pragma once

#include <stateobj.h>


class CADCSState : public CStateObj {
public:
	CADCSState();
	~CADCSState();

	void setup();
  void stateMsg(CMsg &msg);
  void enter();
  void exit();
	
};
