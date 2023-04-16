#include "system_mgr.h"


#define PIN_RESOLUTION 12
#define PIN_FREQUENCY 10000

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
#else
  #include <analogWrite.h>   
  #include <axp20x.h>

  AXP20X_Class axp;
#endif







void CSystemMgr::pinsOn(){
  CMsg m;
  m.set(_NAME, _PINSON);
  for (auto x:Pins){
    writeconsoleln(x.first.c_str());     
    //pinMode(x.second, OUTPUT); ///Set is to output mode  if you need lots of current
    digitalWrite(x.second, HIGH);    
    delay(250);
  }
  writeconsoleln(m.serializeout());  
}

void CSystemMgr::pinsOff(){
  CMsg m;
  m.set(_NAME,_PINSOFF);
  for (auto x:Pins){
    writeconsoleln(x.first.c_str());
    //pinMode(x.second, OUTPUT); ///Set is to output mode  if you need lots of current
    digitalWrite(x.second, LOW);    
    delay(250);
  }  
  writeconsoleln(m.serializeout());  
}


PinName CSystemMgr::getPinName(std::string strpin){
  PinName n;


  CSystemMgr *pMgr=(CSystemMgr *)getSystem(_MANAGER);  //Try not to use get system

  if(pMgr!=NULL){
    n=pMgr->Pins[strpin];
  }
  
  return n;
}

int CSystemMgr::pinDigitalReadOutput(PinName pin)
{
  #if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
  int bit = digitalPinToBitMask(pin);
  int port = digitalPinToPort(pin);
  if (port == NOT_A_PIN) 
    return LOW;

  return (*portOutputRegister(port) & bit) ? HIGH : LOW;
  #else
  return HIGH;
  #endif

}





void CSystemMgr::pinPWM(CMsg &msg) {
	
	std::string strpin=msg.get(_PIN,_BLANK);
	PinName n=getPinName(strpin);

	CPin pin;
  pin.config(n);

  for (int val=0;val<=100;val++){
    pin.pct((float)val);
    delay(100);
  }


  return;    
}






void CSystemMgr::pinHigh(CMsg &msg){
	writeconsole("High : ");

	std::string strpin=msg.get(_PIN,_BLANK);
  if(strpin==_BLANK) return;
	PinName n=getPinName(strpin);
  

  pinMode(n, OUTPUT);
	delay(100);
	digitalWrite(n, HIGH);  

	return;
  }

void CSystemMgr::pinLow(CMsg &msg){
	writeconsoleln("LOW");

	std::string strpin=msg.get(_PIN,_BLANK);
  if(strpin==_BLANK) return;
	PinName n=getPinName(strpin);
 // pinMode(n, OUTPUT); ///Set is to output mode   
	delay(10);
	digitalWrite(n, LOW);
  writeconsole(" LOW ");
  writeconsoleln(n);
	return;
  }

  
void CSystemMgr::pinRead(CMsg &msg){
	writeconsoleln("READ");

	std::string strpin=msg.get(_PIN,_BLANK);
  if(strpin==_BLANK) return;
	PinName n=getPinName(strpin);
  pinMode(n, INPUT); ///Set is to output mode   
	delay(10);
	int val=digitalRead(n);
  writeconsole(" READ ");
  writeconsoleln(val);


  msg.set(strpin,val);
  addTransmitList(msg);
	return;
  }

  void CSystemMgr::pinReadState(CMsg &msg){
	writeconsoleln("READ");

	std::string strpin=msg.get(_PIN,_BLANK);
  if(strpin==_BLANK) return;
	PinName n=getPinName(strpin);  
	delay(10);
	//int val=digitalReadOutputPin(n);
  int val=digitalRead(n);
  writeconsole(" READSTATE ");
  writeconsoleln(val);

  msg.set(strpin,val);
  addTransmitList(msg);
	return;
  }




void CSystemMgr::tBeam(CMsg &msg){
	
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  #else
  //#define AXP192_SLAVE_ADDRESS 0x34
  Wire.begin(I2C_SDA, I2C_SCL);
  writeconsole(" T Beam ");
  
  if (!axp.begin(Wire, AXP192_SLAVE_ADDRESS)) {
    std::string tMode=msg.get(_MODE,_BLANK);
    writeconsoleln(tMode);
    // power on device
    if (tMode==_MODEON){
      // this one seems to be enough to make the led work.
      //axp.setDCDC1Voltage(3300); //Set Pin header 3.3V line to 3.3V. Processor is happy down to 1.8V which saves power  
      axp.setLDO2Voltage(3300);   //LoRa VDD
      axp.setLDO3Voltage(3300);   //GPS  VDD
      axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
      axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
      axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
      axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
      axp.setPowerOutPut(AXP192_DCDC3, AXP202_ON);
      axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
    }

    if (tMode==_MODEOFF){
      axp.setPowerOutPut(AXP192_LDO2, AXP202_OFF);
      axp.setPowerOutPut(AXP192_LDO3, AXP202_OFF);
      axp.setPowerOutPut(AXP192_DCDC1, AXP202_OFF);
      axp.setPowerOutPut(AXP192_DCDC2, AXP202_OFF);
      axp.setPowerOutPut(AXP192_DCDC3, AXP202_OFF);
      axp.setPowerOutPut(AXP192_EXTEN, AXP202_OFF);
    }
    
    
    // Set mode of blue onboard LED (OFF, ON, Blinking 1Hz, Blinking 4 Hz)
    if (tMode==_MODELEDOFF) axp.setChgLEDMode(AXP20X_LED_OFF);
    if (tMode==_MODELEDON)axp.setChgLEDMode(AXP20X_LED_LOW_LEVEL);
    if (tMode==_MODELEDBLINK) axp.setChgLEDMode(AXP20X_LED_BLINK_1HZ);
    if (tMode==_MODELEDBLINKFAST) axp.setChgLEDMode(AXP20X_LED_BLINK_4HZ);
}
    
  #endif

  }


