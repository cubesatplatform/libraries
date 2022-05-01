#pragma once

#include <map>
#include <systemobject.h>
#include <system_imu.h>
#include <mdrive.h>
 
    
class CMagTorquer:public CSystemObject{
  private:    
    CMDrive *_pMagX=NULL;
    CMDrive *_pMagY=NULL;
    CMDrive *_pMagZ=NULL;
    CIMU *_pIMU=NULL; 
    float _gyroX=0.0;
    float _gyroY=0.0;
    float _gyroZ=0.0;

    float _magX=0.0;
    float _magY=0.0;
    float _magZ=0.0;
    
  public:
    CMagTorquer();    
    ~CMagTorquer(){}

    void setup();      
    void loop();
    void init();
    bool isIdle();
    void activate(CMDrive *pMag, float gyro, float mag);
    void newCMD(CMsg &msg);
    void deactivate();    
    void getGyroData();
    void getMagData();
    void loopDetumble();            
};
