#pragma once

#include <systemobject.h>
#include <SparkFun_TMP117.h>

 
#define TEMPMIN -20.0
#define TEMPMAX 40.0

#define _TEMP "TEMP" 
#define _TEMPX1 "TEMPX1" 
#define _TEMPX2 "TEMPX2" 
#define _TEMPY1 "TEMPY1" 
#define _TEMPY2 "TEMPY2" 
#define _TEMPZ1 "TEMPZ1" 
#define _TEMPZ2 "TEMPZ2" 
#define _TEMPOBC "TEMPOBC" 

#define _SENSORS "SENSORS"
#define _HOT "HOT"
#define _COLD "COLD"


class CTemperatureObject:public CSystemObject{
  TwoWire *_pWire=&Wire;
  char _address;
  float _temp=0.0;
  long _ltime=0;
  TMP117 sensor;
public:
   CTemperatureObject();
  ~CTemperatureObject(){};
  void address(char tmp=0x48){_address=tmp;}
  void config(char addr, TwoWire *pWire);
  float getTemp(){return _temp;}
  float getTempTime(){return _ltime;}  
  void setup();
  void loop();
  void init();
  void config(CMsg &msg);  
  void runOnce(CMsg &m);  
};

