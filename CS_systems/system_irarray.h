#pragma once

#include <systemobject.h>
#include <Adafruit_MLX90640.h>
#include <tuple>


#define IR_X 32
#define IR_Y 24


class CIRArray:public CSystemObject{
private:
  TwoWire *_pWire=&Wire;
  float fmin=10000.0,fmax=-10000.0;  
  int imin,imax;
  
  
  Adafruit_MLX90640 mlx;
  
  float frame[IR_X*IR_Y]; // buffer for full frame of temperatures
  float avgframe[IR_X*IR_Y];
  char imageTable[770];  //need it null terminated  
  char _address; 
  

public:  
  CIRArray();
   
  void config(char addr, TwoWire *pwire=&Wire);
  void setup();
  void loop();
  void init();
  void test(CMsg &msg);
  void config(CMsg &msg);
  std::tuple<int, int> getHotSpot();
  float getTemp(int x, int y, int incx, int incy);
  void fillPixel();
  void fillGrey();
  void fillAscii();
  void consoleOut();
  void consoleOutTemp();
  void runOnce(CMsg &msg);
  void Output(CMsg &msg);

  void callCustomFunctions(CMsg &msg);
};

