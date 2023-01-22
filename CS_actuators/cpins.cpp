#include "cpins.h"
#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  #include <analogWrite_portenta.h>  
#else
  #include <analogWrite.h>    
#endif
 
#define _BLINK "BLINK"
#define _MODE_PWM "MODE_PWM"

//TO:BS291~SYS:PINCONTROLLER~ACT:CONFIG~V:.4~PIN:24
#define PINCONTROLLER "PINCONTROLLER"

int CPinController::channel=0;  

CPinController::CPinController(){  ///Never init the static stuff here  do later  otherwise crashes

  setInterval(10);

  }

CPinController::~CPinController(){
  for(auto it=_pins.begin();it!=_pins.end();it++){
      it->set(_VALUE,0.0);
      activatePin(*it);
    }
}



void CPinController::config(CMsg &msg){
  msg.writetoconsole();
  std::string strPin=msg.get(_PIN);
  
  int del=msg.get(_DELETE,0);
  for(auto it=_pins.begin();it!=_pins.end();it++){
    if(it->get(_PIN)==strPin){
      if(del==1){
        it->set(_VALUE,0.0);
        activatePin(*it);
        _pins.erase(it);
        return;
      }
      it->clear();
      (*it)=msg;
      return;
    }
  }
            
  #if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
      analogWriteResolution(Pins[strPin],_resolution);   
  #endif

  if(del!=1)  _pins.push_back(msg);
}




void CPinController::loop(){  
  for(auto it=_pins.begin();it!=_pins.end();it++){
  // writeconsole("CPinController::loop");  writeconsoleln(count++);
    it->writetoconsole();

    std::string strMode=it->get(_MODE);  
    if((strMode==_MODE_PWM)||(strMode==_BLANK)){
      activatePin(*it);
    }

    if(strMode==_BLINK){
      blinkPin(*it);
    }
  }
}  





void CPinController::activatePin(CMsg &msg){  //from 0 to 100%
 int update=msg.get(_UPDATE,1);

    if (update==0){
      return;
    }
    msg.set(_UPDATE,0);

    std::string strMode=msg.get(_MODE);

  std::string strPin=msg.get(_PIN);
  
  float val=msg.get(_VALUE,0.0);
  update=msg.get(_UPDATE,1);


/*
  if(val<=0){    
    digitalWrite(pin,LOW);      
  }

  if(val>=0){      
    digitalWrite(pin,HIGH);
  }
  */
          
  sendPWM(Pins[strPin],convertToPWM(val));
}


void CPinController::blinkPin(CMsg &msg){  //from 0 to 100%
  writeconsoleln("CPinController::blinkPin");
  long interval=msg.get(_INTERVAL,5000);
  long lastTime=msg.get(_LAST,0);
  long now=millis();


  if((now-lastTime)<interval){
    return;
  }

  msg.set(_LAST,now);

  
  

  std::string strPin=msg.get(_PIN);
  int val=msg.get(_VALUE,0);

  if(val==0){
    val=100;
  }else{
    val=0;
  }

  msg.set(_VALUE,val);



/*
  if(val<=0){    
    digitalWrite(pin,LOW);      
  }

  if(val>=0){      
    digitalWrite(pin,HIGH);
  }
  */
          
  sendPWM(Pins[strPin],convertToPWM(val));
}



void CPinController::sendPWM(PinName pin,int nVal){
  
  analogWrite(pin,nVal,(int)(pow(2,_resolution)-1.0));
  
}


int CPinController::convertToPWM(float val){   //From 0 to 100.0   
  val=abs(val);
  float pwmMax=pow(2,_resolution)-2.0;
  int nVal=(int)(val*pwmMax/100.0);
  if(nVal<2) nVal=1;


  writeconsole("convertToPWM ");writeconsole(val); writeconsole(" to ");writeconsoleln(nVal);
  return nVal;
}
