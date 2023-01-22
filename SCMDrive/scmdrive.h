#pragma once

#include <arduino.h>
#include "basedrive.h"
#include "SCMDW.h"
#include "SCMDW_config.h" //Contains #defines for common SCMD register names and values


#include <Wire.h>
  //  It uses .setDrive( motorNum <0,1>, direction<1:forward,0:backward>, level<0...255> ) to drive the motors.

//classSCMDrive:public CSystemObject{
  class SCMDrive:public CBaseDrive{
 // char _address=0x5D;
  char _address=0x60;

  TwoWire *pWire=&Wire;


  SCMDW myMotorDriver; //This creates the main object of one motor driver and connected slaves.
public:
 SCMDrive();
 SCMDrive(const char * str,char addr, int m=0);
  ~SCMDrive();  
  void config(const char * str,char addr=0x60, TwoWire &twowire=Wire,int m=0);
  void Address(char c);
  bool init();
  void activateDrive(float val, bool dir=true, int motor=0);
 
};
