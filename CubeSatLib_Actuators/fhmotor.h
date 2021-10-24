#pragma once
#include <defs.h>
#include "basedrive.h"
#include <arduino.h>
#include "consoleio.h"
#ifdef PORTENTA
 // #include <mbed.h>
 // #include <PwmOut.h>
using namespace mbed;
#else
//#include <arduino.h>
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
   // mbed::InterruptIn _interrupt;

    PinName _interrupt;

    unsigned long lastCount=0;
    unsigned long prevT=0;
    unsigned long lastT=0;
    
public:
    PWMCounter(PinName pin);

    unsigned long read();

    unsigned long count();
    float RPM();
    float RPS();
};


class CMotorController:public CBaseDrive{
private:

// setting PWM properties
  const int freq = 10000;
  const int resolution = 12;

  PinName PIN_SIGNAL;
  PinName PIN_DIR;
  PinName PIN_FG;  //Feedback

 
  static int channel;
  int _channel;

  PWMCounter *pCounter=NULL;

  
public:
  CMotorController();
  ~CMotorController();

  void config(const char  *str,PinName sig, PinName fg,PinName dir);
  void Init();
  float RPM();
  float RPS();
  unsigned long Count();
  void sendPWM(int nVal);
  
  void activateDrive(float val, bool dir=true, int motor=0);
 
};

