#pragma once

#include <msg.h>
#include <funcs.h>

#include <list>
#include <systemobject.h>
#include <SparkFun_BNO080_Arduino_Library.h>
#include <radio.h>

#include <string>

#include <unordered_set>

#include <fhmotor.h>
#include <system_irarray.h>
#include <system_temperature.h>
#include <mdrive.h>


#define MAG_ADDRESS_X  0x60
#define MAG_ADDRESS_Y  0x61
#define MAG_ADDRESS_Z  0x63

 


#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  
   // #include <pinDefinitions.h>
/*
    #define IMU_OBC_NSS PinNameToIndex(PB_4)
    #define IMU_OBC_RST PinNameToIndex(PI_14)
    #define IMU_OBC_INT PinNameToIndex(PI_15)
    #define IMU_OBC_WAKE PinNameToIndex(PB_3)

*/
    #define IMU_OBC_NSS PinNameToIndex(PJ_8)
    #define IMU_OBC_RST PinNameToIndex(PI_14)
    #define IMU_OBC_INT PinNameToIndex(PI_15)
    #define IMU_OBC_WAKE PinNameToIndex(PJ_9)

#else    
    #define Wire2 Wire
    #include <analogWrite.h>   
    typedef int PinName; 
#endif




class CSystemMgr:public CSystemObject{
  
  std::list<CMsg> Scheduler;      
  std::list<CMsg> DoneList;      
  int _channel=0;
  const int freq = 10000;
  const int resolution = 12;

  long _lastPhone=0;
  
public:
 // std::map<std::string, PinName> Pins;
 // std::map<std::string, PinName> pwmPins;

  std::map<std::string, std::string> I2CMap;

  CSystemMgr(); 
  ~CSystemMgr();

  void init();
  void initCommands();
  void setup();
  void loop();

  void chkAll(CMsg &msg);
  void chkIRArrays(CMsg &msg);
  void chkTemps(CMsg &msg);
  void chkRadio(CMsg &msg);
  void chkBattery(CMsg &msg);
  void chkRotation(CMsg &msg);
  void chkMagField(CMsg &msg);
  void chkMags(CMsg &msg);
  void chkMessages(CMsg &msg);
  void chkIMUI(CMsg &msg);
  void chkIMUS(CMsg &msg);

  void randomState();

  void pinHigh(CMsg &msg);
  void pinLow(CMsg &msg);
  void pinPWM(CMsg &msg);
  

  void runSystem(CMsg &msg);

  void pinRead(CMsg &msg);
	void pinReadState(CMsg &msg);
	int pinDigitalReadOutput(PinName pin);

	PinName getPinName(std::string strpin);

  void sendBeacon(CMsg &msg);  
  void sendError(std::string str){};
    
  //void newState(const char *str=_LOWPOWER);
  //void newState(CMsg &msg);  

  void showTests(); 

  void loopWire(char s);
  void loopWire(CMsg &msg);

  void resetWire(char s);
  void resetWire(CMsg &msg);
  void toggleWire(CMsg &msg);
  void enableI2C();
  void disableI2C();
  void phone();
  void chargePhone();
  void pinsOn();
  void pinsOff();
  
  
 // void controlPlan();
  
  void mag(CMsg &msg);
  void motor(CMsg &msg);
  void temp(CMsg &msg);
  void ir(CMsg &msg);
  void IMU(CMsg &msg);

  void testDataMap(CMsg &msg);
  void getData(CMsg &msg);
  void scheduleData(CMsg &msg){};
  void tBeam(CMsg &msg);
  void initI2CMap();
  void sendPicture(CMsg &msg);

  void sendSerial(const char* cmd);
  void callCustomFunctions(CMsg &msg) override;
  
};



 









  
