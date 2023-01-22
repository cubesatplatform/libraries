#pragma once

#include <map>
#include <list>
#include <systemobject.h>
#include <fhmotor.h>
#include <system_imu.h>

#define _REACTIONWHEELS "REACTIONWHEELS"
 
    
class CRW:public CSystemObject{
  private:         
    

  public:
    CRW();
    
    ~CRW(){}
    void setup();
    void loop();        
    

    void callCustomFunctions(CMsg &msg) override;
    CMotorController *getMotorAxis(char axis);
};
