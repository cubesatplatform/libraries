#include "system_temperature.h"

void CTemperatureObject::init(){
  CSystemObject::init();
  _temp=0.0;
  _ltime=0;
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
  //if(subscribers(Name())){
  CMsg m;
  runOnce(m);
  //}
}

void CTemperatureObject::test(CMsg &msg){
  CSystemObject::test(msg);  
  CMsg m;
  runOnce(m);

  m.setSYS(Name());
  m.setParameter("TEMP",_temp);
  m.setParameter("TIME",_ltime);

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
   _ltime=getTime();
  }
return;
}

CMsg CTemperatureObject::getTempMSG(){
 
  CMsg m;
  runOnce(m);

  m.setSYS(Name());
  m.setParameter("TEMP",_temp);
  m.setParameter("TIME",_ltime);

  addDataList(m);
  return m;
}



