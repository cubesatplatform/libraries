#pragma once

#include <systemobject.h>
#include <Adafruit_MLX90640.h>
#include <tuple>


#define IR_X 32
#define IR_Y 24

#define IRARRAYSIZE 768

#define _IRX1 "IRX1"  
#define _IRX2 "IRX2"  
#define _IRY1 "IRY1"  
#define _IRY2 "IRY2"  
#define _IRZ1 "IRZ1"  
#define _IRZ2 "IRZ2"  

#define _CONSOLE "CONSOLE"

#define _IR_MIN "IR_MIN"
#define _IR_MAX "IR_MAX"

#define _PIXELX "PIXELX"
#define _PIXELY "PIXELY"
#define _PIXELXY "PIXELXY"
#define _FILLTEMP "FILLTEMP"
#define _FILLPIXEL "FILLPIXEL"
#define _FILLGREY "FILLGREY"
#define _FILLASCII "FILLASCII"




class CIRArray:public CSystemObject{
private:
  TwoWire *_pWire=&Wire;
  float fmin=10000.0,fmax=-10000.0;  
  int imin,imax;  
  
  Adafruit_MLX90640 mlx;
  
  float frame[32*24]; // buffer for full frame of temperatures
  
  char _address; 
  long _transmitInterval=60000;
  long _lastTransmit=0;
  

public:  
  CIRArray();
   
  void config(char addr, TwoWire *pwire=&Wire);
  void setup();
  void loop();
  void init();  
  void config(CMsg &msg);
  std::tuple<int, int> getHotSpot();
  float getTemp(int x, int y, int incx, int incy);
  std::pair<double, double> getHotSpotDFS();
  void fillTemp(unsigned char *imageTable);   //These were are regular char before   Trying to do base64
  void fillPixel(unsigned char *imageTable);
  void fillGrey(unsigned char *imageTable);
  void fillAscii(unsigned char *imageTable);  
  void consoleOut(unsigned char *imageTable);
  void runOnce(CMsg &msg);
  void output(CMsg &msg);
  CMsg variance(float a[], int n);

  void callCustomFunctions(CMsg &msg);
};

