#pragma once

#include <systemobject.h>
#include <Adafruit_MLX90640.h>


class CIRArray:public CSystemObject{
private:
  TwoWire *_pWire=&Wire;
  float fmin,fmax;  
  int imin,imax;
  
  Adafruit_MLX90640 mlx;
  
  float frame[32*24]; // buffer for full frame of temperatures
  char imageTable[770];  //need it null terminated  
  char _address; 
  

public:  
  CIRArray();
   
  void config(char addr, TwoWire *pwire=&Wire);
  void setup();
  void loop();
  void init();
  void test(CMsg &msg);
  void fillPixel();
  void fillGrey();
  void fillAscii();
  void consoleOut();
  void consoleOutTemp();
  void runOnce(CMsg &msg);
  void Output(CMsg &msg);
};

