#pragma once
#include <systemobject.h>



class CBaseDrive:public CSystemObject{
protected: 
 bool bOn=false;
  int _motor=0;
  float _mspeed=0.0;
  int _mdir=1;
  float _setSpeed=1.0;
  int _PWMSpeed=0;

  unsigned long _startTime=0;
  unsigned long _maxRunTime=10000;
  unsigned long _interval=0;
  unsigned long _modifiedTime=0;
  unsigned long _duration=0;
  unsigned long _changedOn;
public:
  CBaseDrive();
  
  void Forward(float s=1.0,unsigned long dur=0);
  void Backward(float s=1.0,unsigned long dur=0);
  void Reverse();
  bool isForward();
  virtual void Speed(float s=1.0,unsigned long dur=0);
  int Speed(){return _mspeed;};
  unsigned long Duration(){return _duration;};
  void Duration(unsigned long tmp){_duration=tmp;}
  virtual void activateDrive(float val, bool dir=true, int motor=0){}
  int Motor(){return _motor;};
  void runOnce();
  void TestMotor(); 
  virtual void loop();
  unsigned long maxRunTime(){return _maxRunTime;};
  void maxRunTime(unsigned long tmp){_maxRunTime=tmp;}
  unsigned long modifiedTime(){return _modifiedTime;};
  void modifiedTime(unsigned long tmp){_modifiedTime=tmp;}
  unsigned long interval(){return _interval;};
  void interval(unsigned long tmp){_interval=tmp;}
 
  
  void Stop();

  float setSpeed(){return _setSpeed;}
  void setSpeed(float tmp){_setSpeed=tmp;}

  unsigned long startTime(){return _startTime;};
  void startTime(unsigned long tmp){_startTime=tmp;}

  void callNewFunction(CMsg &msg);
  
};
