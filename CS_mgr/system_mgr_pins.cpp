#include "system_mgr.h"


#define PIN_RESOLUTION 12
#define PIN_FREQUENCY 10000

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
#else
  #include <analogWrite.h>   
  #include <axp20x.h>

  AXP20X_Class axp;
#endif



#if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
void CSystemMgr::initPins() {
  Pins["LED"] = LED_BUILTIN;
  Pins["36"] = 36;
  Pins["39"] = 39;
  Pins["34"] = 34;
  Pins["35"] = 35;
  Pins["32"] = 32;
  Pins["33"] = 33;
  Pins["25"] = 25;
  Pins["14"] = 14;
  Pins["13"] = 13;
  Pins["2"] = 2;
  Pins["1"] = 1;
  Pins["3"] = 3;
  Pins["23"] = 23;
  Pins["4"] = 4;
  Pins["0"] = 0;
  Pins["22"] = 22;
  Pins["21"] = 21;
  pwmPins["36"]=36;
  pwmPins["39"]=39;
  pwmPins["34"]=34;
  pwmPins["35"]=35;
  pwmPins["32"]=32;
  pwmPins["33"]=33;
  pwmPins["25"]=25;
  pwmPins["14"]=14;
  pwmPins["13"]=13;
  pwmPins["2"]=2;
  pwmPins["1"]=1;
  pwmPins["3"]=3;
  pwmPins["23"]=23;
  pwmPins["4"]=4;
  pwmPins["0"]=0;
  pwmPins["22"]=22;
  pwmPins["21"]=21;
}


void CSystemMgr::initI2CMap(){
  I2CMap["20_51"]="2> IR X1 - 0x33/51";
  I2CMap["0_50"]="2> IR X2 - 0x32/50";
  I2CMap["1_51"]="2> IR Y1 - 0x33/51";
  I2CMap["1_50"]="2> IR Y2 - 0x32/50";
  I2CMap["2_51"]="2> IR Z1 - 0x33/51";
  I2CMap["2_50"]="2> IR Z2 - 0x32/50";
  
  I2CMap["0_74"]="0>Temp_OBC - 0x4A/74";
  I2CMap["0_75"]="0>IMU_ADCS - 0x4B/75";

  I2CMap["0_73"]="2> Temp X1 - 0x49/73";
  I2CMap["0_72"]="2> Temp X - 0x48/72";
  I2CMap["1_73"]="2> Temp Y1 - 0x49/73";
  I2CMap["1_72"]="2> Temp Y - 0x48/72";
  I2CMap["2_73"]="2> Temp Z1 - 0x49/73";
  I2CMap["2_72"]="2> Temp Z - 0x48/72";

  I2CMap["1_96"]="1> 96 ??";  
  I2CMap["1_54"]="1> 54 ??";
  I2CMap["1_8"]=" 1> 8 ??";
  
  I2CMap["2_99"]="2> Mag Z - 0x63/99";
  I2CMap["2_97"]="2> Mag Y - 0x61/97";
  I2CMap["2_96"]="2> Mag X - 0x60/96";
  

};

#else

void CSystemMgr::initI2CMap(){
  I2CMap["0_51"]="2> IR X1 - 0x33/51";
  I2CMap["0_50"]="2> IR X2 - 0x32/50";
  I2CMap["1_51"]="2> IR Y1 - 0x33/51";
  I2CMap["1_50"]="2> IR Y2 - 0x32/50";
  I2CMap["2_51"]="2> IR Z1 - 0x33/51";
  I2CMap["2_50"]="2> IR Z2 - 0x32/50";
  
  I2CMap["0_74"]="0>Temp_OBC - 0x4A/74";
  I2CMap["0_75"]="0>IMU_ADCS - 0x4B/75";

  I2CMap["0_73"]="2> Temp X1 - 0x49/73";
  I2CMap["0_72"]="2> Temp X - 0x48/72";
  I2CMap["1_73"]="2> Temp Y1 - 0x49/73";
  I2CMap["1_72"]="2> Temp Y - 0x48/72";
  I2CMap["2_73"]="2> Temp Z1 - 0x49/73";
  I2CMap["2_72"]="2> Temp Z - 0x48/72";

  I2CMap["1_96"]="1> 96 ??";  
  I2CMap["1_54"]="1> 54 ??";
  I2CMap["1_8"]=" 1> 8 ??";
  
  I2CMap["2_99"]="2> Mag Z - 0x63/99";
  I2CMap["2_97"]="2> Mag Y - 0x61/97";
  I2CMap["2_96"]="2> Mag X - 0x60/96";
  

};

void CSystemMgr::initPins() {
  Pins["LED"] = LED_BUILTIN;
  Pins["PA_0"]=PA_0;
  //Pins["PA_1"]=PA_1;
  Pins["PA_10"]=PA_10;
  Pins["PA_11"]=PA_11;
  Pins["PA_12"]=PA_12;
  Pins["PA_13"]=PA_13;
  Pins["PA_14"]=PA_14;
  Pins["PA_15"]=PA_15;
  Pins["PA_2"]=PA_2;
//  Pins["PA_3"]=PA_3;
  Pins["PA_4"]=PA_4;
//  Pins["PA_5"]=PA_5;
  Pins["PA_6"]=PA_6;
  //Pins["PA_7"]=PA_7;
  Pins["PA_8"]=PA_8;
  Pins["PA_9"]=PA_9;
//  Pins["PB_0"]=PB_0;
 // Pins["PB_1"]=PB_1;
//  Pins["PB_10"]=PB_10;
  //Pins["PB_11"]=PB_11;
  //Pins["PB_12"]=PB_12;
  //Pins["PB_13"]=PB_13;
  Pins["PB_14"]=PB_14;
  Pins["PB_15"]=PB_15;
  Pins["PB_2"]=PB_2;
  Pins["PB_3"]=PB_3;
  Pins["PB_4"]=PB_4;
  //Pins["PB_5"]=PB_5;
  Pins["PB_6"]=PB_6;
  Pins["PB_7"]=PB_7;
  Pins["PB_8"]=PB_8;
  Pins["PB_9"]=PB_9;
  //Pins["PC_0"]=PC_0;
  //Pins["PC_1"]=PC_1;
  Pins["PC_10"]=PC_10;
  Pins["PC_11"]=PC_11;
  Pins["PC_12"]=PC_12;
  Pins["PC_13"]=PC_13;
  Pins["PC_14"]=PC_14;
  Pins["PC_15"]=PC_15;
  Pins["PC_2"]=PC_2;
  Pins["PC_3"]=PC_3;
  //Pins["PC_4"]=PC_4;
  //Pins["PC_5"]=PC_5;
  Pins["PC_6"]=PC_6;
  Pins["PC_7"]=PC_7;
  Pins["PC_8"]=PC_8;
  Pins["PC_9"]=PC_9;
  Pins["PD_0"]=PD_0;
  Pins["PD_1"]=PD_1;
  Pins["PD_10"]=PD_10;
  Pins["PD_11"]=PD_11;
  Pins["PD_12"]=PD_12;
  Pins["PD_13"]=PD_13;
  Pins["PD_14"]=PD_14;
  Pins["PD_15"]=PD_15;
  Pins["PD_2"]=PD_2;
  Pins["PD_3"]=PD_3;
  Pins["PD_4"]=PD_4;
  Pins["PD_5"]=PD_5;
  Pins["PD_6"]=PD_6;
  Pins["PD_7"]=PD_7;
  Pins["PD_8"]=PD_8;
  Pins["PD_9"]=PD_9;
  Pins["PE_0"]=PE_0;
  Pins["PE_1"]=PE_1;
  Pins["PE_10"]=PE_10;
  Pins["PE_11"]=PE_11;
  Pins["PE_12"]=PE_12;
  Pins["PE_13"]=PE_13;
  Pins["PE_14"]=PE_14;
  Pins["PE_15"]=PE_15;
  Pins["PE_2"]=PE_2;
  Pins["PE_3"]=PE_3;
  Pins["PE_4"]=PE_4;
  Pins["PE_5"]=PE_5;
  Pins["PE_6"]=PE_6;
  Pins["PE_7"]=PE_7;
  Pins["PE_8"]=PE_8;
  Pins["PE_9"]=PE_9;
  Pins["PF_0"]=PF_0;
  Pins["PF_1"]=PF_1;
  Pins["PF_10"]=PF_10;
  Pins["PF_11"]=PF_11;
  Pins["PF_12"]=PF_12;
  Pins["PF_13"]=PF_13;
  Pins["PF_14"]=PF_14;
  Pins["PF_15"]=PF_15;
  Pins["PF_2"]=PF_2;
  Pins["PF_3"]=PF_3;
  Pins["PF_4"]=PF_4;
  Pins["PF_5"]=PF_5;
  Pins["PF_6"]=PF_6;
  Pins["PF_7"]=PF_7;
  Pins["PF_8"]=PF_8;
  Pins["PF_9"]=PF_9;
  Pins["PG_0"]=PG_0;
  Pins["PG_1"]=PG_1;
  Pins["PG_10"]=PG_10;
  //Pins["PG_11"]=PG_11;
  //Pins["PG_12"]=PG_12;
  //Pins["PG_13"]=PG_13;
  
  Pins["PG_14"]=PG_14;
  Pins["PG_15"]=PG_15;
  Pins["PG_2"]=PG_2;
  Pins["PG_3"]=PG_3;
  Pins["PG_4"]=PG_4;
  Pins["PG_5"]=PG_5;
  Pins["PG_6"]=PG_6;
  Pins["PG_7"]=PG_7;
  Pins["PG_8"]=PG_8;
  Pins["PG_9"]=PG_9;
  Pins["PH_0"]=PH_0;
//  Pins["PH_1"]=PH_1;  ??Can go High not Low
  Pins["PH_10"]=PH_10;
  Pins["PH_11"]=PH_11;
  Pins["PH_12"]=PH_12;
  Pins["PH_13"]=PH_13;
  Pins["PH_14"]=PH_14;
  Pins["PH_15"]=PH_15;
  Pins["PH_2"]=PH_2;
  Pins["PH_3"]=PH_3;
//  Pins["PH_4"]=PH_4;
  Pins["PH_5"]=PH_5;
  Pins["PH_6"]=PH_6;

  
  Pins["PH_7"]=PH_7;
  Pins["PH_8"]=PH_8;
  Pins["PH_9"]=PH_9;
  
  Pins["PI_0"]=PI_0;
  Pins["PI_1"]=PI_1;
//  Pins["PI_10"]=PI_10;
//  Pins["PI_11"]=PI_11;
  Pins["PI_12"]=PI_12;
  Pins["PI_13"]=PI_13;
  Pins["PI_14"]=PI_14;

  
  Pins["PI_15"]=PI_15;
  Pins["PI_2"]=PI_2;
  Pins["PI_3"]=PI_3;
  Pins["PI_4"]=PI_4;
  Pins["PI_5"]=PI_5;
  Pins["PI_6"]=PI_6;
  Pins["PI_7"]=PI_7;
  Pins["PI_8"]=PI_8;
  Pins["PI_9"]=PI_9;
  Pins["PJ_0"]=PJ_0;
  Pins["PJ_1"]=PJ_1;
  Pins["PJ_10"]=PJ_10;
  Pins["PJ_11"]=PJ_11;
  Pins["PJ_12"]=PJ_12;
  Pins["PJ_13"]=PJ_13;
  Pins["PJ_14"]=PJ_14;
  //Pins["PJ_15"]=PJ_15;
  Pins["PJ_2"]=PJ_2;
  Pins["PJ_3"]=PJ_3;
//  Pins["PJ_4"]=PJ_4;
  Pins["PJ_5"]=PJ_5;
  Pins["PJ_6"]=PJ_6;
  Pins["PJ_7"]=PJ_7;
  Pins["PJ_8"]=PJ_8;
  Pins["PJ_9"]=PJ_9;
  Pins["PK_0"]=PK_0;
  Pins["PK_1"]=PK_1;
  Pins["PK_2"]=PK_2;
  Pins["PK_3"]=PK_3;
  Pins["PK_4"]=PK_4;
  Pins["PK_5"]=PK_5;
  Pins["PK_6"]=PK_6;
  Pins["PK_7"]=PK_7;
  
  pwmPins["PH_15"]=PH_15;
  pwmPins["PK_1"]=PK_1;
  pwmPins["PJ_11"]=PJ_11;
  pwmPins["PG_7"]=PG_7;
  pwmPins["PC_7"]=PC_7;
  pwmPins["PC_6"]=PC_6;
  pwmPins["PA_8"]=PA_8;
}

#endif



void CSystemMgr::pinsOn(){
  CMsg m;
  m.setSYS("Pins ON");
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
  m.setSYS("Pins OFF");
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
	writeconsole("High : ");

	std::string strpin=msg.getParameter("PIN","0");
	PinName n=getPinName(strpin);
  
  #if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
    writeconsole("High : ");writeconsole(strpin);writeconsole(n);
    //pinMode(n, GPIO_MODE_INPUT_OUTPUT); ///Set is to output mode   
    pinMode(n, OUTPUT); ///Set is to output mode
    
  #else
    pinMode(n, GPIO_MODE_OUTPUT);
  #endif
	delay(100);
	digitalWrite(n, HIGH);  

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




void CSystemMgr::tBeam(CMsg &msg){
	
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  #else
  #define AXP192_SLAVE_ADDRESS 0x34
  Wire.begin(I2C_SDA, I2C_SCL);
  writeconsole("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX T Beam ");
  
  if (!axp.begin(Wire, AXP192_SLAVE_ADDRESS)) {
    std::string tMode=msg.getParameter("MODE","");
    writeconsoleln(tMode);
    // power on device
    if (tMode=="ON"){
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

    if (tMode=="OFF"){
      axp.setPowerOutPut(AXP192_LDO2, AXP202_OFF);
      axp.setPowerOutPut(AXP192_LDO3, AXP202_OFF);
      axp.setPowerOutPut(AXP192_DCDC1, AXP202_OFF);
      axp.setPowerOutPut(AXP192_DCDC2, AXP202_OFF);
      axp.setPowerOutPut(AXP192_DCDC3, AXP202_OFF);
      axp.setPowerOutPut(AXP192_EXTEN, AXP202_OFF);
    }
    
    
    // Set mode of blue onboard LED (OFF, ON, Blinking 1Hz, Blinking 4 Hz)
    if (tMode=="LEDOFF") axp.setChgLEDMode(AXP20X_LED_OFF);
    if (tMode=="LEDON")axp.setChgLEDMode(AXP20X_LED_LOW_LEVEL);
    if (tMode=="LEDBLINK") axp.setChgLEDMode(AXP20X_LED_BLINK_1HZ);
    if (tMode=="LEDBLINKFAST") axp.setChgLEDMode(AXP20X_LED_BLINK_4HZ);
}
    
  #endif

  }


