#pragma once

#include <systemobject.h>
#include <SparkFun_TMP117.h>


class CTemperatureObject:public CSystemObject{
  TwoWire *_pWire;
  char _address;
  float _temp=0.0;
  unsigned long _ltime=0;
  TMP117 sensor;
public:
   CTemperatureObject();
  ~CTemperatureObject(){};
  void address(char tmp=0x48){_address=tmp;}
  void config(char addr, TwoWire *pWire);
  float getTemp(){return _temp;}
  float getTempTime(){return _ltime;}
  CMsg getTempMSG();
  void setup();
  void loop();
  void init();
  void config(CMsg &msg);
  void test(CMsg &msg);
  void runOnce(CMsg &m);
};

