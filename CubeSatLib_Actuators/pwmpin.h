#pragma once
#include <defs.h>
#include "basedrive.h"
#include <arduino.h>
#include "consoleio.h"
#ifdef PORTENTA
 // #include <mbed.h>
  #include <PwmOut.h>
using namespace mbed;
#else
//#include <arduino.h>
typedef int PinName; 
#endif

//CMotorController mc1(PA_8,PC_6,PC_5);



class CPWMController:public CBaseDrive{
private:

// setting PWM properties
  const int freq = 5000;
  const int resolution = 8;


  PinName PIN_SIGNAL;
  
  static int channel;  //Need this static (only one variable) as this defines what channl on the board is being used
  int _channel;

  #ifdef PORTENTA
  mbed::PwmOut *pMotor;
  #endif


 // unsigned long _startTime=0;
 // unsigned long _maxRunTime=10000;
 // unsigned long _modifiedTime=0;
 // unsigned long _interval=0;
 // unsigned long _duration=0;
 // unsigned long _changedOn;
 // float _setSpeed=1.0;
 // float _currentSpeed=0.0;
 // bool bOn=false;
 // void Speed(float x=1.0);
  
public:
  
  CPWMController(const char *str,PinName sig);
  CPWMController(){_INTERVAL=5;_channel=channel;channel++;};
  ~CPWMController();

  void setup();
  void config(const char *str, PinName sig);

  void Init();
  void activateDrive(float val, bool dir=true, int motor=0);

 // void start(){bOn=true; _startTime=getTime();loop();};
 // void stop(){bOn=false;loop();};
 // int Speed();
 // void Test();
 // void Log();
 // void loop();
 // unsigned long Duration(){return _duration;};
 // void Duration(unsigned long tmp){_duration=tmp;}
//  float setSpeed(){return _setSpeed;}
//  void setSpeed(float tmp){_setSpeed=tmp;}
//  unsigned long startTime(){return _startTime;};
//  void startTime(unsigned long tmp){_startTime=tmp;}
 // unsigned long maxRunTime(){return _maxRunTime;};
 // void maxRunTime(unsigned long tmp){_maxRunTime=tmp;}
 // unsigned long modifiedTime(){return _modifiedTime;};
 // void modifiedTime(unsigned long tmp){_modifiedTime=tmp;}
 // unsigned long interval(){return _interval;};
 // void interval(unsigned long tmp){_interval=tmp;}
 // void callNewFunction(CMsg &msg);
 // void Log(const char *str);
  
};
