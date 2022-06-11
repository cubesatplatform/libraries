#pragma once

#include <systemobject.h>


class CSatPins :public CSystemObject {
	
public:
	CSatPins():CSystemObject() {Name("PINS");init(); }
	~CSatPins() {}
  	void init(){CSystemObject::init();  setForever();setInterval(50000);}
	void read(CMsg &msg);
	void readState(CMsg &msg);
	void high(CMsg &msg);
	void low(CMsg &msg);
	void pwm(CMsg &msg);
	void callCustomFunctions(CMsg &msg) override;
	int digitalReadOutputPin(PinName pin);
	void loop();

 };
