#pragma once

#include "basedrive.h"
#include <Adafruit_DRV8830.h>


#include <Wire.h>
  
class CMDrive:public CBaseDrive{
  char _address=0x60;
  TwoWire *_pWire=&Wire;
  bool _active=false;
  double _Setpoint=0.0, _Input=0.0, _Output=0.0;
  
public:
  Adafruit_DRV8830 myMotorDriver; //This creates the main object of one motor driver and connected slaves.
  
  CMDrive();
  ~CMDrive();  
  void setup(){init();}
  void loop();

  void detumble();
  
  void config(char addr=0x60, TwoWire *twowire=&Wire,int m=0);
  void config(CMsg &msg);

  void address(char c);
  void runOnce(CMsg &m);
  void init();
  void activate();
  void deactivate();
  void test(CMsg &msg);
  void sendPlotter();  
    
  void speed(float val); 
  void callCustomFunctions(CMsg &msg);
};
