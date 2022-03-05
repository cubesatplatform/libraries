#pragma once
#include "defs.h"
#include "basedrive.h"
#include <Adafruit_DRV8830.h>


#include <Wire.h>
  
class CMDrive:public CBaseDrive{
  char _address=0x60;

  TwoWire *pWire=&Wire;

  
public:
  Adafruit_DRV8830 myMotorDriver; //This creates the main object of one motor driver and connected slaves.
  
  CMDrive();
  ~CMDrive();  
  void config(char addr=0x60, TwoWire *twowire=&Wire,int m=0);
  void Address(char c);
  void init();
  void setup(){init();}
  void activateDrive(int val); 
};
