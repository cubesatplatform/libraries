#include "mgr_defs.h"
#if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
  //#include <analogWrite.h>  ???


  #define PHONE_TX 23
  #define PHONE_RX 4

#endif
#include "system_mgr.h"
#include <messages.h>
#include <ceps.h>


#include <system_imu.h>




#define PHONE_BAUD_RATE 115200

CSystemMgr::CSystemMgr(){  
  setForever();
  setInterval(300);
  }; 
         
CSystemMgr::~CSystemMgr(){}  

void CSystemMgr::setup(){  
  init();
  setState(_PLAY);
  }
  
void CSystemMgr::init(){  

}


void CSystemMgr::loop(){  
  
  }



void CSystemMgr::chkAll(CMsg &msg){
  chkIRArrays(msg);
  chkTemperature(msg);
  chkRadio(msg);
  chkBattery(msg);
  chkRotation(msg);
  chkMagField(msg);
  chkMags(msg);
  chkMessages(msg);
  chkIMUI(msg);
  chkIMUS(msg);

  randomState();


  pinRead(msg);
  pinReadState(msg);
/*
  pinHigh(msg);  
  pinLow(msg);
  pinPWM(msg);
  
	
  pinsOn();
  pinsOff();
  runSystem(msg);
*/
  
  sendBeacon(msg);  
  
  //newState(msg);  

  showTests(); 
  
  loopWire(msg);

  resetWire(msg);
  enableI2C();
  disableI2C();
  phone();
  chargePhone();

  
  //controlPlan();
  
  mag(msg);
  motor(msg);
  temp(msg);
  ir(msg);
  IMU(msg);

  testDataMap(msg);
  getData(msg);
  scheduleData(msg);
  
  }

void CSystemMgr::sendPicture(CMsg &msg){
  int start = msg.get(_START,1);
  int stop = msg.get(_STOP,start+50);
  if(start<1) start=1;
  for (int count=start; count<stop; count++){
    CMsg m;
    std::string val = msg.get(_VALUE,"2");
    std::string streamstr="stream("+val+"_";
    streamstr+=tostring(count);
    streamstr+=".jpg,0)";
    //TO:BS169~SYS:PHONE~ACT:SENDSERIAL~V:stream(2_155.jpg,0)
    m.set(_SYS,"PHONE");
    m.set(_ACT,_SENDSERIAL);
    m.set(_VALUE,streamstr);
    m.writetoconsole();
    m.set(_PROCESSTIME,2500*(count-start));
    
    addMessageList(m);   
  }
};


void CSystemMgr::getData(CMsg &msg){
  
  CMsg m=getDataMap(msg.get(_NAME));  
  addTransmitList(m);   
};





void CSystemMgr::IMU(CMsg &msg){
  chkIMUI(msg);
  chkIMUS(msg);
  
  
}


//TO:ADR1~SYS:MANAGER~ACT:CHKIMUS
void CSystemMgr::chkIMUI(CMsg &msg){

  enablePin(_PINADCSI2C);
  CIMU *pIMU=(CIMU *)getSystem(_IMUI,"ChkRotation IMUI");
  if(pIMU!=NULL) {    
    pIMU->run(50);
    pIMU->goMag();
    pIMU->run(1500);
    pIMU->test(msg);
    std::string str=_IMUI;    
    str+=_MAG;
    CMsg m=getDataMap(str);  
    addTransmitList(m);  
  }    
}



//TO:ADR1~SYS:MANAGER~ACT:CHKIMUS
void CSystemMgr::chkIMUS(CMsg &msg){

  CIMU *pIMU=(CIMU *)getSystem(_IMUS,"ChkRotation IMUS");
  if(pIMU!=NULL) {   
    pIMU->setState(_BLANK);  //resets it  starts fresh
  
    pIMU->run(50);
    //pIMU->test(msg);
     
    
    pIMU->goMag();
    pIMU->run(1500);
    pIMU->test(msg);
    std::string str=_IMUS;    
    str+=_MAG;
    CMsg m=getDataMap(str);  
    addTransmitList(m);  
    
  }    
}



void CSystemMgr::chkBattery(CMsg &msg){
  
  CMsg m=getDataMap(_SATEPS);  
  addTransmitList(m);  
}

void CSystemMgr::chkRadio(CMsg &msg){
  CMsg m;
  CRadio *pRadio=(CRadio *)getSystem(_RADIO);
  CRadio *pRadio2=(CRadio *)getSystem(_RADIO2);
  
  m.set(_NAME,_CHKRADIO);

  if(pRadio!=NULL){
    m.set(R_STATE,pRadio->state());
    m.set(R_INSTATE,pRadio->getRadioState());
    m.set(R_LASTTX,pRadio->getLastTransmit());
    m.set(R_LASTRX,pRadio->getLastReceive());    
    m.set(R_TM,pRadio->getTransmitter());
    }

  if(pRadio2!=NULL){
    m.set(R2_STATE,pRadio2->state());
    m.set(R2_INSTATE,pRadio2->getRadioState());
    m.set(R2_LASTTX,pRadio2->getLastTransmit());
    m.set(R2_LASTRX,pRadio2->getLastReceive());    
    m.set(R2_TM,pRadio2->getTransmitter());
    }    
  addTransmitList(m);  

}


void CSystemMgr::chkMessages(CMsg &msg){
  CMsg m;

  m.set(_NAME,_CHECKMESSAGES);
  m.set(_LASTRECEIVED,MMM.getLastReceived());
  m.set(_LASTMESSAGE,MMM.getLastMessage());
  m.set(_LASTDATA,MMM.getLastData());
  m.set(_LASTTRANSMIT,MMM.getLastTransmit());

  addTransmitList(m);  
}



void CSystemMgr::chkRotation(CMsg &msg){
  
  CIMU *pIMU=(CIMU *)getSystem(_IMUI,"ChkRotation IMUI");
  writeconsoleln("A");
  if(pIMU!=NULL) {    
    writeconsoleln("B");
    pIMU->goGyro();
    writeconsoleln("C");
    pIMU->run(250);
    writeconsoleln("D");
  }

/*
  pIMU=(CIMU *)getSystem(_IMUS,"ChkRotation IMUS");
  if(pIMU!=NULL) {    
    pIMU->goGyro();
    pIMU->run(250);
  }
  */

  std::string str=_IMUI;  
  str+=_GYRO;
  float x=(float)getDataValue(str,_XAXIS);
  float y=(float)getDataValue(str,_YAXIS);
  float z=(float)getDataValue(str,_ZAXIS);
  long imut=(long)getDataValue(str,_TIME);

  long currentTime=getTime();

  writeconsoleln("ChkRotation: "+tostring(x)+","+tostring(y)+","+tostring(z)+","+tostring(imut)+","+tostring(currentTime));
  if(currentTime-imut<1000){
    if(abs(x)>0.2 || abs(y)>0.2 || abs(z)>0.2){
      goState(_DETUMBLE);
    }
   
  }
}


void CSystemMgr::chkMagField(CMsg &msg){  
  CIMU *pIMU=(CIMU *)getSystem(_IMUI,"ChkRotation IMUI");
  if(pIMU!=NULL) {    
    pIMU->goMag();
    pIMU->run(250);

    std::string str=_IMUI;    
    str+=_MAG;
    CMsg m=getDataMap(str);  
    addTransmitList(m);  
  }

  pIMU=(CIMU *)getSystem(_IMUS,"ChkRotation IMUI");
  if(pIMU!=NULL) {    
    pIMU->goMag();
    pIMU->run(250);
    std::string str=_IMUS;    
    str+=_GYRO;
    CMsg m=getDataMap(str);  
    addTransmitList(m);  
  }  
}


void CSystemMgr::randomState() {  
  long r=random(0,6);
  writeconsoleln("Random State: "+tostring(r));
  switch(r){
    case 0: goState(_LOWPOWER); break;
    case 1: goState(_NORMAL); break;
    case 2: goState(_DETUMBLE); break;
    case 3: goState(_ADCS); break;
    case 4: goState(_DEPLOY); break;
    case 5: goState(_PAYLOAD); break;    
  }
  
}



void CSystemMgr::showTests() {
  CMsg m;  
  std::string tmpstr;

  for (auto it : Pins) {    
    m.set(it.first,it.second);    
  }

}


void CSystemMgr::chargePhone(){

  enablePin(_PIN65V);  
  enablePin(_PINADCSI2C);
  enablePin(_PINPHONE);
}



void CSystemMgr::phone(){
#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  Serial1.begin(PHONE_BAUD_RATE);
#else  
   Serial1.begin(PHONE_BAUD_RATE,SERIAL_8N1, PHONE_TX, PHONE_RX);
#endif  

  Serial1.write("Hello");
  sendSerial("INIT"); //to synchronise Arduino and app
}

//TO:BS~SYS:PHONE~ACT:CBATTERY
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
  enablePin(_PIN65V);  
  enablePin(_PINOBCI2C);  
 // enablePin(_PINADCSI2C);
  

  CMsg m;
  m.set(_NAME,"ENABLEI2C");
  m.set(_INFO,"enableSensors");
  m.set(_COMMENT,"enableMags");
  m.writetoconsole();
  
  addTransmitList(m);
  
}



void CSystemMgr::disableI2C(){
  
  disablePin(_PINOBCI2C);  
  disablePin(_PIN65V);
  disablePin(_PINADCSI2C);

  CMsg m;
  m.set(_NAME,"DISABLEI2C");
  m.set(_INFO,"disableSensors");
  m.set(_COMMENT,"disableMags");
  writeconsoleln(m.serializeout());
  
  addTransmitList(m);
  
}



void CSystemMgr::resetWire(CMsg &msg){
  std::string s=msg.get(_VALUE,"2"); 

  if(s.size()==0)
    return;
  char c=s[0];

  resetWire(c);
}





void CSystemMgr::resetWire(char  s) {
  TwoWire *wire=NULL;

  if (s=='0') wire=&Wire;
  if (s=='1') wire=&Wire1;
  if (s=='2') wire=&Wire2;

  if(wire==NULL)
    return;

  wire->begin();
  CMsg m;
  m.set(_NAME,"TWOWIRE RESET");
  m.set(_INFO,s);
  m.writetoconsole();

}




void CSystemMgr::motor(CMsg &msg){
  writeconsole("ENTER void CSystemMgr::motor(CMsg &msg): ");  
  std::string s=msg.get(_VALUE,_MOTORX); 
  writeconsoleln(s);
  
  
  //enablePin(_PINOBCI2C);  
  //enablePin(_PIN65V);
  //enablePin(_PINADCSI2C);
    
  
  CSystemObject *pSys=getSystem(s.c_str(),"Test Motor");
  if(pSys!=NULL) {
    writeconsoleln("Testing Motor");    
    pSys->test(msg);
  }
  else {
    writeconsole("System not found: "); writeconsoleln(s);
  }
  //disablePin(_PIN65V);
  //disablePin(_PINADCSI2C);
  
}


//ACT:MAG~V:MAGZ
void CSystemMgr::mag(CMsg &msg){
  writeconsoleln("ENTER void CSystemMgr::mag(CMsg &msg) ");  
  std::string s=msg.get(_VALUE,_MAGZ); 
    

  CSystemObject *pSys=getSystem(s.c_str(),"Test Mag");
  if(pSys!=NULL) {    
    writeconsoleln(pSys->name());
    pSys->test(msg);
  }
  else {
    writeconsole("System not found: "); writeconsoleln(s);
  }
}



void CSystemMgr::testDataMap(CMsg &msg){  //Usually stops over 600 so 500 is a safe number for Portenta   300 for TBeam
//SYS:MGR~ACT:SENDDATA~START:7~END:10~K:KEY_
std::string key="KEY_";
  for(int count=0; count<100;count++){
    std::string key1=key+tostring(count);
    CMsg m;
    //m.set(VALUE,"0123456789QWERTYUIOP[]ASDFGHJKL;ZXCVBNM,./qwertyuiop[]asdfghjkl;zxcvbnm,./0123456789QWERTYUIOP[]ASDFGHJKL;ZXCVBNM,./qwertyuiop[]asdfghjkl;zxcvbnm,");
    m.set(_VALUE,"0123456789");
    addDataMap(key1,m);
    
    writeconsoleln(count);
    delay(50);
  }
}


void CSystemMgr::ir(CMsg &msg){
  writeconsoleln("ENTER void CSystemMgr::ir(CMsg &msg)");  
  std::string s=msg.get(_VALUE); 
  
  enablePin(_PINOBCI2C);  
  Wire2.begin();
  
  CSystemObject *pSys=getSystem(s.c_str(),"Test IR");
  if(pSys!=NULL) {    
    pSys->setState(_BLANK);
    pSys->run(250);
  }
  else {
    writeconsole("System not found: "); writeconsoleln(s);
  }  
}

void CSystemMgr::temp(CMsg &msg){
  std::string s=msg.get(_VALUE); 
  writeconsoleln("ENTER void CSystemMgr::temp(CMsg &msg)");  
  
  enablePin(_PINOBCI2C);  
  
  CSystemObject *pSys =(CSystemObject *)getSystem(s.c_str(),"Temp");
  
  if(pSys!=NULL) {    
    pSys->run(150);
  }
  else  { writeconsole("System not found: "); writeconsoleln(s);
  }
    
}

//TO:ADR1~SYS:MANAGER~ACT:CHKTEMPERATURE
void CSystemMgr::chkTemperature(CMsg &msg){
  CMsg m;
    
  writeconsoleln("ENTER chkTemperature");  
  
  enablePin(_PINOBCI2C);  
  

  const char * temps [] =  {_TEMPX1,_TEMPX2,_TEMPY1,_TEMPY2,_TEMPZ1,_TEMPZ2,_TEMPOBC};
  
  for(auto s:temps){
    writeconsoleln(s);
    CSystemObject *pSys=getSystem(s,s);  
    if(pSys!=NULL) {    
      unsigned int n=(unsigned int)pSys;
			writeconsole("Loop "); writeconsole(name());  writeconsole("     ");writeconsoleln((long) n);
      pSys->run(250L);                 
    }    
    else {
      writeconsole("System not found: "); writeconsoleln(s);
    }
  }
  
  
  m=getDataMap(_SATINFO);
  addTransmitList(m);  
}

void CSystemMgr::runSystem(CMsg &msg){
  writeconsoleln("ENTER void CSystemMgr::runSystem(CMsg &msg)");  
  std::string s=msg.get(_VALUE); 
  
  CSystemObject *pSys=getSystem(s.c_str(),"runSystem");
  if(pSys!=NULL) {
    writeconsoleln("runSystem");    
    pSys->run(msg);
  }
  else {
    writeconsole("System not found: "); writeconsoleln(s);
  }
}

void CSystemMgr::chkIRArrays(CMsg &msg){
  writeconsoleln("void chkIRArrays"); 
   
  enablePin(_PINOBCI2C);  

  const char * temps [] =  {_IRX1,_IRX2,_IRY1,_IRY2,_IRZ1,_IRZ2};

  for(int x=0;x<2;x++){  //First time setup  //Second time data
    for(auto s:temps){
      writeconsoleln(s);
      CSystemObject *pSys=getSystem(s,"chkIRArrays");  
      if(pSys!=NULL)   {
        
        pSys->run(300);        
      }
      else {
        writeconsole("System not found: "); writeconsoleln(s);
      }
    }
  }
}

void CSystemMgr::chkMags(CMsg &msg){
  writeconsoleln("void chkMags"); 
   
  enablePin(_PIN65V);     //Need these   Do in this order
  delay(100);
  enablePin(_PINOBCI2C);  

  delay(100);

  const char * temps [] =  {_MAGX,_MAGY,_MAGZ};

  for(int x=0;x<3;x++){  //First time setup  //Second time data
    
    std::string s=temps[x];
    writeconsoleln(s);
    
    CSystemObject *pSys=getSystem(s.c_str(),"Test Mag");
    if(pSys!=NULL) {   
      writeconsoleln("xxxxxxxxxxxxxxxxxxxxxxxxxxx") ;
      writeconsoleln(s);
      writeconsoleln(pSys->name());
      pSys->test(msg);
    }
    else{
      
      writeconsole("System not found: "); writeconsoleln(s);
    }
  

  }
  //disablePin(_PIN65V);
  //disablePin(_PINADCSI2C);
  
}



void CSystemMgr::loopWire(CMsg &msg){
  std::string s=msg.get(_VALUE,"2"); 

  if(s.size()==0)
    return;
  char c=s[0];

  loopWire(c);
}


#if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
void CSystemMgr::initI2CMap(){  
  I2CMap["20_51"]="2> IR X1 - 0x33/51";
  I2CMap["0_50"]="2> IR X2 - 0x32/50";
  I2CMap["1_51"]="2> IR Y1 - 0x33/51";
  I2CMap["1_50"]="2> IR Y2 - 0x32/50";
  I2CMap["2_51"]="2> IR Z1 - 0x33/51";
  I2CMap["2_50"]="2> IR Z2 - 0x32/50";
  
  I2CMap["0_74"]="0>Temp_OBC - 0x4A/74";
  I2CMap["0_75"]="0>IMU_ADCS - 0x4B/75";

  I2CMap["0_73"]="2> Temp X1 - 0x49/73";
  I2CMap["0_72"]="2> Temp X - 0x48/72";
  I2CMap["1_73"]="2> Temp Y1 - 0x49/73";
  I2CMap["1_72"]="2> Temp Y - 0x48/72";
  I2CMap["2_73"]="2> Temp Z1 - 0x49/73";
  I2CMap["2_72"]="2> Temp Z - 0x48/72";

  I2CMap["1_96"]="1> 96 ??";  
  I2CMap["1_54"]="1> 54 ??";
  I2CMap["1_8"]=" 1> 8 ??";
  
  I2CMap["2_99"]="2> Mag Z - 0x63/99";
  I2CMap["2_97"]="2> Mag Y - 0x61/97";
  I2CMap["2_96"]="2> Mag X - 0x60/96";
  

};

#else

void CSystemMgr::initI2CMap(){  
  I2CMap["0_51"]="2> IR X1 - 0x33/51";
  I2CMap["0_50"]="2> IR X2 - 0x32/50";
  I2CMap["1_51"]="2> IR Y1 - 0x33/51";
  I2CMap["1_50"]="2> IR Y2 - 0x32/50";
  I2CMap["2_51"]="2> IR Z1 - 0x33/51";
  I2CMap["2_50"]="2> IR Z2 - 0x32/50";
  
  I2CMap["0_74"]="0>Temp_OBC - 0x4A/74";
  I2CMap["0_75"]="0>IMU_ADCS - 0x4B/75";

  I2CMap["0_73"]="2> Temp X1 - 0x49/73";
  I2CMap["0_72"]="2> Temp X - 0x48/72";
  I2CMap["1_73"]="2> Temp Y1 - 0x49/73";
  I2CMap["1_72"]="2> Temp Y - 0x48/72";
  I2CMap["2_73"]="2> Temp Z1 - 0x49/73";
  I2CMap["2_72"]="2> Temp Z - 0x48/72";

  I2CMap["1_96"]="1> 96 ??";  
  I2CMap["1_54"]="1> 54 ??";
  I2CMap["1_8"]="1> 8 ??";
  
  I2CMap["2_99"]="2> Mag Z - 0x63/99";
  I2CMap["2_97"]="2> Mag Y - 0x61/97";
  I2CMap["2_96"]="2> Mag X - 0x60/96";
  

};

#endif

void CSystemMgr::loopWire(char s) {  //I2C2 Sensitive to VOltage   Needs 3.4V sometimes
  TwoWire *wire=NULL;

  if (s=='0') wire=&Wire;
  if (s=='1') wire=&Wire1;
  if (s=='2') wire=&Wire2;

  if(wire==NULL)
    return;

  
  if(I2CMap.size()==0)
    initI2CMap();

 
  
  byte error;
  int nDevices=0;  
  enablePin(_PINOBCI2C);  
  //enable65V();  
  
  CMsg m;
  
  wire->begin();
  delay(200);
  for(byte address = 1; address < 127; address++ ) {
    

    writeconsole(".");
    
    wire->beginTransmission(address);
    
    error = wire->endTransmission();
    
    if (error == 0) {      
      std::string str;
      std::string strI,strIM;
      strI=tostring((int)address);
      str+=s;
      str+="_";
      str+=strI;
      
      strIM=I2CMap[str];
    
      m.set(str,strIM);   
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
      m.set(pstr,address);
    }    
  }
  if (nDevices == 0) {
    m.set(_INFO,"No devices found.");
  }
 
  writeconsoleln(".");
  m.set(_NAME,"LoopWire Scanning");  
  m.writetoconsole();
  
  addTransmitList(m);
  //disable65V();  
  delay(150);          
}




void CSystemMgr::sendBeacon(CMsg &msg){  
  CMsg m;
  //msg1.set(STATE,getSatState());
  m.set(_NAME,_BEACON);
  m.set(_VALUE,IAM());
  m.set(_TIME,getTime());

  CMsg mm=getDataMap(_SYSPOWER);
  
  m.set(BatteryVolt,mm.get(BatteryVolt));
  m.set(BatteryCrnt,mm.get(BatteryCrnt));
  m.set(BatTemp1,mm.get(BatTemp1));
  m.set(XVolt,mm.get(XVolt));
  m.set(YVolt,mm.get(YVolt));
  m.set(ZVolt,mm.get(ZVolt));

  CSystemObject *pSysR=getSystem(_RADIO);
  CSystemObject *pSysR2=getSystem(_RADIO2);
  if(pSysR!=NULL){
    m.set(R_STATE,pSysR->state());
  }
  
  if(pSysR2!=NULL){
    m.set(R2_STATE,pSysR2->state());
  }

  addTransmitList(m);
}
