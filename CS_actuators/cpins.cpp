#include "cpins.h"

#define _BLINK "BLINK"
#define _MODE_PWM "MODE_PWM"

//TO:BS291~SYS:PINCONTROLLER~ACT:CONFIG~V:.4~PIN:24
#define PINCONTROLLER "PINCONTROLLER"


CPins::CPins(){  ///Never init the static stuff here  do later  otherwise crashes

  setInterval(1000);

  }

CPins::~CPins(){
  for(auto it=_pins.begin();it!=_pins.end();it++){
      it->second->pct(0.0);      
    }
}


CPin *CPins::find(std::string name){
auto it = _pins.find(name);
if (it != _pins.end()) {  // The element was found
  return it->second;  
} 

  return NULL;
}



void CPins::addpin(CMsg &msg){
  
  std::string pin=msg.get(_PIN);
  float val=msg.get(_VALUE,100.0);
  CPin *ptr=find(pin);
  if (ptr==NULL){
    ptr=new CPin();
    _pins[pin]=ptr;
    ptr->config(pin);
    ptr->pct(val);

  }
}


void CPins::deletepin(CMsg &msg){
  std::string pin=msg.get(_PIN);
  
  CPin *ptr=find(pin);
  if (ptr!=NULL){    
    ptr->pct(0.0);
    delete ptr;
    _pins[pin]=NULL;
  }
}


void CPins::changepin(CMsg &msg){
  std::string pin=msg.get(_PIN);
  float val=msg.get(_VALUE,0.0);
  
  CPin *ptr=find(pin);
  if (ptr!=NULL){    
    ptr->pct(val);
  }
}



void CPins::loop(){  
  for(auto it=_pins.begin();it!=_pins.end();it++){  
    it->second->print();
  }
   
}  



void CPins::callCustomFunctions(CMsg &msg) {
  writeconsoleln("CPinController::callCustomFunctions :  ");
  
  std::string act=msg.get(_ACT); 
  float val=msg.get(_VALUE,0.0); 

  
  mapcustommsg(addpin)
  mapcustommsg(deletepin)
  mapcustommsg(changepin)
  
}
