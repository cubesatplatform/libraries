#pragma once
#include "mdrive.h"
#include "consoleio.h"

  //  It uses .setDrive( motorNum <0,1>, direction<1:forward,0:backward>, level<0...255> ) to drive the motors.

CMDrive::CMDrive(){}
CMDrive::CMDrive(const char * str,char addr, int m){Name(str);_address=addr;_motor=m;init();}
CMDrive::~CMDrive(){Stop();}

void CMDrive::Address(char c){_address=c;}

 

void CMDrive::activateDrive(float val, bool dir, int motor){
  writeconsoleln("CMDrive ---   ACTIVATE DRIVE !!!!!!!!!!");
  //myMotorDriver.run(RELEASE);
  int nval=val*255.0;

  if(dir)
    myMotorDriver.run(FORWARD);
  else
    myMotorDriver.run(BACKWARD);
  myMotorDriver.setSpeed(nval);  
}
  
  
void CMDrive::config(const char * str,char addr, TwoWire &twowire,int m){
  Name(str);
  _address=addr;
  _motor=m;
  _forever=true;
  _INTERVAL=10;
  pWire=&twowire;

  init();
  }

  

bool CMDrive::init(){
  setState("ERROR");
  int count=0;
  

  while ( !myMotorDriver.begin(_address, pWire) ) //Wait until a valid ID word is returned
  {      
    delay(100);
    count++;

      writeconsole(Name()); writeconsole(myMotorDriver.begin());writeconsoleln("  Fail");
    if (count>5) return false;
  }

  writeconsole(Name()); writeconsoleln("  Success");
  setState("READY");
}

