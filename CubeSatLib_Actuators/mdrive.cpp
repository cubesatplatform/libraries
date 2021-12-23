#pragma once
#include "mdrive.h"
#include "consoleio.h"

  //  It uses .setDrive( motorNum <0,1>, direction<1:forward,0:backward>, level<0...255> ) to drive the motors.

CMDrive::CMDrive(){
  Name("MAGNET");
  writeconsoleln("MDrive Constructor");
  }


CMDrive::~CMDrive(){
  //Stop();
  }

  
  
void CMDrive::config(char addr, TwoWire *twowire,int m){
  _address=addr;
  setMotor(m);
  setForever();
  setInterval(10);
  pWire=twowire;
  setDuration(20000);
  init();
  }



void CMDrive::init(){
  //setState("ERROR");
  int count=0;
  

  while ( !myMotorDriver.begin(_address, pWire) ) //Wait until a valid ID word is returned
  {      
    delay(100);
    count++;

      writeconsole(Name());writeconsoleln("  Fail");
    if (count>5) {
      setState("ERROR");
      return;
    }
  }

  writeconsole(Name()); writeconsoleln("  Success");
  //setState("IDLE");
  setState("READY");
  myMotorDriver.run(RELEASE);

  return;
}




void CMDrive::Address(char c){_address=c;}

 

void CMDrive::activateDrive(float val){
 
  writeconsole(Name());
  writeconsole(val);
  writeconsoleln("   CMDrive ---   ACTIVATE DRIVE !!!!!!!!!!");
  if (val==0.0){
      myMotorDriver.run(RELEASE);
      return;
  }
  int nval=abs(val)*255.0;

  if(val>0.0){
    setDir(true);
  }

  if(val<0.0){
    setDir(false);
  }

  if(getDir())
    myMotorDriver.run(FORWARD);
  else
    myMotorDriver.run(BACKWARD);

  myMotorDriver.setSpeed(nval);  
}


