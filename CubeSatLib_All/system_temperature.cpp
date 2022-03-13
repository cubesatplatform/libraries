#include "system_temperature.h"

void CTemperatureObject::init(){
  CSystemObject::init();
  _temp=0.0;
  _time=0;
  setInterval(10000);
}

void CTemperatureObject::setup(){
 
int count=0;
init();
while(count<5){
 if (sensor.begin(_address, *_pWire)== true)
  //if (sensor.begin() == true) // Function to check if the sensor will correctly self-identify with the proper Device ID/Address
  {
  setState("PLAY");
  return;
  }
  
  count++;
  delay(50);
}
setState("ERROR");
return;
}

void CTemperatureObject::loop(){
  if(subscribers(Name())){
  CMsg m;
  runOnce(m);
  }
}

void CTemperatureObject::test(){
  CSystemObject::test();  
  CMsg m;
  runOnce(m);

  m.setSYS(Name());
  m.setParameter("TEMP",_temp);
  m.setParameter("TIME",_time);

  addTransmitList(m);

}


void CTemperatureObject::config(char addr, TwoWire *pWire){  
  setInterval(60000);
  _address=addr;
  _pWire=pWire;

}

void CTemperatureObject::runOnce(CMsg &m){
  if (State()!="PLAY"){
    setup();    
    return;
}
 if (sensor.dataReady() == true) // Function to make sure that there is data ready to be printed, only prints temperature values when data is ready
  {
   _temp = sensor.readTempC();
   _time=getTime();
  }
return;
}

CMsg CTemperatureObject::getTempMSG(){
 
  CMsg m;
  runOnce(m);

  m.setSYS(Name());
  m.setParameter("TEMP",_temp);
  m.setParameter("TIME",_time);

  addDataList(m);
  return m;
}



