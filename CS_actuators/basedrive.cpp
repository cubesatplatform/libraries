#include "basedrive.h"
#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
#include <analogWrite_portenta.h>  
#else
  #include <analogWrite.h>  
#endif
#include <consoleio.h>

#include <Wire.h>

int CBaseDrive::channel=0;

// It uses .setDrive( motorNum <0,1>, direction<1:forward,0:backward>, level<0...255> ) to drive the motors.

CBaseDrive::CBaseDrive(){}

void CBaseDrive::forward(float s){speed(abs(s));}
void CBaseDrive::backward(float s){speed(-1*abs(s));}
void CBaseDrive::reverse(){speed(-1.0*_mspeed);}
bool CBaseDrive::isForward(){if(_mdir) return true; return false;}
void CBaseDrive::stopActuator(){
  speed(0.0); 
  setState(_PAUSE);
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



  setForever();
}

void CBaseDrive::sendPWM(float fVal){

  if (fVal==getPWMSpeed()){   //Do nothing if same speed
    writeconsole("PWM same  DIDNT skip THOUGH ");    writeconsoleln(fVal);
   // return;
  }
  setPWMSpeed(fVal);
  
  cPin.pct(fVal);
  
}

void CBaseDrive::speed(float val){

  cPin.pct(val);
  }

  



void CBaseDrive::testMotor(){
  // Operate the Motor Driver 
  //  This walks through all 34 motor positions driving them forward and back.
  //  It uses .setDrive( motorNum, direction, level ) to drive the motors.

    for (int y=0; y<1000; y+=20){
      speed(y);
      delay(20);
    }
         
    stopActuator();
  }
 

void CBaseDrive::loop(){
  if(_duration==0)
    return;
  long ct=getTime();
  
  if(ct>_changedOn+_duration){
    stopActuator();            
    writeconsoleln("Stopping Time's up");
    _duration=0;
    return;
    }
  CMsg m;
  runOnce(m);
}


void CBaseDrive::manual(){    
  float fspeed=_m.get(_SPEED,0.0);
     
  long startTime=_m.get(_STARTTIME,0L);        
  long stopTime=_m.get(_STOPTIME,0L);        
         
  long lastTime=_m.get(_LAST,0L);
  long currentTime=getTime();
    

  if(lastTime==0){      
    startTime=startTime+currentTime;
    lastTime=startTime;

    if(stopTime<STOPTASKLIMIT)
      stopTime=startTime+stopTime;     
    else 
      stopTime=STOPTASKMAX;
    
    _m.set(_START,startTime);     
    _m.set(_STOP,stopTime);           
    _m.set(_LAST,lastTime);                 
    speed(fspeed);
    return;
    }   


  
  if(currentTime>stopTime){      
    lastTime=currentTime;
    _m.set(_LAST,lastTime);      
    speed(0.0);              
    return;      
  }
 
}






void CBaseDrive::callCustomFunctions(CMsg &msg){   //Calls a specific function directly
  
  std::string act=msg.get(_ACT);  
  float val=msg.get(_VALUE,100.00);
  long duration=msg.get(_DURATION,0);
  
  writeconsoleln(msg.serializeout());
  //std::string callback=msg.getParameter("CALLBACK");
 // if(callback.size()) _cmsg.Parameters["CALLBACK"]=callback;
  if (act==_GOFORWARD){forward(val); return;}
  if (act==_GOBACKWARD) {backward(val); return;}
  if (act==_GOREVERSE) {reverse(); return;}
  if (act==_STOP) {stopActuator(); return;}
  if (act==_SPEED) {speed(val); return;}
  if (act==_START) {forward(val); return;}

  if (act==_SENDPWM) {sendPWM(val); return;}
  if (act==_SETSETSPEED) {setSetSpeed(val); return;}
  if (act==_SETPWMSPEED) {setPWMSpeed(val); return;}

  if (act==_SETDURATION) {setDuration(duration); return;}
  if (act==_SETMAXRUNTIME) {setMaxRunTime(duration); return;}
  if (act==_SETMODIFIEDTIME) {setModifiedTime(duration); return;}
  if (act==_SETDIR) {setDir(val); return;}
  if (act==_SETDRIVEINTERVAL) {setDriveInterval(val); return;}
  if (act==_SETDRIVESTARTTIME) {setDriveStartTime(val); return;}

  if (act==_GETDURATION) {transmitResult(act,tostring(getDuration())); return;}
  if (act==_GETSPEED) {transmitResult(act,tostring(getSpeed())); return;}
  if (act==_GETSETSPEED) {transmitResult(act,tostring(getSetSpeed())); return;}
  if (act==_GETPWMSPEED) {transmitResult(act,tostring(getPWMSpeed())); return;}
  if (act==_GETMAXRUNTIME) {transmitResult(act,tostring(getMaxRunTime())); return;}
  if (act==_GETMODIFIEDTIME) {transmitResult(act,tostring(getModifiedTime())); return;}
  if (act==_GETDIR) {transmitResult(act,tostring(getDir())); return;}

  if (act==_GETDRIVEINTERVAL) {transmitResult(act,tostring(getDriveInterval())); return;}
  if (act==_GETDRIVESTARTTIME) {transmitResult(act,tostring(getDriveStartTime())); return;}

  CSystemObject::callCustomFunctions(msg);
}

double CBaseDrive::getIMUValue(std::string sname,std::string sfield){  
  std::string key=_IMU+sname;

  CMsg msg=getDataMap(key);

//  msg.writetoconsole();
  
  long t=msg.get(_TIME,0L);

  if(getTime()-t>1000){    
    if(_resetIMU>5){
      Wire.begin();
      Wire.begin();
    if(_IMU==_IMUI){ _IMU=_IMUS; }      
      else{_IMU=_IMUI;}
      _resetIMU=0;
    }
    else{
      _errorIMU++;
      if(_errorIMU>50){
        _errorIMU=0;
        _resetIMU++;
        CMsg m;
        m.set(_SYS,_IMUI);
        m.set(_ACT,"GO"+sname);
        addMessageList(m);      
        //writeconsoleln("IMU ERROR..........................................................................................................");           
      }
    }
  }



  double val=0.0;
  val=msg.get(sfield,val); 
  return val;
}