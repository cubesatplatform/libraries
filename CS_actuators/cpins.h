#pragma once


#include <systemobject.h>
#include <consoleio.h>


#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)

//using namespace mbed;
#else
  #include <analogWrite.h>  
  typedef int PinName; 
#endif




class CPinController:public CSystemObject{
private:

// setting PWM properties
  const int _freq = 5000;
  const int _resolution = 12;

  
  static int channel;  //Need this static (only one variable) as this defines what channl on the board is being used


  std::list<CMsg> _pins;

  
public:
  

  CPinController(); //{setInterval(5);_channel=channel;channel++;};
  ~CPinController();

  void setup(){};
  void loop();
  void config(std::string pin);
  void config(CMsg &msg);
  
  void activatePin(CMsg &msg);  
  void blinkPin(CMsg &msg);  
  int convertToPWM(float val);   //From 0 to 100.0   
  void sendPWM(PinName pin,int nVal);
};
