#include "pwmpin.h"

int CPWMController::channel=0;


  

CPWMController::CPWMController(const char *str, PinName sig){
  _channel=channel;channel++;
  config(str, sig);

  }

  CPWMController::~CPWMController(){
    #ifdef PORTENTA    
    if (pMotor!=NULL) delete pMotor;
    #endif
  }


void CPWMController::config(const char *str, PinName sig){
  _INTERVAL=50;
    Name(str);
    PIN_SIGNAL=sig;
    Init();
}



  void CPWMController::Init(){
    
    #ifdef PORTENTA
      pMotor=new mbed::PwmOut(PIN_SIGNAL);
      pMotor->period_ms(1);   //PWM signal must have a fixed frequency between 500Hz and 18Khz.   1==1000Hz   2=500Hz
      pMotor->write(0.0);      //Percent from 0 to 1
    
    #endif

      
  #ifdef TTGO
    ledcSetup(_channel, freq, resolution);   // configure LED PWM functionalitites
    ledcAttachPin(PIN_SIGNAL, _channel);  // attach the channel to the GPIO to be controlled
  #endif

 
  }


void CPWMController::setup(){bOn=false;}
void CPWMController::activateDrive(float val, bool dir, int motor){ 
  writeconsoleln("PWMController ---   ACTIVATE DRIVE !!!!!!!!!!");
  #ifdef PORTENTA     
    pMotor->write(val);
  #else
      float fval=255.0*val;
      //fval=255.0;
     
      ledcWrite(_channel, (int) fval); 
 
  #endif

}
