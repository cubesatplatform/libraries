#pragma once

#include <systemobject.h>
#include "SparkFun_BNO080_Arduino_Library.h"
#include "actuator_defs.h" 
#include <string>

/*  Using the BNO080 IMU
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14586

  This example shows how to output the i/j/k/real parts of the rotation vector.
  https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation

  It takes about 1ms at 400kHz I2C to read a record from the sensor, but we are polling the sensor continually
  between updates from the sensor. Use the interrupt pin on the BNO080 breakout to avoid polling.

*/

/// WARNING   IMU Stops sending data after buffer overruns.  Need to keep clearing data!!!    No more than 500ms between data polls or you need to reset and start over
//myIMU.softReset();
//myIMU.beginSPI(PinNameToIndex(PJ_8), PinNameToIndex(PJ_9), PinNameToIndex(PI_15), PinNameToIndex(PI_14));
//myIMU.enableRotationVector(50); //Send data update every 50ms


 //_spiPort->beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));  //RadioLib
 //change this in Spakfun_BNO080_Arduino_Library.cpp
 //boolean BNO080::receivePacket(void)
 //boolean BNO080::sendPacket(uint8_t channelNumber, uint8_t dataLength)

struct CIData{
  float _fX=0.0;
  float _fY=0.0;
  float _fZ=0.0;
  float _fR=0.0;

  float _lastX=0.0;
  float _lastY=0.0;
  float _lastZ=0.0;
  float _lastR=0.0;
  byte _lastAcc=0;

  long _lastTime=0;
  long _changedOn=0;

  float lastAcc=0.0;
  float *plastX=&_lastX;
  float *plastY=&_lastY;
  float *plastZ=&_lastZ;
  float *plastR=&_lastR;
  
  float *pX=&_fX;
  float *pY=&_fY;
  float *pZ=&_fZ;
  float *pR=&_fR;
  byte Acc=0;

  float &X=_fX;
  float &Y=_fY;
  float &Z=_fZ;

  float *pPitch=&_fX;
  float *pRoll=&_fY;
  float *pYaw=&_fZ;

  float *pI=&_fX;
  float *pJ=&_fY;
  float *pK=&_fZ;


  CIData(){}
  void init();

  float get(std::string axis){
    if(axis==_XAXIS) return *pX;
    if(axis==_YAXIS) return *pY;
    if(axis==_ZAXIS) return *pZ;
    //if(axis==RAXIS) return *pR;
    return 0.0;
  }
  
  void archiveData(){
    _lastR=*pR;
    _lastX=*pX;
    _lastY=*pY;
    _lastZ=*pZ;
    lastAcc=Acc;
    _lastTime=_changedOn;
  
  }

  long lastChanged(){return (_changedOn-_lastTime);}

  bool anythingNew(){
    const float hf=.02;   //Bandpass filter
    const float lf=.02;
    if(
      (abs(_lastR)<=abs(_fR)+hf) &&     (abs(_lastX)<=abs(_fX)+hf) &&     (abs(_lastY)<=abs(_fY)+hf) &&     (abs(_lastZ)<=abs(_fZ)+hf)
      && (abs(_lastR)>=abs(_fR)-lf) &&     (abs(_lastX)>=abs(_fX)-lf) &&     (abs(_lastY)>=abs(_fY)-lf) &&     (abs(_lastZ)>=abs(_fZ)-lf)    
    ) 
    
    
    return false;
    return true;
  }
  
  CMsg makeMessage(std::string sname){
    CMsg m;
    std::string param;
    
    
    m.set(_NAME,sname);
    
    param=_RAXIS;m.Parameters[param]=tostring(*pR);
    param=_XAXIS;m.Parameters[param]=tostring(*pX);
    param=_YAXIS;m.Parameters[param]=tostring(*pY);
    param=_ZAXIS;m.Parameters[param]=tostring(*pZ);
    param=_TIME;m.Parameters[param]=tostring(_changedOn);    

    //m.writetoconsole();

    return m;
    };

    
  void deserialize(CMsg &m){////////ERRROROROROR  WRONG     Need to deal with Prefix
    std::string sfirst,ssecond;
    //writeconsoleln("Start Desearialize");
    for (auto x:m.Parameters){
      sfirst=x.first;
      ssecond=x.second;
      float ftmp=std::atof(ssecond.c_str());
      int n=sfirst.size();
      if (n<2)
        continue;
      if (sfirst[n-2]!='_')
        continue;

      if (sfirst[n-1]=='R') *pR=ftmp;
      if (sfirst[n-1]=='X') *pX=ftmp;
      if (sfirst[n-1]=='Y') *pY=ftmp;
      if (sfirst[n-1]=='Z') *pZ=ftmp;
      if (sfirst[n-1]=='T') _changedOn=std::atol(ssecond.c_str());              
    }
  //writeconsoleln("End Desearialize");
  }
    
  


    void remap(float * pnewX, float *pnewY, float *pnewZ);    
};

class CIMU:public CSystemObject{
  private:
    long _dataUpdatedOn=0;
    int _newDataCounter=0;
    char _address;
    TwoWire *_pWire=&Wire;    
  

  public:
    BNO080 myIMU;  //options   q,a,m,g,l,e
    CIData _Data;

    
    CIMU() {init();_m.set(_MODE,_BLANK);}

    void config(char addr, TwoWire *twowire=&Wire);
    bool ShutDown(){  setState(_DONE); return true;    }    
   
    void init();
    void setup();
    void config(CMsg &msg);
    void setupI2C();
    void setupSPI();
    void loop();
    void test(CMsg &msg);
    void runOnce(CMsg &m);
    void goMag();
    void goGyro();
    
    void goPRY();
    void goQuat();
    void goAccel();

    void newMode(CMsg &msg);

    void GetData();
    
    void setPeriod(int period=10);
    CMsg fillData(){
      CMsg m;
      return m;
    };


    void switchPlay(){};      //This needs to be set manually by some other function using the IMU to have it start getting data  Other systems will advance automatically
    long updatedOn(){return _dataUpdatedOn;}
    void callCustomFunctions(CMsg &msg) override;
    
    void remap(){
      _Data.remap(&_Data._fZ,&_Data._fX,&_Data._fY);  
      /*
      _Gyro.remap(&_Gyro._fZ,&_Gyro._fX,&_Gyro._fY);  
      _Mag.remap(&_Mag._fZ,&_Mag._fX,&_Mag._fY);  
      _Lin.remap(&_Lin._fZ,&_Lin._fX,&_Lin._fY);  
      _Accel.remap(&_Accel._fZ,&_Accel._fX,&_Accel._fY);  
      _Quat.remap(&_Quat._fZ,&_Quat._fX,&_Quat._fY);  
      _PRY.remap(&_PRY._fZ,&_PRY._fX,&_PRY._fY);        
      */
    }
  
};    
