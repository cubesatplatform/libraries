#include "acds_defs.h"
#include "system_reactionwheel.h"

//https://randomnerdtutorials.com/esp32-pwm-arduino-ide/
//https://circuits4you.com/2018/12/31/esp32-pwm-example/
//https://circuits4you.com/2017/12/21/esp8266-pwm-example/``
// the number of the LED pin
//const int ledPin = 2;  // 16 corresponds to GPIO16

#define GYROLIMIT 0.1
#define GYROPERIOD 50
#define GYROWAIT 50


CRW::CRW():CSystemObject(){
  init();
  Name("RW");   
}


CMotorController *CRW::getMotorAxis(char axis){
  if(axis=='X') return _pMotorX;
  if(axis=='Y') return _pMotorY;
  if(axis=='Z') return _pMotorZ;
  return NULL;

}

void CRW::newCMD(CMsg &msg){
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
    if(_pMotorX!=NULL) {
     _pMotorX->setMode(xMode);
     _pMotorX->setPoint(xsetpoint,xduration);
    }
  }
  if(yMode.size()){
    if(_pMotorY!=NULL) {
     _pMotorY->setMode(yMode);
     _pMotorY->setPoint(ysetpoint,yduration);
    }
  }
  if(zMode.size()){
    if(_pMotorZ!=NULL) {
     _pMotorZ->setMode(zMode);
     _pMotorZ->setPoint(zsetpoint,zduration);
    }
  }   
}



void CRW::setup()
{
  init();
  _pIMU=(CIMU *)getIMU();
 
  if(_pIMU==NULL){             
     if(incErrorCount()){
      sendError();
    }
    return;
  }   

 _pMotorX=(CMotorController *)getSystem("MOTORX");
 _pMotorY=(CMotorController *)getSystem("MOTORY");
 _pMotorZ=(CMotorController *)getSystem("MOTORZ");

  if(_pMotorX!=NULL) { _pMotorX->Speed(0,10000);}
  if(_pMotorY!=NULL) { _pMotorY->Speed(0,10000);}
  if(_pMotorZ!=NULL) { _pMotorZ->Speed(0,10000);}
  
  
  setForever();
  setInterval(20);
  setMode("");

  setState("PLAY");
}


void  CRW::callCustomFunctions(CMsg &msg){
  std::string act=msg.getACT();

  if(act=="CMD") newCMD(msg);  
}

void CRW::loop(){ 
  if (Mode()==""){   //Do some control
    if(_pIMU!=NULL) _pIMU->Run();
    if(_pMotorX!=NULL)_pMotorX->Run();
    if(_pMotorY!=NULL)_pMotorY->Run();
    if(_pMotorZ!=NULL)_pMotorZ->Run();
  }
}