#pragma once
#include "defs.h"

#include "systemobject.h"
#include <Adafruit_MLX90640.h>


class CIRArray:public CSystemObject{
private:
  TwoWire *_pWire=&Wire;
  float fmin,fmax;  
  int imin,imax;
  
  Adafruit_MLX90640 mlx;
  
  float frame[32*24]; // buffer for full frame of temperatures
  char pixelTable[770];  //need it null terminated
  char greyTable[770];  //need it null terminated
  char _address=IRARRAY_ADDRESS_X1; 
  

public:  
  CIRArray();
   
  void config(char addr, TwoWire *pwire=&Wire);
  void setup();
  void loop();
  void init();
  void fillPixelTable();
  void fillGreyTable();
  void consoleOut();
  void runOnce(CMsg &msg);
  void Output(CMsg &msg);
  void callCustomFunctions(CMsg &msg);

};

class CIRMaster:public CSystemObject{

public:
  CIRMaster();
  ~CIRMaster();
  void setup();
  void loop();
  void init();
  void runOnce(CMsg &msg){};  
  std::string strData(int z);
  void Output(CMsg &msg);
  void callCustomFunctions(CMsg &msg);
  void callOutput(const char *sysname);

};
