#include "basedrive.h"
#include <arduino.h>
#include "consoleio.h"

// It uses .setDrive( motorNum <0,1>, direction<1:forward,0:backward>, level<0...255> ) to drive the motors.

CBaseDrive::CBaseDrive(){
  
  
  }

void CBaseDrive::Forward(float s,unsigned long dur){Speed(abs(s),dur);}
void CBaseDrive::Backward(float s,unsigned long dur){Speed(-1.0*abs(s),dur);}
void CBaseDrive::Reverse(){Speed(-1.0*_mspeed);}
bool CBaseDrive::isForward(){if(_mdir) return true; return false;}
void CBaseDrive::stopActuator(){//Speed(0.0,0); 
   setState("PAUSE");}

void CBaseDrive::init(){
  CSystemObject::init();

  bOn=false;  
  _motor=0;
  _mspeed=0.0;
  _mdir=1;
  _setSpeed=1.0;
  _PWMSpeed=0;

  _driveStartTime=0;
  _maxRunTime=10000;
  _driveInterval=0;
  _modifiedTime=0;
  _duration=0;
  _changedOn=0;
  setForever();
}

void CBaseDrive::Speed(float s,unsigned long dur){
  if(State()!="PLAY")
    setState("PLAY");

  if(_driveStartTime==0) _driveStartTime=getTime();
  _changedOn=getTime();

  _mspeed=s;
  
  //activateDrive(_mspeed);   //For testing  put bacl
  if (dur>0) _duration=dur;
  }

  

void CBaseDrive::runOnce(CMsg &m){
}


void CBaseDrive::TestMotor(){
  // Operate the Motor Driver 
  //  This walks through all 34 motor positions driving them forward and back.
  //  It uses .setDrive( motorNum, direction, level ) to drive the motors.

    for (float y=0.0;y<1.0; y+=0.05){
      Speed(y);
      delay(20);
    }
         
    stopActuator();
  }
 

void CBaseDrive::loop(){
    if(_duration<1){
    }
    unsigned long ct=getTime();
    
    if(ct>_changedOn+_duration){
      stopActuator();            
    }
  CMsg m;
  runOnce(m);
  }

void CBaseDrive::callNewFunction(CMsg &msg){   //Calls a specific function directly
  std::string act=msg.getParameter("ACT");  
  float speed=msg.getParameter("SPEED",(float)1.0);
  unsigned long duration=msg.getParameter("DURATION",0);

  writeconsoleln(msg.serializeout());
  //std::string callback=msg.getParameter("CALLBACK");
 // if(callback.size()) _cmsg.Parameters["CALLBACK"]=callback;
  if (act=="FORWARD") Forward(speed,duration);
  if (act=="BACK") Backward(speed,duration);
  if (act=="REVERSE")Reverse();
  if (act=="STOP") stopActuator();
  if (act=="SPEED") Speed(speed,duration);
  if (act=="START") Forward(speed,duration);

}