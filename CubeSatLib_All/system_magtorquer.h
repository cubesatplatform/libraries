#pragma once
#include <map>
#include "defs.h"
#include "systemobject.h"
#include <system_imu.h>
#include <mdrive.h>
 
    
class CMagTorquer:public CSystemObject{
  private:
    long _timeStart=0;
    long _timeEnd=0;
    long _timeLast=0;    
    long _lastMag=0;
    unsigned long _prevTime=0;
    
    float _difx=0.0;
    float _dify=0.0;
    float _difz=0.0;
        
    float _lastx=0.0;
    float _lasty=0.0;
    float _lastz=0.0;
    
    CMDrive *pMAGX=NULL;
    CMDrive *pMAGY=NULL;
    CMDrive *pMAGZ=NULL;
    CIMU *_pIMU=NULL; 
    
  public:
    CMagTorquer();    
    ~CMagTorquer(){}

    void setup();      
    void loop();
    void init();
    bool isIdle();
    void Detumble();        
    bool calcDiffs();
    bool activateMag(CMsg &msg) ;
    bool activateMag(char axis,float val) ;
    
};
