#include "mdrive.h"
#include <consoleio.h>

#define PWM_MAX 4000

  //  It uses .setDrive( motorNum <0,1>, direction<1:forward,0:backward>, level<0...255> ) to drive the motors.

CMDrive::CMDrive(){
  //Name("MAGNET");
  writeconsoleln("MDrive Constructor");
  }


CMDrive::~CMDrive(){
  //Stop();
  }

void CMDrive::runOnce(CMsg &m){  
  if(Mode()=="")  {}
}
  

  
void CMDrive::config(char addr, TwoWire *twowire, int m){
  _address=addr;
  _pWire=twowire;
  setMotor(m);   
  setForever();
  setInterval(5);  
  setModifiedTime(getTime());
  setDuration(200000);

  }



void CMDrive::init(){
  //setState("ERROR");
  int count=0;
  
  while ( !myMotorDriver.begin(_address, _pWire) ) //Wait until a valid ID word is returned
  {      
    delay(100);
    count++;
    writeconsoleln(Name());writeconsoleln("  Fail");
    if (count>5) {
      setState("ERROR");
      return;
    }
  }
  writeconsoleln(Name()); writeconsoleln("  Success");

  setState("PLAY");
  myMotorDriver.run(RELEASE);

  return;
}




void CMDrive::Address(char c){_address=c;}

 

void CMDrive::activateDrive(int val){   //0-1000
 
  writeconsole(Name());writeconsole("  Speed:");
  writeconsole(val);
  writeconsoleln("   CMDrive ---   ACTIVATE DRIVE !!!!!!!!!!");
  if (val==0){
      myMotorDriver.run(RELEASE);
      return;
  }
  int nval=abs(val)/4.0;  //0-1000 so div by 4

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


void CMDrive::test(CMsg &msg){
  CSystemObject::test(msg);
  Speed(PWM_MAX); 
  
  delay(10000);
  
  Speed(-PWM_MAX);
  
  delay(10000);
  
  
  Speed(0.0);
}