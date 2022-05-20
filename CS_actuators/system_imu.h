#pragma once

#include <systemobject.h>
#include "SparkFun_BNO080_Arduino_Library.h"
#include <SimpleKalmanFilter.h>
#include <string>

/*  Using the BNO080 IMU
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14586

  This example shows how to output the i/j/k/real parts of the rotation vector.
  https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation

  It takes about 1ms at 400kHz I2C to read a record from the sensor, but we are polling the sensor continually
  between updates from the sensor. Use the interrupt pin on the BNO080 breakout to avoid polling.

*/



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

  unsigned long _lastTime=0;
  unsigned long _changedOn=0;

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

  SimpleKalmanFilter kfR;
  SimpleKalmanFilter kfX;
  SimpleKalmanFilter kfY;
  SimpleKalmanFilter kfZ;
  CIData(){}
  void init();
  
  void archiveData(){
    _lastR=*pR;
    _lastX=*pX;
    _lastY=*pY;
    _lastZ=*pZ;
    lastAcc=Acc;
    _lastTime=_changedOn;
  
  }

  unsigned long lastChanged(){return (_changedOn-_lastTime);}

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
  
  CMsg makeMessage(const char * str){
    CMsg m;
    std::string param;
    
    param=str; param+="_R";m.Parameters[param]=tostring(*pR);
    param=str; param+="_X";m.Parameters[param]=tostring(*pX);
    param=str; param+="_Y";m.Parameters[param]=tostring(*pY);
    param=str; param+="_Z";m.Parameters[param]=tostring(*pZ);
    param=str; param+="_T";m.Parameters[param]=tostring(_changedOn);    

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
    
    float K_R(){return kfR.updateEstimate(*pR);}
    float K_X(){return kfX.updateEstimate(*pX);}
    float K_Y(){return kfY.updateEstimate(*pY);}
    float K_Z(){return kfZ.updateEstimate(*pZ);}


    void remap(float * pnewX, float *pnewY, float *pnewZ);    
};

class CIMU:public CSystemObject{
  private:
  

    long _dataUpdatedOn=0;
    char _address;
    TwoWire *_pWire=&Wire;
    std::string _dataMode="GYRO";

  public:
    BNO080 myIMU;  //options   q,a,m,g,l,e
    CIData PRY;
    CIData Quat;
    CIData Lin;
    CIData Mag;
    CIData Gyro;
    
    CIMU() {init();}

    void config(char addr, TwoWire *twowire=&Wire);
    bool ShutDown(){  setState("DONE"); return true;    }
   
    void init();
    void setup();
    void config(CMsg &msg);
    void setupI2C();
    void setupSPI();
    void loop();
    void test(CMsg &msg);
    void runOnce(CMsg &m);

    void GetData();
    std::string getDataMode(){return _dataMode;}
    void dataMode(std::string option, int period=50);
    CMsg fillData(){
      CMsg m;
      return m;
    };
    void switchPlay(){};      //This needs to be set manually by some other function using the IMU to have it start getting data  Other systems will advance automatically
    long UpdatedOn(){return _dataUpdatedOn;}
    void callCustomFunctions(CMsg &msg){};
    
    void remap(){
      Gyro.remap(&Gyro._fZ,&Gyro._fX,&Gyro._fY);  
      Mag.remap(&Mag._fZ,&Mag._fX,&Mag._fY);  
      Lin.remap(&Lin._fZ,&Lin._fX,&Lin._fY);  
      Quat.remap(&Quat._fZ,&Quat._fX,&Quat._fY);  
      PRY.remap(&PRY._fZ,&PRY._fX,&PRY._fY);        
    }
  
};    
