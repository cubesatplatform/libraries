#pragma once

#include "actuator_defs.h"
#include <systemobject.h>
#define PIN_RESOLUTION 12
#define PIN_FREQUENCY 10000


class CBaseDrive:public CSystemObject{   //Speed PWM 0-1000
  bool bOn=false;  
  int _motor=0;
  int _mspeed=0.0;
  bool _mdir=true;
  int _setSpeed=1.0;
  int _PWMSpeed=0;

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
  CBaseDrive();

  void init();

  void forward(int s=1000, long dur=0);
  void backward(int s=1000, long dur=0);
  void reverse();

  bool isForward();

  void manual();
  
  virtual void speed(int s=1000, long dur=0);
  
  virtual void activateDrive(float val){}
  virtual void runOnce(CMsg &m){};
  
  void sendPWM(int nVal);

  int getSpeed(){return _mspeed;};
  void testMotor(); 
  virtual void setup(){};
  virtual void loop();
  void stopActuator();

  int getSetSpeed(){return _setSpeed;}
  void setSetSpeed(int tmp){_setSpeed=tmp; }

  int getPWMSpeed(){return _PWMSpeed;};
  void setPWMSpeed(int tmp){_PWMSpeed=tmp;};

  int getMSpeed(){return _mspeed;};
  bool setMSpeed(int tmp){if (tmp==_mspeed) return false; _mspeed=tmp;return true;};
 

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
  int convertToPWM(float val);

  
  double getIMUValue(std::string sname,std::string sfield);
};
