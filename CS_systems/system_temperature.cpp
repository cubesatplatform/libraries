#include "system_temperature.h"

CTemperatureObject::CTemperatureObject(){ 
  Name("TEMP");
  init();
  _pWire=&Wire;
  };

void CTemperatureObject::init(){
  CSystemObject::init();
  _temp=0.0;
  _ltime=0;
  setInterval(10000);
  setErrorThreshold(4);
}

void CTemperatureObject::setup(){
if(_pWire==NULL) {
  sendError();
  return;
}

init();

for(int count=0;count<5;count++){
 if (sensor.begin(_address, *_pWire)== true)
  {  
  setState("PLAY");
  return;
  }
  else{   
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

void CTemperatureObject::test(CMsg &msg){
  Run(50);
  echoData(msg);
}


void CTemperatureObject::config(char addr, TwoWire *pWire){  
  setInterval(60000);
  _address=addr;
  _pWire=pWire;
}


void CTemperatureObject::config(CMsg &msg){
  std::string straddress=msg.getParameter("ADDRESS");
  std::string strwire=msg.getParameter("WIRE");

  TwoWire *pWire;

  if(strwire=="Wire") pWire=&Wire;  
  if(strwire=="Wire1") pWire=&Wire1;  
  if(strwire=="Wire2") pWire=&Wire2;  

  if(straddress.size()>0){
    config(straddress[0],pWire);
  }
}

void CTemperatureObject::runOnce(CMsg &m){

 if (sensor.dataReady() == true) // Function to make sure that there is data ready to be printed, only prints temperature values when data is ready
  {
   _temp = sensor.readTempC();
   _ltime=getTime();

    m.setKEY(Name());
    m.setTIME(_ltime);
    m.setParameter("TEMP",_temp);
    
    addDataMap(Name(),m);
  }
  else{
    incErrorCount();
  }
return;
}



void CTemperatureObject::echoData(CMsg &msg){
  if (State()!="ERROR"){
    CMsg m=getDataMap(std::string(Name()));
    addTransmitList(m);
  }
}