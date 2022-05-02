#include  <string>
#include "system_mgr.h"
#include <messages.h>
#include "mgr_defs.h"
#include <powerup.h>
#include <ceps.h>

#define MAG_ADDRESS_X  0x60
#define MAG_ADDRESS_Y  0x61
#define MAG_ADDRESS_Z  0x63

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
#else
  #define Wire2 Wire
#endif
   
void CSystemMgr::initCommands(){
  CMsg m;
  std::list<CMsg> ml;
  
  ml.clear();

  m.clear();  
  m.setSYS("GPS");
  m.setACT("START");
  ml.push_back(m);

  m.clear();  
  m.setSYS("GPS");
  m.setACT("OUTPUT");
  m.setParameter("_START",3000L);
  m.setParameter("_STOP",30000L);
  m.setParameter("_INTERVAL",10000L);  
  ml.push_back(m);

  m.clear();  
  m.setSYS("GPS");
  m.setACT("STOP");
  m.setParameter("_START",33000L);
  m.setParameter("_STOP", 33000L);  
  ml.push_back(m);

  Commands["CMD_GPS_OUTPUT"]=ml;
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  ml.clear();

  m.clear();  
  m.setSYS("IRX1");
  m.setACT("START");
  ml.push_back(m);

  m.clear();  
  m.setSYS("IRX1");
  m.setACT("OUTPUT");
  m.setParameter("_START",3000L);
  m.setParameter("_STOP",300000L);
  m.setParameter("_INTERVAL",10000L);   
  ml.push_back(m);

  m.clear();  
  m.setSYS("IRX1");
  m.setACT("STOP");
  m.setParameter("_START",10000L);
  m.setParameter("_STOP",10000L);  
  ml.push_back(m);
  Commands["CMD_IRX1_OUTPUT"]=ml;



}

void CSystemMgr::showScheduler(CMsg &msg){
  writeconsoleln("");
  writeconsoleln("ShowScheduler xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");  
  for(auto m:Scheduler){    
    writeconsoleln(m.serialize());  
  }
   writeconsoleln("");
}

void CSystemMgr::showCommands(){
  writeconsoleln("");
  writeconsoleln("ShowCommands xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");  
  
  std::list<CMsg> ml;
  std::string namestr;
  for (auto it = Commands.begin(); it != Commands.end(); ++it) {    
    namestr = it->first;
    writeconsoleln(namestr);
    ml=it->second;  
    for(auto m:ml){    
      writeconsoleln(m.serialize());  
    }
  }
  writeconsoleln("");
}


 void CSystemMgr::callCustomFunctions(CMsg &msg){
  CMessages* pMSG= getMessages();
  std::string act=msg.getACT(); 
  writeconsoleln("");
  writeconsole(" CallCustomFunctions :  ");
  writeconsoleln(act);  


  if(act=="CHKRADIO") {chkRadio(msg); return; }
  if(act=="CHKTEMPERATURE") {chkTemperature(msg); return; }
  if(act=="CHKBATTERY") {chkBattery(msg); return; }
  if(act=="CHKROTATION") {chkRotation(msg); return; }
  if(act=="CHKMAGFIELD") {chkMagField(msg); return; }
  if(act=="CHKMESSAGES") {chkMessages(msg); return; }
  
  if(act=="SENDBEACON") {sendBeacon(msg); return; }
  if(act=="ADD")  {addSchedule(msg); return; }
  if(act=="DELETE")  {deleteSchedule(msg);   return; }
  if(act=="ENABLEI2C") {enableI2C(); return; }
  
  if(act=="SHOWTESTS") {showTests(); return; }
  if(act=="SHOWCOMMANDS") {showCommands(); return; }
  if(act=="SHOWSCHEDULER") {showScheduler(msg); return; }

  if(act== "MAGDX"){  testMAGDrive(MAG_ADDRESS_X); return; }
  if(act== "MAGDY"){  testMAGDrive(MAG_ADDRESS_Y); return; }
  if(act== "MAGDZ"){  testMAGDrive(MAG_ADDRESS_Z); return; }

  if(act== "ADCSON"){  enableADCS(); return; }
  if(act== "ADCSOFF"){  disableADCS(); return; }

  if(act== "SENSORSON"){  enableSensors(); return; }
  if(act== "SENSORSOFF"){  disableSensors(); return; }

  if(act== "MAGSMOTORSON"){  enableMagsMotors(); return; }
  if(act== "MAGSMOTORSOFF"){  disableMagsMotors(); return; }


  if(act=="TRANSMITDATA") {pMSG->movetoTransmitList(msg);return;}
  if(act=="DATALISTCLEAR") {pMSG->DataList.clear();return;}
  if(act=="MESSAGESLISTCLEAR") {pMSG->MessageList.clear();return;}
  if(act=="TRANSMITLISTCLEAR") {pMSG->TransmitList.clear();return;}
  if(act=="TRANSMITTEDLISTCLEAR") {pMSG->TransmittedList.clear();return;}
  if(act=="SENDDATA") {pMSG->sendData(msg);return;}
  if(act=="SUBSCRIBE") {pMSG->subscribe(msg.getDATA());return;}
  if(act=="UNSUBSCRIBE") {pMSG->unsubscribe(msg.getDATA());return;}

  if(act== "?") {    showTests();    return;  }

  if(act== "PINSON") {    pinsOn();    return;  }
  if(act== "PINSOFF") {    pinsOff();    return;  }
  
  if((act== "IMUI2C") || (act== "IMUSPI")) {    testIMU(msg);    return;  }
  
  if(act== "I2C0") {   loopWire(&Wire, "0");   return; }
  if(act== "I2C1") {   loopWire(&Wire1,"1");   return; }
  if(act== "I2C2") {   loopWire(&Wire2,"2");   return; }

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  if(act== "PHONEON") { phone();    return;  }
  if(act== "PHONEOFF") { disablePhone();    return;  }
  if(act== "BURN") {  burn();    return;  }
  if((act== "MOTORX") || (act== "MOTORY") || (act== "MOTORZ") ){  testMotor(msg);  return;}
  if((act== "IRX1") || (act== "IRX2") ||  (act== "IRY1") || (act== "IRY2") ||  (act== "IRZ1") || (act== "IRZ2") ){  testIR(msg);  return;}
  if((act== "MAGX") || (act== "MAGY") || (act== "MAGZ") ){testMAG(msg);return;}
  if((act== "TEMPX1") || (act== "TEMPX2") || (act== "TEMPY1") || (act== "TEMPY2") || (act== "TEMPZ1") || (act== "TEMPZ2") || (act== "TEMPOBC") || (act== "TEMPADCS")  ){    testTemp(msg);    return;  }
  if(act == "RESETI2C2") {    enableI2C();    resetWire(&Wire2,"2");    return;  }
#endif

  if(act == "RESETI2C0") {    enableI2C();    resetWire(&Wire, "0");    return;  }
  if(act == "RESETI2C1") {    enableI2C();    resetWire(&Wire1,"1");    return;  }
  
  if(act=="SCHEDULE") {SendCmdToScheduler(msg);return;}

  SendCmd(msg);
    
}


void CSystemMgr::chkBattery(CMsg &msg){
  CEPS *pEPS=(CEPS *)getSystem("EPS");
  
  if(pEPS==NULL)
    return;
  CMsg m=pEPS->m;
  addTransmitList(m);  
}

void CSystemMgr::chkRadio(CMsg &msg){
  CMsg m;
  CRadio *pRadio=(CRadio *)getSystem("RADIO");
  CRadio *pRadio2=(CRadio *)getSystem("RADIO2");
  

  if(pRadio!=NULL){
    m.setParameter("R_STATE",pRadio->State());
    m.setParameter("R_INSTATE",pRadio->getRadioState());
    m.setParameter("R_LASTTX",pRadio->getLastTransmit());
    m.setParameter("R_LASTRX",pRadio->getLastReceive());    
    m.setParameter("R_TM",pRadio->getTransmitter());
    }

  if(pRadio2!=NULL){
    m.setParameter("R2_STATE",pRadio2->State());
    m.setParameter("R2_INSTATE",pRadio2->getRadioState());
    m.setParameter("R2_LASTTX",pRadio2->getLastTransmit());
    m.setParameter("R2_LASTRX",pRadio2->getLastReceive());    
    m.setParameter("R2_TM",pRadio2->getTransmitter());
    }    
  addTransmitList(m);  

}


void CSystemMgr::chkMessages(CMsg &msg){
  CMessages* pM=getMessages();
  CMsg m;

  m.setParameter("lastReceived",pM->getLastReceived());
  m.setParameter("lastMessage",pM->getLastMessage());
  m.setParameter("lastData",pM->getLastData());
  m.setParameter("lastTransmit",pM->getLastTransmit());

  addTransmitList(m);  
}



void CSystemMgr::chkRotation(CMsg &msg){
  CIMU *pIMU=(CIMU *)getIMU();
  if(pIMU!=NULL){
    CMsg m;
    pIMU->test(m);
    m=pIMU->Gyro.makeMessage("GYRO");
    addTransmitList(m);
  }
}


void CSystemMgr::chkMagField(CMsg &msg){
  CIMU *pIMU=(CIMU *)getIMU();
  if(pIMU!=NULL){
    CMsg m;
    pIMU->test(m);
    m=pIMU->Mag.makeMessage("MAG");
    addTransmitList(m);
  }
}



void CSystemMgr::sendBeacon(CMsg &msg){//Have the Satelite send this out  so this just messages it

}
  
