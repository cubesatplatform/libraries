#include "acds_defs.h"
#include "system_reactionwheel.h"

//https://randomnerdtutorials.com/esp32-pwm-arduino-ide/
//https://circuits4you.com/2018/12/31/esp32-pwm-example/
//https://circuits4you.com/2017/12/21/esp8266-pwm-example/``
// the number of the LED pin
//const int ledPin = 2;  // 16 corresponds to GPIO16

#define GYROLIMIT 0.1
#define GYROPERIOD 50
#define GYROWAIT 50


CRW::CRW():CSystemObject(){
  init();
  
}





void CRW::setup()
{
  init();
  
  
  setForever();
  setInterval(20);


  setState(_PLAY);
}


void  CRW::callCustomFunctions(CMsg &msg){
  
  
  CSystemObject::callCustomFunctions(msg);
}

void CRW::loop(){ 
  if (Mode()==_BLANK){   //Do some control
  }
}

