#pragma once
#include "basedrive.h"
#include <arduino.h>
#include "consoleio.h"

  //  It uses .setDrive( motorNum <0,1>, direction<1:forward,0:backward>, level<0...255> ) to drive the motors.

  CBaseDrive::CBaseDrive(){_forever=true;}
 
  void CBaseDrive::Forward(float s,unsigned long dur){Speed(s,dur);}
  void CBaseDrive::Backward(float s,unsigned long dur){Speed(-1.0*s,dur);}
  void CBaseDrive::Reverse(){Speed(-1.0*_mspeed);}
  bool CBaseDrive::isForward(){if(_mdir) return true; return false;}
  void CBaseDrive::Stop(){Speed(0.0,0);}

  void CBaseDrive::Speed(float s,unsigned long dur){
    if(_ostate!="PLAY")
      setState("PLAY");
    writeconsoleln("SPEED CALLED xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx") ;
    if(_startTime==0) _startTime=getTime();
    _changedOn=getTime();

    _mspeed=s;

    
    activateDrive(_mspeed,_mdir,_motor);
    if (dur>0) _duration=dur;
    }

  

void CBaseDrive::runOnce(){
 // writeconsoleln(Name());
    if(_duration<1){
    }
  
    unsigned long ct=getTime();
    
    if((ct>_changedOn+_duration)&&(_mspeed!=0.0)){
      Stop();
      CMsg m;
      m.Parameters[Name()]="1"; 
      respondCallBack(m);
      //setState("IDLE");
    }

}



void CBaseDrive::TestMotor(){
  // Operate the Motor Driver 
  //  This walks through all 34 motor positions driving them forward and back.
  //  It uses .setDrive( motorNum, direction, level ) to drive the motors.

    for (float y=0.0;y<1.0; y+=0.05){
      Speed(y);
      delay(20);
    }
         
    Stop();
  }
 



void CBaseDrive::loop(){
  runOnce();
  }

void CBaseDrive::callNewFunction(CMsg &msg){   //Calls a specific function directly
  std::string act=msg.getParameter("ACT");  
  float speed=msg.getParameter("SPEED",(float)1.0);
  unsigned long duration=msg.getParameter("DURATION",0);

  writeconsoleln("");
  writeconsole("BaseDrive Speed, Dir:");
  writeconsole(speed);
  writeconsoleln(duration);

  //std::string callback=msg.getParameter("CALLBACK");
 // if(callback.size()) _cmsg.Parameters["CALLBACK"]=callback;
  if (act=="FORWARD") Forward(speed,duration);
  if (act=="BACK") Backward(speed,duration);
  if (act=="REVERSE")Reverse();
  if (act=="Stop") Stop();
  if (act=="SPEED") Speed(speed,duration);
}