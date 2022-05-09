#pragma once


#include "basedrive.h"
#include <consoleio.h>
#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
 // #include <mbed.h>
  #include <PwmOut.h>
using namespace mbed;
#else

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

  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  mbed::PwmOut *pMotor;
  #endif

  
public:
  
  CPWMController(const char *str,PinName sig);
  CPWMController(){setInterval(5);_channel=channel;channel++;};
  ~CPWMController();

  void setup();
  void config(PinName sig);
  void config(CMsg &msg);

  void init();
  void activateDrive(float val);  
};
