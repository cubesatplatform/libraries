#pragma once

#include <systemobject.h>
#include <SparkFun_TMP117.h>


class CTemperatureObject:public CSystemObject{
  TwoWire *_pWire;
  char _address;
  float _temp=0.0;
  long _time=0;
  TMP117 sensor;
public:
   CTemperatureObject(){ Name("TEMP");init();};
  ~CTemperatureObject(){};
  void address(char tmp=0x48){_address=tmp;}
  void config(char addr, TwoWire *pWire);
  float getTemp(){return _temp;}
  CMsg getTempMSG();
  void setup();
  void loop();
  void init();
  void test();
  void runOnce(CMsg &m);
};

