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

/*
Faulhaber SC1801

PWM   : 96 KHz

Up    : Power electronics
Umot  : Power motor
Unsol : Speed
FG    : Hall effect sensor
DIR   : Direction
GND   : Ground
*/


class PWMCounter {
private:
#ifdef PORTENTA
//https://forum.arduino.cc/t/interrupts-using-m7-vs-m4-core/675577
//need to fix  see this
    //mbed::InterruptIn _interrupt;
     int _interrupt;
#else     
  int _interrupt;
#endif
    volatile long _count=0;
    long lastCount;
    long prevT=0;
    long lastT=0;
    
public:
    PWMCounter(PinName pin);
    void increment();
    int read();

    long count();
    float RPM();
};


class CMotorController:public CBaseDrive{
private:

// setting PWM properties
  const int freq = 10000;
  const int resolution = 8;

  PinName PIN_SIGNAL;
  PinName PIN_DIR;
  PinName PIN_FG;  //Feedback

 // unsigned long _startTime=0;
 // unsigned long _stopTime=0;

  static int channel;
  int _channel;

 // int _mdir=true;
 // int _mspeed=0.0;
 // unsigned long _duration=0;
 // unsigned long _changedOn;
  PWMCounter *pCounter;

  #ifdef PORTENTA
  mbed::PwmOut *pMotor;
  #endif

  
  
  
public:

  CMotorController();
  ~CMotorController();

  void config(const char  *str,PinName sig, PinName fg,PinName dir);
  void Init();
  float RPM();
  long Count();
  
  void activateDrive(float val, bool dir=true, int motor=0);
 // void Speed(float s, unsigned long dur=0);
 // int Speed();
 // void Stop();
 // void Forward(float s=255,unsigned long dur=0);
 // void Backward(float s=255,unsigned long dur=0);
 // void Reverse();
  //void Test();
  //void Log();
  //void loop();
  //void runOnce();
  /*
  unsigned long startTime(){return _startTime;};
  unsigned stopTime(){return _stopTime;};
  unsigned changedTime(){return _changedOn;};
  unsigned Duration(){return _duration;};
  void Duration(unsigned long tmp){_duration=tmp;}

  void CallFunction(CMsg &msg){   //Calls a specific function directly
    std::string act=msg.getParameter("ACT");
    float speed=msg.getParameter("SPEED",(float)1.0);
    unsigned long duration=msg.getParameter("DURATION",50);
    if (act=="Forward") Forward(speed);
    if (act=="Back") Backward(speed);
    if (act=="Reverse")Reverse();
    if (act=="Stop") Stop();
    if (act=="Speed") Speed(speed);
  }
*/  
};

