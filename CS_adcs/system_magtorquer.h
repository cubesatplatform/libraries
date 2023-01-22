#pragma once

#include <map>

#include <systemdefs.h>
#include <systemobject.h>


#include <mdrive.h>
#include <system_imu.h>
 


    
class CMagTorquer:public CSystemObject{
  private:    
    
    long _nextStepTime=0;
    std::string _strIMU=_IMUI;
    
  public:
    CMagTorquer();    
    ~CMagTorquer();

    void setup();      
    void loop();
    void init();
    
    void activate(CMDrive *pMag, float gyro, float mag);        
    void deactivate();    
    void activate();    

    void loopDetumble();      
    void callCustomFunctions(CMsg &msg);      
};
