#pragma once


#include "basedrive.h"
#include <consoleio.h>

#include <CSPID.h>

#define PWM_MIN 100
#define PWM_MAX 4000





#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
 #include <analogwrite_portenta.h>  
//using namespace mbed;
#else
  #include <analogWrite.h>  
  typedef int PinName; 
#endif

//CMotorController mc1(PA_8,PC_6,PC_5);
//https://github.com/Xinyuan-LilyGO/LilyGo-LoRa-Series/issues/25
/*
Faulhaber SC1801

PWM   : 96 KHz

Up    : Power electronics
Umot  : Power motor
Unsol : Speed
FG    : Hall effect sensor
DIR   : Direction
GND   : Ground

TBeam Interrupt on Pin 25
*/

    

class PWMCounter {
private:

//https://forum.arduino.cc/t/interrupts-using-m7-vs-m4-core/675577
//need to fix  see this
    PinName _interrupt;
    std::string _axis=_XAXIS;
    double _rps=0.0;

    long _Count=0;
    long _lastCount=0;
    long _Time=0;
    long _lastTime=0;    
    
    
public:
    PWMCounter();
    void axis(std::string s) {_axis=s;}
    std::string axis(){return _axis;}
    
    void config(PinName pin);

    long read();

    long getCount();
    double getRPM();
    double getRPS();
};


class CMotorController:public CBaseDrive{
private:  

  double _Setpoint=0.0, _Input=0.0, _Input_last=0.0, _Output=0.0,_Setpoint_last=0.0,_Output_last=0.0, _x=0.0, _y=0.0, _z=0.0;
  int _OutputPWM=0;
  std::string _axis=_ZAXIS;
  double _Kp=0.0, _Ki=0.0, _Kd=0.0;
  long _lastStats=0L,_Output_duration=0L,  _t=0L;
  CMsg _mLog;

  PWMCounter pwmCounter;

  CSPID myPID;

public:
  CMotorController();
  ~CMotorController();

  void setup();
  void loop();
  void off();  

  void config(PinName sig, PinName fg,PinName dir);
 // void config(CMsg &msg);

  void init();
  double getRPM();
  double getRPS();
  long getCount();  

  void runOnce(CMsg &msg);
  void newMode(CMsg &msg);
  void newMode(std::string tmp){CMsg m; m.set(_MODE,tmp);newMode(m);}
  void newGains(CMsg &msg);

  void setupLock();
  void setupSpeed();
  void setupSpeedSimple();
  void setupPosition();


  void loopLock();
  void loopSpeed();
  void loopSpeedSimple();
  void loopPosition();

  
  void setPoint(double sp,long dur=10000000){_Setpoint=sp; changed();setDuration(dur);}
  void setPoint(CMsg &msg);
  
  void activateDrive(float val);
  void sendPlotter(CMsg &msg);

  void callCustomFunctions(CMsg &msg) override;
};

