#pragma once

#include "scmdrive.h"
#include "consoleio.h"

  //  It uses .setDrive( motorNum <0,1>, direction<1:forward,0:backward>, level<0...255> ) to drive the motors.

  SCMDrive::SCMDrive(){}
  SCMDrive::SCMDrive(const char * str,char addr, int m){Name(str);_address=addr;_motor=m;init();}
  SCMDrive::~SCMDrive(){Stop();}

  void SCMDrive::Address(char c){_address=c;}

 

void SCMDrive::activateDrive(float val, bool dir, int motor){
  writeconsoleln("SCMDrive ---   ACTIVATE DRIVE !!!!!!!!!!");

  int nval=val*255.0;
  myMotorDriver.setDrive(motor,dir,nval);
}
  
  
  void SCMDrive::config(const char * str,char addr, TwoWire &twowire,int m){
    Name(str);
    _address=addr;
    _motor=m;
    _forever=true;
    _INTERVAL=10;
    pWire=&twowire;

    init();
    }

   

  bool SCMDrive::init(){
    setState("ERROR");
    int count=0;
    myMotorDriver.settings.commInterface = I2C_MODE;
    myMotorDriver.settings.I2CAddress = _address; //config pattern "0101" on board for address 0x5A
    myMotorDriver.Wire=pWire;
    //  set chip select if SPI selected with the config jumpers
    //myMotorDriver.settings.chipSelectPin = 10;
  
    while ( myMotorDriver.begin() != 0xA9 ) //Wait until a valid ID word is returned
    {      
      delay(100);
      count++;

        writeconsole(Name()); writeconsole(myMotorDriver.begin());writeconsoleln("  Fail");
      if (count>5) return false;
    }

  //  Check to make sure the driver is done looking for slaves before beginning
  count=0;
  while ( myMotorDriver.ready() == false ) {
    delay(100);
    count++;
    if (count>5) return false;
  }


  //*****Set application settings and enable driver*****//
  count=0;
  while ( myMotorDriver.busy() ){
    delay(100);
    count++;
    if (count>10) return false;
  }
  myMotorDriver.enable();
  writeconsole(Name()); writeconsoleln("  Success");
  setState("READY");
}

