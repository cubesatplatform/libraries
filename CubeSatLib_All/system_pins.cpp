#include "system_pins.h"
#include <Arduino.h>

void CSatPins::loop() {
}


int CSatPins::digitalReadOutputPin(PinName pin)
{
  #if defined(TTGO1) || defined(TTGO)
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  if (port == NOT_A_PIN) 
    return LOW;

  return (*portOutputRegister(port) & bit) ? HIGH : LOW;
  #else
  return HIGH;
  #endif

}

void CSatPins::high(CMsg &msg){
	writeconsoleln("High");

	std::string strpin=msg.getParameter("PIN","");
	PinName n=Pins[strpin];
   #if defined(TTGO1) || defined(TTGO)
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

void CSatPins::low(CMsg &msg){
	writeconsoleln("LOW");

	std::string strpin=msg.getParameter("PIN","");
	PinName n=Pins[strpin];
 // pinMode(n, OUTPUT); ///Set is to output mode   
	delay(10);
	digitalWrite(n, LOW);
  writeconsole("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX LOW ");
  writeconsoleln(n);
	return;
  }

  
void CSatPins::read(CMsg &msg){
	writeconsoleln("READ");

	std::string strpin=msg.getParameter("PIN","");
	PinName n=Pins[strpin];
  pinMode(n, INPUT); ///Set is to output mode   
	delay(10);
	int val=digitalRead(n);
  writeconsole("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX READ ");
  writeconsoleln(val);

  msg.setTABLE("PINSTATE");
  msg.setParameter("VALUE",val);
  addTransmitList(msg);
	return;
  }

  void CSatPins::readState(CMsg &msg){
	writeconsoleln("READ");

	std::string strpin=msg.getParameter("PIN","");
	PinName n=Pins[strpin];  
	delay(10);
	//int val=digitalReadOutputPin(n);
  int val=digitalRead(n);
  writeconsole("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX READSTATE ");
  writeconsoleln(val);

  msg.setTABLE("PINSTATE");
  msg.setParameter("VALUE",val);
  addTransmitList(msg);
	return;
  }



void CSatPins::pwm(CMsg &msg){
	
	std::string strpin=msg.getParameter("PIN","");
	PinName n=Pins[strpin];

	int freq =msg.getParameter("FREQUENCY",10000);
	int resolution =msg.getParameter("RESOLUTION",12); 
  int _channel=0;

 
  #if defined(TTGO1) || defined(TTGO) 
    ledcSetup(_channel, freq, resolution);   // configure LED PWM functionalitites
    ledcAttachPin(n, _channel);  // attach the channel to the GPIO to be controlled
    delay(50);
    ledcWrite(_channel, n); 

  #else
    analogWriteResolution(12);   
    for (int count=0;count<4000;count+=100){
      analogWrite(n,count);
      writeconsole("PWM: ");
      writeconsole(count);
      delay(200);
    }
  #endif


  return;    
}



void  CSatPins::callCustomFunctions(CMsg &msg){
  std::string act=msg.getACT();
  
  writeconsoleln(act);
  
  if(act=="READ") read(msg);
  if(act=="READSTATE") readState(msg);
  if(act=="HIGH") high(msg);
  if(act=="LOW") low(msg);
  if(act=="PWM") pwm(msg);
}
