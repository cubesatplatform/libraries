#include "mdrive.h"
#include <consoleio.h>

#define PWM_MAX 4000

  //  It uses .setDrive( motorNum <0,1>, direction<1:forward,0:backward>, level<0...255> ) to drive the motors.

CMDrive::CMDrive(){  
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
  setErrorThreshold(5);

  }

  
void CMDrive::config(CMsg &msg){
  std::string straddress=msg.getParameter("ADDRESS");
  std::string strwire=msg.getParameter("WIRE");

  TwoWire *pWire;

  if(strwire=="Wire") pWire=&Wire;  
  if(strwire=="Wire1") pWire=&Wire1;  
  if(strwire=="Wire2") pWire=&Wire2;  

  if(straddress.size()>0){
    config(straddress[0],pWire);
  }
}



void CMDrive::init(){  
  for(int count=0;count<5;count++){  
    if( !myMotorDriver.begin(_address, _pWire) ) {//Wait until a valid ID word is returned          
      incErrorCount();        
      }
      else{
        writeconsoleln(Name()); writeconsoleln("  Success");
        setState("PLAY");
        myMotorDriver.run(RELEASE);
        return;
      }
    } 
  sendError();
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
  Run(50);
  Speed(PWM_MAX); 
  
  delay(10000);
  
  Speed(-PWM_MAX);
  
  delay(10000);
  
  
  Speed(0.0);
}