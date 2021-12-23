#include "pwmpin.h"

int CPWMController::channel=0;  

CPWMController::CPWMController(const char *str, PinName sig){
  _channel=channel;channel++;
  config(sig);
  }

  CPWMController::~CPWMController(){
    #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
    if (pMotor!=NULL) delete pMotor;
    #endif
  }


void CPWMController::config( PinName sig){
    setInterval(50);
    PIN_SIGNAL=sig;
    init();
}



  void CPWMController::init(){
    
    #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
      pMotor=new mbed::PwmOut(PIN_SIGNAL);
      pMotor->period_ms(1);   //PWM signal must have a fixed frequency between 500Hz and 18Khz.   1==1000Hz   2=500Hz
      pMotor->write(0.0);      //Percent from 0 to 1
    
    #endif

      
  #ifdef TTGO
    ledcSetup(_channel, freq, resolution);   // configure LED PWM functionalitites
    ledcAttachPin(PIN_SIGNAL, _channel);  // attach the channel to the GPIO to be controlled
  #endif

 
  }


void CPWMController::setup(){} //bOn=false;

void CPWMController::activateDrive(float val){ 
  writeconsoleln("PWMController ---   ACTIVATE DRIVE !!!!!!!!!!");
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
    pMotor->write(val);
  #else
      float fval=255.0*abs(val);
      //fval=255.0;
     
      ledcWrite(_channel, (int) fval); 
 
  #endif
}
