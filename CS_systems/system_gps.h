#pragma once


#if defined(TTGO) || defined(TTGO1)
#include "systemobject.h"
#include <string>
#include <TinyGPS++.h>
#include <axp20x.h>
#include "boards.h"

class CGPS :public CSystemObject {
 // TinyGPSPlus gps;
  TinyGPSPlus gps;
  AXP20X_Class axp;

  float latitude=0.0;
  float longitude=0.0;
  float altitude=0.0;
  char SIV=' ';
  int Yr=0;
  int Mon=0;
  int Day=0;
  int Hr=0;
  int Min=0;
  int Sec=0;
  long Speed=0;
public:

  CGPS();
  ~CGPS() {
    writeconsole("GPS destroyed");
  }
  void setup();
  void loop();
  void init();
  void output();
  void runOnce(CMsg &msg);
  void smartDelay(unsigned long ms);
  CMsg fillData();
 
};

#endif
