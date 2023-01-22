#include "acds_defs.h"
#include "system_magtorquer.h"


#define PWM_MAX 4000

CMagTorquer::CMagTorquer():CSystemObject(){    
  init();

  setInterval(20);
  
}

CMagTorquer::~CMagTorquer(){}

void CMagTorquer::init()  {
  CSystemObject::init();
}

void CMagTorquer::setup()  {
  init();


  setForever();
  setInterval(20);

  CMsg m;

  m.set(_NAME, _MAGNETORQUERS);
  m.set(_DEFAULTIMU, _IMU1);
  addDataMap(m);


  m.set(_SYS,_MAGX);
  m.set(_MODE,_DETUMBLE);
  addMessageList(m);
  m.set(_SYS,_MAGY);  
  addMessageList(m);
  m.set(_SYS,_MAGZ);  
  addMessageList(m);

  Mode(_DETUMBLE);
 
  setState(_PLAY);
}



void CMagTorquer::deactivate(){
  CMsg m;
  m.set(_SYS, _MAGX);
  m.set(_ACT, _DEACTIVATE);

  addMessageList(m);
  m.set(_SYS,_MAGY);  
//  addMessageList(m);
  m.set(_SYS,_MAGZ);  
//  addMessageList(m);  
}



void CMagTorquer::activate(){
  CMsg m;
  m.set(_SYS, _MAGX);
  m.set(_ACT, _ACTIVATE);

  addMessageList(m);
  m.set(_SYS,_MAGY);  
//  addMessageList(m);
  m.set(_SYS,_MAGZ);  
//  addMessageList(m);
 
 
}


  

void CMagTorquer::loopDetumble(){
  long STEPSIZE=2000;
  CMsg m,mm;

  if(getTime()<_nextStepTime)
    return;
  
  
  switch (getStep()){
  
  case 0:    
    writeconsole("Mag Case 0    Set Mode Gyro");
    m.set(_SYS,_strIMU);
    m.set(_ACT,_NEWMODE);
    m.set(_VALUE,_GYRO);
    addMessageList(m);
    
    _nextStepTime=getTime()+STEPSIZE/2;   
    
    break;
    
  case 1:    
    writeconsoleln("Mag Case 1    Generate Gyro Data");
    mm= getDataMap("IMUI_GYRO");
    mm.writetoconsole();
    
    _nextStepTime=getTime()+STEPSIZE;    
    
    break;
  case 2:
    writeconsoleln("Mag Case 2   Deactive Magnets");
    deactivate();
    _nextStepTime=getTime()+STEPSIZE/2;    
    break;


  case 3:          
    writeconsoleln("Mag Case 3   Turn on Magnet Data");
    m.set(_SYS,_strIMU);
    m.set(_ACT,_NEWMODE);
    m.set(_VALUE,_MAG);
    addMessageList(m);
    
    _nextStepTime=getTime()+STEPSIZE;    
    
    break;  
  case 4:    
    writeconsoleln("Mag Case 4  Activate Magnets with New Data");    
    mm= getDataMap("IMUI_MAG");
    mm.writetoconsole();
    activate();

    _nextStepTime=getTime()+STEPSIZE*2;    
    
    break;      
  }

  incStep();
  
  if (getStep()>5) {
    writeconsoleln("Mag Case 5");
    setStep(0);  
    _nextStepTime=getTime()+STEPSIZE;    
  }
}



  

void CMagTorquer::loop(){ 
  
  if (Mode()==_DETUMBLE){
    loopDetumble();     
  }    
  std::string str=state();
  if ((str==_STOP)||(str==_ERROR)){
    goState(_LOWPOWER);
  }
}  


void  CMagTorquer::callCustomFunctions(CMsg &msg){
  
  
  CSystemObject::callCustomFunctions(msg);
}
