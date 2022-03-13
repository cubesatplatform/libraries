#pragma once

#include <map>
#include <list>
#include <systemobject.h>
#include <system_imu.h>
#include <fhmotor.h>
 
    
class CRW:public CSystemObject{
  private:    
     CMsg _tmpSpeed;
     CIMU *_pIMU=NULL; //Open I2C ADR jumper - goes to address 0x4B

     CMotorController *pMotorX=NULL;
     CMotorController *pMotorY=NULL;
     CMotorController *pMotorZ=NULL;

  public:
    CRW();
    
    ~CRW(){}
    void setup();
    void loop();    
    void setSpeed(CMsg &msg);
    void setTmpSpeed(CMsg &msg);
    void adjustSpeed();
    void callCustomFunctions(CMsg &msg);
};
