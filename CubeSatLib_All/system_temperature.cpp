#include "system_temperature.h"

void CTemperatureObject::init(){
  CSystemObject::init();
  float _temp=0.0;
  long _time=0;
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
  CMsg m;
  runOnce(m);
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
  return m;
}




void CTempMaster::setup() {
  CSystemObject::init();  
  setInterval(1000);


  setState("PLAY");
}

void CTempMaster::runCustomFunctions(CMsg &m){

  m.setACT("");
}


float CTempMaster::getTemp(const char *sysname){
  CTemperatureObject *pTemp=(CTemperatureObject *) getSystem(sysname,"float CTempMaster::getTemp(const char *sysname)");
  float ftmp=-99.0;
  if(pTemp!=NULL){
    ftmp=pTemp->getTemp();
  }
  return(ftmp);
    
}

void CTempMaster::output(){
  std::string act=_cmsg.getACT();
  CMsg m;

  if(act=="SENDALL"){
    m.setParameter("TEMPX1",getTemp("TEMPX1"));
    m.setParameter("TEMPX2",getTemp("TEMPX2"));

    m.setParameter("TEMPY1",getTemp("TEMPY1"));
    m.setParameter("TEMPY2",getTemp("TEMPY2"));

    m.setParameter("TEMPZ1",getTemp("TEMPZ1"));
    m.setParameter("TEMPZ2",getTemp("TEMPZ2"));
  }
  else{    
    m.setParameter(act.c_str(),getTemp(act.c_str()));
  }
 
  addTransmitList(m);    
}
 

void CTempMaster::loop(){
  _cmsg.setACT("SENDALL");
  output();

}
