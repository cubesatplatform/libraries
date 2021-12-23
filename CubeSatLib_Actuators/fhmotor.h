#pragma once
#include <defs.h>
#include "basedrive.h"
#include <arduino.h>
#include "consoleio.h"
#include "system_imu.h"
#include "PID1.h"


#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
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
    PWMCounter();
    
    void init(PinName pin);

    unsigned long read();

    unsigned long getCount();
    float RPM();
    float RPS();
};


class CMotorController:public CBaseDrive{
private:

// setting PWM properties
  CIMU *_pIMU=NULL;
  const int freq = 10000;
  const int resolution = 12;

  PinName PIN_SIGNAL;
  PinName PIN_DIR;
  PinName PIN_FG;  //Feedback

 
  static int channel;
  int _channel;

  double _Setpoint=0.0, _Input=0.0, _Output=0.0,_Output_last=0.0;
  char _axis='X';
  //Specify the links and initial tuning parameters
  //double Kp=2, Ki=5, Kd=1;
  double _Kp=2.0, _Ki=5.0, _Kd=1.0;

  PWMCounter pwmCounter;

  PID myPID;

  
public:
  CMotorController();
  ~CMotorController();

  void configSpeed(PinName sig, PinName fg,PinName dir);
  void configRotation(PinName sig, PinName fg,PinName dir,CIMU *pIMU, char axis);
  void init();
  float RPM();
  float RPS();
  unsigned long getCount();
  void sendPWM(int nVal);
  void loop();
  void loopSpeed();
  void loopRotation();
  void setPoint(double sp){_Setpoint=sp;}
  void setPointRotation(double sp){_Setpoint=sp;setMode("ROTATION");}
  
  void activateDrive(float val);
};

