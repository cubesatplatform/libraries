#pragma once
#include <msg.h>
#include <funcs.h>

#include <list>
#include <systemobject.h>
#include <fhmotor.h>
#include "SparkFun_BNO080_Arduino_Library.h"
#include <radio.h>
#include <arduino.h>

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
    #define TTGO
    typedef int PinName; 
#endif




class CSystemMgr:public CSystemObject{
  
  std::list<CMsg> Scheduler;      
  std::map<std::string, std::list<CMsg>> Commands;      
  std::list<CMsg> DoneList;      
  int _channel=0;
  const int freq = 5000;
  const int resolution = 8;

  unsigned long _lastPhone=0;
  
public:
  CSystemMgr(); 
  ~CSystemMgr();

  void init();
  void initCommands();
  void setup();
  void loop();

  void callCustomFunctions(CMsg &msg);
  
  void powerLevel(CMsg &msg);
  void chkClocks(){writeconsoleln("Checking Clocks & Counters...");};
  void chkRadio(){writeconsoleln("Checking Radio...");};
  void chkCurrentState(){writeconsoleln("Checking Current...");};
  void chkAllStates(){writeconsoleln("Checking All States...");};
  void chkTemperature(){writeconsoleln("Checking Temps...");};
  void chkBattery(){writeconsoleln("Checking Battery...");};
  void chkRotation(){writeconsoleln("Checking Rotation...");};
  void chkCoreSystems(){writeconsoleln("Checking CoreSystems...");};
  void chkMessages(){writeconsoleln("Checking Messages...");};

  
  void sendHealth(){writeconsoleln("Send Health...");};
  void sendBeacon(){writeconsoleln("Send Beacon...");};
  void sendError(std::string str){};
  
  
  
  void newState(const char *str="LOWPOWER");
  
  void addSchedule(CMsg &msg);
  void showScheduler();
  void showCommands();
  void showTests();
  
  void SendCmd(std::string str);
  void SendCmdToScheduler(std::string str);
  void loopWire(TwoWire *wire);
  void enableI2C();
  void burn();
  void phone();
  void pinsOn();
  void pinsOff();
  void setupIMUSPI();
  void setupIMUI2C(TwoWire *wire);
  
  void initPins();
  void controlPlan();
  void testMAG();
  void Output(CMsg &msg);
  void sendSerial(const char* cmd);
};













  
