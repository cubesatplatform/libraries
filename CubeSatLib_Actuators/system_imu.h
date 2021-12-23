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
  unsigned long prevTime=0;
  unsigned long changedOn=0;

  float prevX=0.0;
  float prevY=0.0;
  float prevZ=0.0;
  float prevR=0.0;
  float prevAcc=0.0;
  
  float X=0.0;
  float Y=0.0;
  float Z=0.0;
  float R=0.0;
  float Acc=0.0;

  float &Pitch=X;
  float &Roll=Y;
  float &Yaw=Z;

  float &I=X;
  float &J=Y;
  float &K=Z;

  SimpleKalmanFilter kfR;
  SimpleKalmanFilter kfX;
  SimpleKalmanFilter kfY;
  SimpleKalmanFilter kfZ;
  CIData(){}
  void init();
  
  void archiveData(){
    prevR=R;
    prevX=X;
    prevY=Y;
    prevZ=Z;
    prevAcc=Acc;
    prevTime=changedOn;
  }

  unsigned long lastChanged(){return (changedOn-prevTime);}

  bool anythingNew(){
    const float hf=.02;   //Bandpass filter
    const float lf=.02;
    if(
      (abs(prevR)<=abs(R)+hf) &&     (abs(prevX)<=abs(X)+hf) &&     (abs(prevY)<=abs(Y)+hf) &&     (abs(prevZ)<=abs(Z)+hf)
      && (abs(prevR)>=abs(R)-lf) &&     (abs(prevX)>=abs(X)-lf) &&     (abs(prevY)>=abs(Y)-lf) &&     (abs(prevZ)>=abs(Z)-lf)    
    ) 
    
    
    return false;
    return true;
  }
  
  CMsg makeMessage(const char * str){
    CMsg m;
    std::string param;
    
    param=str; param+="_R";m.Parameters[param]=tostring(R);
    param=str; param+="_X";m.Parameters[param]=tostring(X);
    param=str; param+="_Y";m.Parameters[param]=tostring(Y);
    param=str; param+="_Z";m.Parameters[param]=tostring(Z);
    param=str; param+="_T";m.Parameters[param]=tostring(changedOn);    
    //writeconsole("Start makeMessage");     writeconsoleln(m.serialize());    
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

      if (sfirst[n-1]=='R') R=ftmp;
      if (sfirst[n-1]=='X') X=ftmp;
      if (sfirst[n-1]=='Y') Y=ftmp;
      if (sfirst[n-1]=='Z') Z=ftmp;
      if (sfirst[n-1]=='T') changedOn=std::atol(ssecond.c_str());              
    }
  //writeconsoleln("End Desearialize");
  }
    
    float K_R(){return kfR.updateEstimate(R);}
    float K_X(){return kfX.updateEstimate(X);}
    float K_Y(){return kfY.updateEstimate(Y);}
    float K_Z(){return kfZ.updateEstimate(Z);}
};

class CIMU:public CSystemObject{
  private:
  

    long _dataUpdatedOn=0;
    

  public:
    BNO080 myIMU;  //options   q,a,m,g,l,e
    CIData PRY;
    CIData Quat;
    CIData Lin;
    CIData Mag;
    CIData Gyro;
    
    CIMU() {init();}
    bool ShutDown(){  setState("DONE"); return true;    }
   
    void init();
    void setup();
    void setupI2C();
    void setupSPI();
    void loop();
    void runOnce(CMsg &m);

    void Output(CMsg &msg);
    void GetData();
    CMsg fillData(){
      CMsg m;
      return m;
    };
    void switchPlay(){};      //This needs to be set manually by some other function using the IMU to have it start getting data  Other systems will advance automatically
    long UpdatedOn(){return _dataUpdatedOn;}
    void callCustomFunctions(CMsg &msg){writeconsoleln("---------------------------------------------- callCustomFunctions(msg)  ------------------------------------------------------------");};

/*
    float K_GyroX(){tic();return simpleKalmanFilterGyroX.updateEstimate(gyrox);}
    float K_GyroY(){tic();return simpleKalmanFilterGyroY.updateEstimate(gyroy);}
  */  
    
};    
