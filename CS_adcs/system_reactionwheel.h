#pragma once

#include <map>
#include <list>
#include <systemobject.h>
#include <system_imu.h>
#include <fhmotor.h>
 
    
class CRW:public CSystemObject{
  private:         
     CIMU *_pIMU=NULL; //Open I2C ADR jumper - goes to address 0x4B

     CMotorController *_pMotorX=NULL;
     CMotorController *_pMotorY=NULL;
     CMotorController *_pMotorZ=NULL;

  public:
    CRW();
    
    ~CRW(){}
    void setup();
    void loop();    
    void newCMD(CMsg &msg);

    void callCustomFunctions(CMsg &msg);
    CMotorController *getMotorAxis(char axis);
};
