#pragma once

#include <string>


#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
#include <PwmOut.h>
#else
  typedef int PinName;
#endif


class CPin{
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
    mbed::PwmOut *led=NULL;
    
    constant int _resolution = 4095; // 12-bit resolution for PWM (0-4095)      Probably not used as this thing wants a percentage
  #else
    //int _ledPin; // Connect the LED to GPIO13
    const int _resolution = 12; // 8-bit resolution for PWM (0-255)    
    static int _ledChannel; // PWM channel
    int _pinChannel; // PWM channel    
    int _brightness=0;
  #endif  
  
    PinName _ledPin; // Connect the LED to GPIO13
    int _freq = 5000; // PWM frequency in Hz
    

  public:
    
   
    CPin(){};   
    ~CPin(){
      #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
        delete led;
      #endif
    }

    void config (PinName pin, int freq=5000);
    void config (std::string pin, int freq=5000);
    void pct(float p);
    void print();
};
