#pragma once
#include "mdrive.h"
#include "consoleio.h"

  //  It uses .setDrive( motorNum <0,1>, direction<1:forward,0:backward>, level<0...255> ) to drive the motors.

CMDrive::CMDrive(){
   writeconsoleln("aaaaaaaaaaaaaaaaaaaaaaaaaa");
}


CMDrive::CMDrive(const char * str,char addr, int m){
   writeconsoleln("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
  Name(str);
  _address=addr;
  _motor=m;
  init();
  }


CMDrive::~CMDrive(){
  //Stop();
  }

void CMDrive::Address(char c){_address=c;}

 

void CMDrive::activateDrive(float val, bool dir, int motor){
 
  writeconsole("CCCCCCCCCCCCCC   ");
  writeconsole(val);
  writeconsoleln("   CMDrive ---   ACTIVATE DRIVE !!!!!!!!!!");
  if (val==0.0){
      myMotorDriver.run(RELEASE);
      return;
  }
  int nval=abs(val)*255.0;

  if(val>0.0){
    if(!dir)_mdir=true;
  }

  if(val<0.0){
    if(dir)_mdir=false;
  }

  if(_mdir)
    myMotorDriver.run(FORWARD);
  else
    myMotorDriver.run(BACKWARD);

  myMotorDriver.setSpeed(nval);  
}
  
  
void CMDrive::config(const char * str,char addr, TwoWire *twowire,int m){
  Name(str);
  _address=addr;
  _motor=m;
  _forever=true;
  _INTERVAL=10;
  pWire=twowire;

  init();
  }

  

bool CMDrive::init(){
  setState("ERROR");
  int count=0;
  

  while ( !myMotorDriver.begin(_address, pWire) ) //Wait until a valid ID word is returned
  {      
    delay(100);
    count++;

      writeconsole(Name());writeconsoleln("  Fail");
    if (count>5) return false;
  }

  writeconsole(Name()); writeconsoleln("  Success");
  setState("READY");
  myMotorDriver.run(RELEASE);

  return true;
}

