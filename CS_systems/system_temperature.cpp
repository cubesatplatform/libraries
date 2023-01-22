#include "system_temperature.h"



CTemperatureObject::CTemperatureObject(){   
  init();
  _pWire=&Wire;
  };

void CTemperatureObject::init(){
  CSystemObject::init();

  _temp=0.0;
  _ltime=0;
  setInterval(40);
  setErrorThreshold(4);
}

void CTemperatureObject::setup(){

init();

if (_pWire==NULL){
  _pWire=&Wire;
  writeconsoleln("ERROR  pWire NULL  ERROR");
}

for(int count=0;count<5;count++){  
 if (sensor.begin(_address, *_pWire)== true)
  {      
  setState(_PLAY);
  return;
  }
  else{   
    writeconsoleln("Error");
    incErrorCount();               
  }
  
  delay(50);
}

sendError();
return;
}

void CTemperatureObject::loop(){    
  CMsg m;
  runOnce(m);  
}




void CTemperatureObject::config(char addr, TwoWire *pWire){    
  setInterval(10);
  _address=addr;
  _pWire=pWire;
}


void CTemperatureObject::config(CMsg &msg){  
  std::string straddress=msg.get(_ADDRESS);
  std::string strwire=msg.get(_WIRE);

  

  if(straddress.size()>0){
    TwoWire *pWire;

    if(strwire==_WIRE) pWire=&Wire;  
    if(strwire==_WIRE1) pWire=&Wire1;  
    if(strwire==_WIRE2) pWire=&Wire2;  
    
    config(straddress[0],pWire);
  }
}

void CTemperatureObject::runOnce(CMsg &m){
 if (sensor.dataReady() == true) // Function to make sure that there is data ready to be printed, only prints temperature values when data is ready
  {
    writeconsoleln("K");
    CMsg msg;
    msg=getDataMap(_SATINFO);
   _temp = sensor.readTempC();
   _ltime=getTime();
    
    msg.set(name(),_temp);
    msg.set(name()+"_T",_ltime);

    msg.writetoconsole();
    
    addDataMap(_SATINFO,msg);
  }

return;
}


