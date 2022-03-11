#include "basedrive.h"
#include <Arduino.h>
#include "consoleio.h"

int CBaseDrive::channel=0;

// It uses .setDrive( motorNum <0,1>, direction<1:forward,0:backward>, level<0...255> ) to drive the motors.

CBaseDrive::CBaseDrive(){}

void CBaseDrive::Forward(int s,unsigned long dur){Speed(abs(s),dur);}
void CBaseDrive::Backward(int s,unsigned long dur){Speed(-1*abs(s),dur);}
void CBaseDrive::Reverse(){Speed(-1*_mspeed);}
bool CBaseDrive::isForward(){if(_mdir) return true; return false;}
void CBaseDrive::stopActuator(){
  Speed(0,0); 
  setState("PAUSE");
}

void CBaseDrive::init(){
  CSystemObject::init();

  bOn=false;  
  _motor=0;
  _mspeed=0;
  _mdir=1;
  _setSpeed=1000;
  _PWMSpeed=0;

  _driveStartTime=0;
  _maxRunTime=10000;
  _driveInterval=0;
  _modifiedTime=0;
  _duration=0;
  _changedOn=0;



  #ifdef TTGO || TTGO1
    ledcSetup(_channel, freq, resolution);   // configure LED PWM functionalitites
    ledcAttachPin(PIN_SIGNAL, _channel);  // attach the channel to the GPIO to be controlled
  #endif
  setForever();
}

void CBaseDrive::sendPWM(int nVal){

  if (nVal==getPWMSpeed()){   //Do nothing if same speed
    writeconsole("skip ");
    writeconsoleln(nVal);
    return;
  }
  setPWMSpeed(nVal);
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
    analogWrite(PIN_SIGNAL,nVal);
  #else
    ledcWrite(_channel, nVal);  
  #endif
}

void CBaseDrive::Speed(int s,unsigned long dur){
  if(State()!="PLAY")
    setState("PLAY");

  if(_driveStartTime==0) _driveStartTime=getTime();
  _changedOn=getTime();

  _mspeed=s;
  
  activateDrive(_mspeed);   
  _duration=dur;
  }

  



void CBaseDrive::TestMotor(){
  // Operate the Motor Driver 
  //  This walks through all 34 motor positions driving them forward and back.
  //  It uses .setDrive( motorNum, direction, level ) to drive the motors.

    for (int y=0; y<1000; y+=20){
      Speed(y);
      delay(20);
    }
         
    stopActuator();
  }
 

void CBaseDrive::loop(){
  if(_duration==0)
    return;
  unsigned long ct=getTime();
  
  if(ct>_changedOn+_duration){
    stopActuator();            
    writeconsoleln("Stoppping Time's up");
    _duration=0;
    return;
    }
  CMsg m;
  runOnce(m);
}

void CBaseDrive::callNewFunction(CMsg &msg){   //Calls a specific function directly
  std::string act=msg.getParameter("ACT");  
  int speed=msg.getParameter("SPEED",1000);
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
