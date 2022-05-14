#include "acds_defs.h"
#include "system_magtorquer.h"


#define PWM_MAX 4000


CMagTorquer::CMagTorquer():CSystemObject(){    
  init();
  Name("MT");
  setInterval(40);
}

CMagTorquer::~CMagTorquer(){}

void CMagTorquer::init()  {
  CSystemObject::init();
}

void CMagTorquer::setup()  {
  init();
 _pIMU=(CIMU *)getIMU();
 
  if(_pIMU==NULL){           
    CMsg m;
    m.setSYS(Name());
    m.setCOMMENT("ERROR   COULDNT START IMU.  Called from MagTorquer ...........................................");  
    m.setINFO("ERROR");
    addTransmitList(m);
    writeconsoleln(m.serializeout());
    setState("ERROR");
    return;
  }   
  
  _pMagX=(CMDrive *)getSystem("MAGX");
  _pMagY=(CMDrive *)getSystem("MAGY");
  _pMagZ=(CMDrive *)getSystem("MAGZ");
  setForever();
  setInterval(20);
 
  setState("PLAY");
}



bool CMagTorquer::isIdle(){  

  if(_pMagX!=NULL) {
    if(_pMagX->State()!="PAUSE") return false;
  }
  if(_pMagY!=NULL) {
    if(_pMagY->State()!="PAUSE") return false;
  }
  if(_pMagZ!=NULL) {
    if(_pMagZ->State()!="PAUSE") return false;
  }
  return true;
}



void CMagTorquer::deactivate(){
  if(_pMagX!=NULL) _pMagX->Speed(0);
  if(_pMagY!=NULL) _pMagY->Speed(0);
  if(_pMagZ!=NULL) _pMagZ->Speed(0);
}


void CMagTorquer::getGyroData(){
  _gyroX=*_pIMU->Gyro.pZ;
  _gyroY=*_pIMU->Gyro.pY;    
  _gyroZ=*_pIMU->Gyro.pX;
}


void CMagTorquer::getMagData(){
  _gyroX=*_pIMU->Mag.pZ;
  _gyroY=*_pIMU->Mag.pY;    
  _gyroZ=*_pIMU->Mag.pX;
}


void CMagTorquer::activate(CMDrive *pMag, float gyro, float mag){
  if(mag>MAGLIMIT){
      if(pMag!=NULL) pMag->Speed(PWM_MAX);    
    } 
    
  if(mag<-MAGLIMIT){
      if(pMag!=NULL) pMag->Speed(-PWM_MAX);    
    }
}
  

void CMagTorquer::loopDetumble(){

if(_pIMU==NULL) {writeconsoleln("NO IMU!!!!!!!!!!!!!!!!!!!!!");setState("ERROR");return;}

  switch (getStep()){

  case 0: 
    _pIMU->dataMode("GYRO", GYROPERIOD);
    break;
  case 1:
    getGyroData();    
    break;
  case 2:
    deactivate();
    _pIMU->dataMode("MAG", MAGPERIOD);
    break;  
  case 3:
    getMagData();
    activate(_pMagX,_gyroX,_magX);
    activate(_pMagY,_gyroY,_magY);
    activate(_pMagZ,_gyroZ,_magZ);
    break;      
  }

  incStep();
  if (getStep()>5) setStep(0);  
}


void CMagTorquer::newCMD(CMsg &msg){
  setMode(msg.getParameter("MODE",Mode()));
  std::string xMode=msg.getParameter("XMODE","");
  float xsetpoint=msg.getParameter("XSETPOINT",0.0);
  int xduration=msg.getParameter("XDURATION",10000);

  std::string yMode=msg.getParameter("YMODE","");
  float ysetpoint=msg.getParameter("YSETPOINT",0.0);
  int yduration=msg.getParameter("YDURATION",10000);

  std::string zMode=msg.getParameter("ZMODE","");
  float zsetpoint=msg.getParameter("ZSETPOINT",0.0);
  int zduration=msg.getParameter("ZDURATION",10000);

  
  if(xMode.size()){
    if(_pMagX!=NULL) {
      _pMagX->Speed(xsetpoint);      
      _pMagX->setDuration(xduration);
    }
  }
  if(yMode.size()){
    if(_pMagY!=NULL) {
      _pMagY->Speed(ysetpoint);      
      _pMagY->setDuration(yduration);    
    }
  }
  if(zMode.size()){
    if(_pMagY!=NULL) {
      _pMagZ->Speed(zsetpoint);     
      _pMagZ->setDuration(zduration);     
    }
  }   
}


  

void CMagTorquer::loop(){ 
   if (Mode()=="CUSTOM"){   //Do some control
    if(_pIMU!=NULL) _pIMU->Run();
    if(_pMagX!=NULL) _pMagX->Run();
    if(_pMagY!=NULL) _pMagY->Run();
    if(_pMagZ!=NULL) _pMagZ->Run();
  }
  loopDetumble();         
  std::string str=State();
  if ((str=="STOP")||(str=="ERROR")){
    goLowPowerState();
  }
}  
