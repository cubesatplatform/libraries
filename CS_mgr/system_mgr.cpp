#include "mgr_defs.h"
#if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
  //#include <analogWrite.h>  ???


  #define PHONE_TX 23
  #define PHONE_RX 4

#endif
#include "system_mgr.h"
#include <messages.h>
#include <powerup.h>
#include <system_imu.h>
#include <ceps.h>
#include <powerup.h>




#define PHONE_BAUD_RATE 115200

CSystemMgr::CSystemMgr(){
  Name("MGR");
  setForever();
  setInterval(300);
  }; 
         
CSystemMgr::~CSystemMgr(){}  

void CSystemMgr::setup(){  
  init();
  setState("PLAY");
  }
  
void CSystemMgr::init(){  
  initPins();  
  initI2CMap();
}


void CSystemMgr::loop(){  
  
  }



void CSystemMgr::getData(CMsg &msg){
  
  CMsg m=getDataMap(msg.getNAME());  
  addTransmitList(m);   
};




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






void CSystemMgr::showTests() {
  initPins();  
  CMsg m;
  
  std::string tmpstr;
  int n;
  for (auto it : Pins) {    
    m.setParameter(it.first,it.second);    
  }
  
m.setParameter("RADIO","RADIO");
m.setParameter("MAG","MAG");
m.setParameter("MAGX","MAGX");
m.setParameter("MAGY","MAGY");
m.setParameter("MAGZ","MAGZ");
m.setParameter("IR","IR");
m.setParameter("IRX1","IRX1");
m.setParameter("IRX2","IRX2");
m.setParameter("IRY1","IRY1");
m.setParameter("IRY2","IRY2");
m.setParameter("IRZ1","IRZ1");
m.setParameter("IRZ2","IRZ2");
m.setParameter("PINSON","PINSON");
m.setParameter("PINSOFF","PINSOFF");
m.setParameter("PHONEON","PHONEON");
m.setParameter("PHONEOFF","PHONEOFF");
m.setParameter("IMUSPI","IMUSPI");
m.setParameter("IMU0","IMU0");
m.setParameter("IMU1","IMU1");
m.setParameter("IMU2","IMU2");
m.setParameter("I2C0","I2C0");
m.setParameter("I2C1","I2C1");
m.setParameter("I2C2","I2C2");
m.setParameter("H","H4");
m.setParameter("L","L4");
m.setParameter("W","PWM4");
m.setParameter("Magnets","MAGX, MAGY, MAGZ");
m.setParameter("ICUs","I2C0, I2C1, I2C2");
m.setParameter("IMUs","IMU0, IMU1, IMU2, IMUSPI");
m.setParameter("Pins","HPH_15, LPH_15, WPH_15   -  High,  Low,  PWM");
writeconsoleln(m.serializeout());
addTransmitList(m);
}





void CSystemMgr::phone(){
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
 
  enablePhone();
   
  Serial1.begin(PHONE_BAUD_RATE);
#else  
   Serial1.begin(PHONE_BAUD_RATE,SERIAL_8N1, PHONE_TX, PHONE_RX);
#endif  

  Serial1.write("Hello");
   sendSerial("INIT"); //to synchronise Arduino and app
}

void CSystemMgr::sendSerial(const char* cmd) {    //Send to Phone
  Serial1.print(1);
  Serial1.flush();
  Serial1.print(",");
  Serial1.flush();
  Serial1.println(cmd);
  Serial1.flush();
  delayMicroseconds(500); 

}






void CSystemMgr::enableI2C(){
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  enableMBLogic();
  enable65V();  
  enableSensors();  
  

  CMsg m;
  m.setSYS("ENABLE I2C");
  m.setINFO("enableSensors");
  m.setCOMMENT("enableMags");
  writeconsoleln(m.serializeout());
  
  addTransmitList(m);
  #endif
}



void CSystemMgr::disableI2C(){
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  disableMBLogic();
  disableSensors();  
  disable65V();  

  CMsg m;
  m.setSYS("DISABLE I2C");
  m.setINFO("disableSensors");
  m.setCOMMENT("disableMags");
  writeconsoleln(m.serializeout());
  
  addTransmitList(m);
  #endif
}


void CSystemMgr::resetWire(TwoWire *wire,const char * s) {
    if(wire==NULL)
      return;

    enableI2C();     
    wire->begin();
    CMsg m;
    m.setSYS("TWOWIRE RESET");
    m.setINFO(s);
    writeconsoleln(m.serializeout());

}




void CSystemMgr::testIMU(CMsg &msg){  

  std::string act=msg.getACT(); 
  CIMU *pTest=(CIMU *)getSystem(act.c_str(),"IMUI2C");

  if(pTest!=NULL) {
    writeconsoleln(act);      
    pTest->test(msg);
  }
  else
    writeconsoleln("Could not find System IMU");  
 

  return;
}


void CSystemMgr::testMotor(CMsg &msg){
  std::string s=msg.getACT(); 
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  writeconsole("ENTER void CSystemMgr::testMotor(char axis): ");  
  writeconsoleln(s);
  
  enable65V();          

  CMotorController *pTest=(CMotorController *)getSystem(s.c_str(),"Test Motor");
  if(pTest!=NULL) {
    writeconsoleln("Testing Motor");    
    pTest->test(msg);
  }
  //disable65V();    
  #endif
}

void CSystemMgr::testMAG(CMsg &msg){
  std::string s=msg.getACT(); 
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  writeconsoleln("ENTER void CSystemMgr::testMAGDrive(char addr)");  
  enableSensors();  
  enable65V();  

  CMDrive *pTest=(CMDrive *)getSystem(s.c_str(),"Test Mag");
  if(pTest!=NULL) {    
    pTest->test(msg);
  }
  
  disable65V();
  
  writeconsoleln("EXIT void CSystemMgr::testMAGDrive(char addr)");
  #endif
}



void CSystemMgr::testDataMap(CMsg &msg){  //Usually stops over 600 so 500 is a safe number
//SYS:MGR~ACT:SENDDATA~START:7~END:10~K:KEY_
std::string key="KEY_";
  for(int count=0; count<500;count++){
    std::string key1=key+tostring(count);
    CMsg m;
    m.setParameter("VAL","0123456789QWERTYUIOP[]ASDFGHJKL;ZXCVBNM,./qwertyuiop[]asdfghjkl;zxcvbnm,./0123456789QWERTYUIOP[]ASDFGHJKL;ZXCVBNM,./qwertyuiop[]asdfghjkl;zxcvbnm,");
    addDataMap(key1,m);
    
    writeconsoleln(count);
    delay(50);
  }
}



void CSystemMgr::testIR(CMsg &msg){
  std::string s=msg.getACT(); 
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  writeconsoleln("ENTER void CSystemMgr::testIR(char addr)");  
  enableSensors();  
  CIRArray *pTest=(CIRArray *)getSystem(s.c_str(),"Test IR");
  if(pTest!=NULL) {    
    pTest->test(msg);
  }
  
  writeconsoleln("EXIT void CSystemMgr::testIR(char addr)");
  #endif
}

void CSystemMgr::testTemp(CMsg &msg){
  std::string s=msg.getACT(); 
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  writeconsoleln("ENTER void CSystemMgr::testTemp(const char *s)");  
  enableSensors();  
  CTemperatureObject *pTest=(CTemperatureObject *)getSystem(s.c_str(),"Test Temp");
  
  if(pTest!=NULL) {    
    pTest->test(msg);
  }
  
  writeconsoleln("EXIT void CSystemMgr::testTemp(const char *s)");  
  #endif
}


void CSystemMgr::chkTemperature(CMsg &msg){

  writeconsoleln("ENTER void CSystemMgr::chkTemperature(CMsg &msg)");  
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  
  
  enableSensors();  
  delay(10);

  const char * temps [] =  {"TEMPX1","TEMPX2","TEMPY1","TEMPY2","TEMPZ1","TEMPZ2","TEMPOBC"};
  unsigned long ct=getTime();

  CMsg msgTemps;
  
  msgTemps.setParameter("TIME",ct);

  
  for(auto s:temps){
    writeconsoleln(s);
    CTemperatureObject *pTest=(CTemperatureObject *)getSystem(s,s);  
    if(pTest!=NULL) {
      CMsg m;
      pTest->Run(50);      
      unsigned long ltime=pTest->getTempTime();
   
     if(ltime>=ct){                      
        msgTemps.setParameter(s,pTest->getTemp()); 
      }      
    }    
  }
  addTransmitList(msgTemps);
#endif
  
  writeconsoleln("EXIT void CSystemMgr::chkTemperature(CMsg &msg)");  
  
}

void CSystemMgr::chkIRArrays(CMsg &msg){
  const char * temps [] =  {"IRX1","IRX2","IRY1","IRY2","IRZ1","IRZ2"};
  unsigned long ct=getTime();

  for(int count=0;count<5;count++){
    for(auto s:temps){
      CIRArray *pTest=(CIRArray *)getSystem(s,s);  
      if(pTest!=NULL)       
        pTest->Run(20);
    }
  }

   CMsg m;
  for(auto s:temps){
    CIRArray *pTest=(CIRArray *)getSystem(s,s);  
    if(pTest!=NULL) {
     
      pTest->Output(m);                     
    }    
  }
}




void CSystemMgr::testMAGDrive(char addr){
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  Adafruit_DRV8830 drv;


  enableSensors();
  enable65V();
  
  if (! drv.begin(addr,   &Wire2 )) {
    writeconsoleln("Failed to find DRV8830");
    disable65V();
    return;
  }
  writeconsoleln("Adafruit DRV8830 found!");


   writeconsoleln("Forward");
  drv.run(FORWARD);
  drv.setSpeed(255);
  delay(10000);

  writeconsoleln("Release");
  drv.run(RELEASE);
  delay(2000);

  writeconsoleln("Backward");
  drv.run(BACKWARD);
  drv.setSpeed(255);
  delay(10000);

  drv.run(RELEASE);
  writeconsoleln("Mag test Done");
  disable65V();
  return;
  #endif
}



void CSystemMgr::loopWire(TwoWire *wire,const char * s) {  //I2C2 Sensitive to VOltage   Needs 3.4V sometimes
  if(wire==NULL){
    CMsg m;
    m.setSYS("TWOWIRE");
    m.setINFO("ERROR NULL");
    writeconsoleln(m.serializeout());

  }
  
  
  byte error, address;
  int nDevices=0;
  int naddress;
  //enableSensors();
  //enable65V();  
  
  CMsg m;
  m.setSYS("LoopWire");
  m.setINFO("Scanning");
  wire->begin();
  delay(500);
  for(byte address = 1; address < 127; address++ ) {
    std::string strI;
    std::string strIM;
    strI=s;
    strI+="_";

    writeconsole(".");
    
    wire->beginTransmission(address);
    
    error = wire->endTransmission();
    //continue;
    if (error == 0) {      
      //if (address<16)  writeconsole("0");
      
      std::string pstr= "Address_";
      pstr+=address; 
      naddress=address;
      strI+=tostring(naddress);
      strIM=I2CMap[strI];
      m.setParameter(pstr,address);
      m.setParameter(strI,strIM);
      nDevices++;
    }
    else if (error==4) {
      writeconsole("Unknow error at address 0x");
      if (address<16) {
        writeconsole("0");
      }
      writeconsoleln(address);  //HEX
      std::string pstr= "UnknownAddress_";
      pstr+=address; 
      m.setParameter(pstr,address);
    }    
  }
  if (nDevices == 0) {
    m.setINFO("No devices found.");
  }
 
  writeconsoleln(m.serializeout());
  
  addTransmitList(m);
  //disable65V();  
  delay(150);          
}




void CSystemMgr::sendBeacon(CMsg &msg){  
  CMsg msg1;
  //msg1.setParameter("STATE",getSatState());
  msg1.setParameter("TIME",getTime());

  CMsg m=getDataMap(POWERKEY);
  
  msg1.setParameter("BatteryVolt",m.getParameter("BatteryVolt"));
  msg1.setParameter("BatteryCrnt",m.getParameter("BatteryCrnt"));
  msg1.setParameter("BatTemp1",m.getParameter("BatTemp1"));
  msg1.setParameter("XVolt",m.getParameter("XVolt"));
  msg1.setParameter("YVolt",m.getParameter("YVolt"));
  msg1.setParameter("ZVolt",m.getParameter("ZVolt"));

  CRadio *pRadio=(CRadio *)getSystem("RADIO");
  CRadio *pRadio2=(CRadio *)getSystem("RADIO2");
  if(pRadio!=NULL){
    msg1.setParameter("R_STATE",pRadio->State());
  }
  
  if(pRadio2!=NULL){
    msg1.setParameter("R2_STATE",pRadio2->State());
  }


  addTransmitList(msg1);
}
