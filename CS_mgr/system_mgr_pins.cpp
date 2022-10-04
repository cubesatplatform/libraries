#include "system_mgr.h"


#define PIN_RESOLUTION 12
#define PIN_FREQUENCY 10000

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
#else
  #include <analogWrite.h>   
#endif


PinName CSystemMgr::getPinName(std::string strpin){
  PinName n;
  CSystemMgr *pMgr=(CSystemMgr *)getSystem("MGR");

  if(pMgr!=NULL){
    n=pMgr->Pins[strpin];
  }
  
  return n;
}

int CSystemMgr::pinDigitalReadOutput(PinName pin)
{
  #if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  if (port == NOT_A_PIN) 
    return LOW;

  return (*portOutputRegister(port) & bit) ? HIGH : LOW;
  #else
  return HIGH;
  #endif

}





void CSystemMgr::pinPWM(CMsg &msg) {
	
	std::string strpin=msg.getParameter("PIN","");
	PinName n=getPinName(strpin);

	int freq =msg.getParameter("FREQUENCY",PIN_FREQUENCY);
	int resolution =msg.getParameter("RESOLUTION",PIN_RESOLUTION); 
  int _channel=0; 

  #if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
   analogWriteResolution(n,PIN_RESOLUTION);   
  #endif

  for (int count=0;count<4000;count+=100){
    analogWrite(n,count);
    writeconsole("PWM: ");
    writeconsole(count);
    delay(200);
  }


  return;    
}






void CSystemMgr::pinHigh(CMsg &msg){
	writeconsoleln("High");

	std::string strpin=msg.getParameter("PIN","");
	PinName n=getPinName(strpin);
  #if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
    pinMode(n, GPIO_MODE_INPUT_OUTPUT); ///Set is to output mode   
  #else
    pinMode(n, GPIO_MODE_INPUT);
  #endif
	delay(10);
	digitalWrite(n, HIGH);
  writeconsole("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX HIGH ");
  writeconsoleln(n);
	return;
  }

void CSystemMgr::pinLow(CMsg &msg){
	writeconsoleln("LOW");

	std::string strpin=msg.getParameter("PIN","");
	PinName n=getPinName(strpin);
 // pinMode(n, OUTPUT); ///Set is to output mode   
	delay(10);
	digitalWrite(n, LOW);
  writeconsole("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX LOW ");
  writeconsoleln(n);
	return;
  }

  
void CSystemMgr::pinRead(CMsg &msg){
	writeconsoleln("READ");

	std::string strpin=msg.getParameter("PIN","");
	PinName n=getPinName(strpin);
  pinMode(n, INPUT); ///Set is to output mode   
	delay(10);
	int val=digitalRead(n);
  writeconsole("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX READ ");
  writeconsoleln(val);


  msg.setParameter(strpin,val);
  addTransmitList(msg);
	return;
  }

  void CSystemMgr::pinReadState(CMsg &msg){
	writeconsoleln("READ");

	std::string strpin=msg.getParameter("PIN","");
	PinName n=getPinName(strpin);  
	delay(10);
	//int val=digitalReadOutputPin(n);
  int val=digitalRead(n);
  writeconsole("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX READSTATE ");
  writeconsoleln(val);

  msg.setParameter(strpin,val);
  addTransmitList(msg);
	return;
  }



