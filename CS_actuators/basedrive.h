#pragma once

#include "cpin.h"
#include "actuator_defs.h"
#include <systemobject.h>



class CBaseDrive:public CSystemObject{   //Speed PWM 0-1000
  bool bOn=false;  
  int _motor=0;
  float _mspeed=0.0;
  bool _mdir=true;
  int _setSpeed=1.0;
  float _PWMSpeed=0.0;    //0.0-100.0

  long _driveStartTime=0;
  long _maxRunTime=10000;
  long _driveInterval=0;
  long _modifiedTime=0;
  long _duration=10000;
  long _changedOn=0;

  
  
protected: 
  
  PinName PIN_SIGNAL;
  PinName PIN_DIR;
  PinName PIN_FG;  //Feedback
  std::string _IMU=_IMUI;
  static int channel;
  int _channel;

  int _errorIMU=0;
  int _resetIMU=0;
  int _switchResetIMU=0;
public:
  CPin cPin;
  
  CBaseDrive();

  void init();

  void forward(float s=100.0);
  void backward(float s=100.0);
  void reverse();

  bool isForward();

  void manual();
  
  virtual void speed(float val);
  
  virtual void runOnce(CMsg &m){};
  
  void sendPWM(float fVal);

  float getSpeed(){return _mspeed;};
  void testMotor(); 
  virtual void setup(){};
  virtual void loop();
  void stopActuator();

  int getSetSpeed(){return _setSpeed;}
  void setSetSpeed(int tmp){_setSpeed=tmp; }

  float getPWMSpeed(){return _PWMSpeed;};
  void setPWMSpeed(float tmp){_PWMSpeed=tmp;};


  bool setpeed(float tmp){if (tmp==_mspeed) return false; _mspeed=tmp;return true;};
 

  long getDuration(){return _duration;};
  void setDuration(long tmp){_duration=tmp;}
  

  void changed(){ _changedOn=getTime();}

  long getMaxRunTime(){return _maxRunTime;};
  void setMaxRunTime(long tmp){_maxRunTime=tmp;}
  
  long getModifiedTime(){return _modifiedTime;};
  void setModifiedTime(long tmp){_modifiedTime=tmp;}
  
  long getDriveInterval(){return _driveInterval;};
  void setDriveInterval(long tmp){_driveInterval=tmp;}
 
  void setDir(bool tmp=true){_mdir=tmp;};
  int getDir(){return _mdir;};

  long getDriveStartTime(){return _driveStartTime;};
  void setDriveStartTime(long tmp){_driveStartTime=tmp;}

  void setMotor(int tmp){_motor=tmp;};
  int getMotor(){return _motor;};

  
  void callCustomFunctions(CMsg &msg) override;  

  
  double getIMUValue(std::string sname,std::string sfield);
};
