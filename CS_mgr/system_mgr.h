#pragma once

#include <msg.h>
#include <funcs.h>

#include <list>
#include <systemobject.h>
#include <fhmotor.h>
#include <system_irarray.h>
#include <system_temperature.h>
#include <SparkFun_BNO080_Arduino_Library.h>
#include <radio.h>

#include <string>
#include <mdrive.h>
#include <unordered_set>
  


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
    typedef int PinName; 
#endif




class CSystemMgr:public CSystemObject{
  
  std::list<CMsg> Scheduler;      
  std::map<std::string, std::list<CMsg>> Commands;      
  std::list<CMsg> DoneList;      
  int _channel=0;
  const int freq = 10000;
  const int resolution = 12;

  unsigned long _lastPhone=0;
  
public:
  CSystemMgr(); 
  ~CSystemMgr();

  void init();
  void initCommands();
  void setup();
  void loop();

  void callCustomFunctions(CMsg &msg);
  
  void chkTemperature(CMsg &msg);
  void chkRadio(CMsg &msg);
  void chkBattery(CMsg &msg);
  void chkRotation(CMsg &msg);
  void chkMagField(CMsg &msg);
  void chkMessages(CMsg &msg);

  void sendBeacon(CMsg &msg);  
  void sendError(std::string str){};
    
  void newState(const char *str="LOWPOWER");
  
  void addSchedule(CMsg &msg);
  void deleteSchedule(CMsg &msg);
  void showScheduler(CMsg &msg);
  void showCommands();
  void showTests();
  
  void SendCmd(CMsg &msg);
  void SendCmdToScheduler(CMsg &msg);
  void loopWire(TwoWire *wire, const char* s);
  void resetWire(TwoWire *wire, const char* s);
  void enableI2C();
  void disableI2C();
  void burn();
  void phone();
  void pinsOn();
  void pinsOff();
  
  
  void initPins();
  void initI2CMap();
  void controlPlan();
  

  void testIMU(CMsg &msg);
  void testMAGDrive(char addr);
  void testMAG(CMsg &msg);
  void testMotor(CMsg &msg);
  void testTemp(CMsg &msg);
  void testIR(CMsg &msg);
  
  void Output(CMsg &msg);
  void sendSerial(const char* cmd);
};













  
