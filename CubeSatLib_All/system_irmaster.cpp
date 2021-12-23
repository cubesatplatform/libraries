#include "system_irarray.h"


CIRMaster::CIRMaster(){  
  Name("IR");
  }

CIRMaster::~CIRMaster(){}

void CIRMaster::init(){
  CSystemObject::init();
  setForever();
  setInterval(60000);
}
  
void CIRMaster::setup(){    
  init();
  setState("PLAY");
}

    
void CIRMaster::callCustomFunctions(CMsg &msg){
}


void CIRMaster::callOutput(const char *sysname){
  CMsg m;
  CIRArray *ptmp=(CIRArray *) getSystem(sysname,"void CIRMaster::callOutput(const char *sysname)");  
  if(ptmp!=NULL) ptmp->Output(m);
}


void CIRMaster::Output(CMsg &msg){
  std::string panel=msg.getParameter("PANEL","IRX1");
  CMsg m;

  if(panel=="ALL"){
    callOutput("IRX1");
    callOutput("IRX2");

    callOutput("IRY1");
    callOutput("IRY2");

    callOutput("IRZ1");
    callOutput("IRZ2");
  }
  else{        
    if(panel.size()>0) callOutput(panel.c_str());
  }
 
  addTransmitList(m);    
}
 

void CIRMaster::loop(){
  //_cmsg.setACT("SENDALL");
  //output();

}

 
