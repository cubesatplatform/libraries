#include "system_magtorquer.h"

#define MAGDELAY 50
#define MAGIMUINTERVAL 50

CMagTorquer::CMagTorquer():CSystemObject(){    
  init();
  Name("MT");
  setInterval(40);
}

void CMagTorquer::init()  {
  CSystemObject::init();
  _timeStart=0;
  _timeEnd=0;
  _timeLast=0;    
  _lastMag=0;
  _prevTime=0;

  _difx=0.0;
  _dify=0.0;
  _difz=0.0;
    
  _lastx=0.0;
  _lasty=0.0;
  _lastz=0.0;
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
  
  pMAGX=(CMDrive *)getSystem("MAGX");
  pMAGY=(CMDrive *)getSystem("MAGY");
  pMAGZ=(CMDrive *)getSystem("MAGZ");
  setForever();
  setInterval(20);

  _timeEnd=0;
  _timeStart=getTime();

  setState("PLAY");
  }


bool CMagTorquer::calcDiffs(){  
   if((abs(_pIMU->Mag.X)>200) || (abs(_pIMU->Mag.Y)>200)  || (abs(_pIMU->Mag.Z)>200)) return false;   //Bad data  continue
  
   float estx = _pIMU->Mag.K_X();
   float esty = _pIMU->Mag.K_Y();
   float estz = _pIMU->Mag.K_Z();

   _difx=(estx-_lastx);
   _dify=(esty-_lasty);
   _difz=(estz-_lastz);

   
   _lastx=estx;
   _lasty=esty;
   _lastz=estz;
   
   return true;

}


bool CMagTorquer::isIdle(){  

  if(pMAGX!=NULL) {
    if(pMAGX->State()!="PAUSE") return false;
  }
  if(pMAGY!=NULL) {
    if(pMAGY->State()!="PAUSE") return false;
  }
  if(pMAGZ!=NULL) {
    if(pMAGZ->State()!="PAUSE") return false;
  }
  return true;
}


void CMagTorquer::Detumble(){  
  if(_pIMU->State()=="ERROR"){
      CMsg m;
      m.setSYS(Name());
      m.setCOMMENT("Mag Torqure - IMU Error  leaving...");  
      m.setINFO("ERROR Detumble");
      addTransmitList(m);
      writeconsoleln(m.serializeout());
      setState("ERROR");
      return; 
  }
  if(calcDiffs()) {               
    if ((abs(_difx)<0.3)&&(abs(_dify)<0.3)&&(abs(_difz)<0.3)){
      stop();      
      return;
    }

   CMsg msg;
   msg.setSYS("MAGX");
   msg.setACT("SPEED");
   msg.setParameter("SPEED",255);
   msg.setParameter("DURATION",50);
   
   if ((_difx>0.3) && (pMAGX!=NULL)) pMAGX->Speed(1.0,1000);   
   if ((_difx<-0.3) && (pMAGX!=NULL)) pMAGX->Speed(-1.0,1000);
      
   if ((_dify>0.3) && (pMAGY!=NULL)) pMAGY->Speed(1.0,1000);      
   if ((_dify<-0.3) && (pMAGY!=NULL))  pMAGY->Speed(-1.0,1000);
   
   if ((_difz>0.3) && (pMAGZ!=NULL)) pMAGZ->Speed(1.0,1000);      
   if ((_difz<-0.3) && (pMAGZ!=NULL)) pMAGZ->Speed(-1.0,1000);      
  }
}

void CMagTorquer::loop(){ 
    Detumble();         
}  
