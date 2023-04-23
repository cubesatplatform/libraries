#include "radio.h"
#include <funcs.h>

/* 
////DANGER  PIN 23 TTGO USED BY RADIO!!!!

#define RADIO1_NSS PinNameToIndex(PC_13)
#define RADIO1_BUSY PinNameToIndex(PD_4)
#define RADIO1_RST PinNameToIndex(PC_15)
#define RADIO1_DIO1 PinNameToIndex(PE_3)

#define RADIO2_NSS PinNameToIndex(PI_4)
#define RADIO2_BUSY PinNameToIndex(PH_10)
#define RADIO2_RST PinNameToIndex(PH_14)
#define RADIO2_DIO1 PinNameToIndex(PI_5)
*/



volatile bool bFlag = false;  // flag to indicate that a packet was sent or received
volatile bool enableInterrupt = true;  // disable interrupt when it's not needed

volatile bool bFlag2 = false;  // flag to indicate that a packet was sent or received
volatile bool enableInterrupt2 = true;  // disable interrupt when it's not needed

#if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
  ICACHE_RAM_ATTR 
#endif
void setFlag(void) {  // this function is called when a complete packet is received or when a transmission is finished
  // check if the interrupt is enabled 
  if(!enableInterrupt) {    
    return;
  }  

  // we sent or received a packet, set the flag
  bFlag=true;
}


#if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
  ICACHE_RAM_ATTR 
#endif
void setFlag2(void) {  // this function is called when a complete packet is received
  // check if the interrupt is enabled  
  if(!enableInterrupt2) {
    return;
  }

  // we sent or received a packet, set the flag
  bFlag2=true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CRadio::CRadio():CSystemObject() {
  _name=_RADIO;
  init();
  };

void CRadio::init(){
  CSystemObject::init();
  setForever();
  setInterval(10);
  setErrorThreshold(5);


  if(name()==_RADIO){
    _pbFlag=&bFlag;
    _penableInterrupt=&enableInterrupt;
    plora=&radio1;
  }
  
  else{
    _pbFlag=&bFlag2;
    _penableInterrupt=&enableInterrupt2;
    #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
    plora=&radio2;
    #endif
  }

}






  void CRadio::sleep(bool tmp){
    if(tmp){
      plora->standby();
      _sleepTime=getTime();
    }
    else  
      clearSleep();
  } 
  
  bool CRadio::stillSleeping(){
    if(_sleepTime){
      if (getTime()>_sleepTime+SLEEPTIME)  
        clearSleep();
      }
    if(_sleepTime==0)
      return false;
    return true;
  }
  
  void CRadio::clearSleep(){
    _sleepTime=0;
    SetRadioReceive();
    writeconsoleln("   Clear Sleep");
  }








/*
// controlled via two pins (RX enable, TX enable)
// to enable automatic control of the switch,
// call the following method
// RX enable: 4   R1_RXEN
// TX enable: 5   R1_TXEN

//radio.setRfSwitchPins(R1_RXEN, R1_TXEN);


// function to set RF mode to transmit or receive
void CRadio::setRfMode(bool transmit) {   //True is transmit

  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
  if(name()==RADIO){
    if(transmit) {
      digitalWrite(R1_RXEN, LOW);
      digitalWrite(R1_TXEN, HIGH);
    } else {
      digitalWrite(R1_TXEN, LOW);
      digitalWrite(R1_RXEN, HIGH);      
    }
  }

  
 else{
    if(transmit) {
      digitalWrite(R2_RXEN, LOW);
      digitalWrite(R2_TXEN, HIGH);
    } else {
      digitalWrite(R2_TXEN, LOW);
      digitalWrite(R2_RXEN, HIGH);      
    }
  }
  delay(20);  //100
  #endif
}

void CRadio::enableRX(){
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
  plora->standby();
  delay(20);
  if(name()==RADIO){
    digitalWrite(R1_TXEN, LOW);
    delay(20);
    digitalWrite(R1_RXEN, HIGH);   
    delay(20);
  }
  else{
    digitalWrite(R2_TXEN, LOW);
    delay(20);
    digitalWrite(R2_RXEN, HIGH);    
    delay(20);
  }
  #endif
}


void CRadio::enableTX(){  
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
  plora->standby();
  delay(20);
  if(name()==RADIO){    
    digitalWrite(R1_RXEN, LOW);
    delay(20);
    digitalWrite(R1_TXEN, HIGH);
    delay(20);
  }
  else{
    digitalWrite(R2_RXEN, LOW);
    delay(20);
    digitalWrite(R2_TXEN, HIGH);
    delay(20);
  }
  #endif
}

*/

void  CRadio::update(CMsg &msg){
  CSystemObject::update(msg);
  writeconsoleln("Radio update Parameters ..............");
  int power=0, bw=0, sf=0;
  power =msg.get(_SYSPOWER,0);
  bw =msg.get(_BW,0);
  sf =msg.get(_SF,0);
  if(power>0) {
    
    writeconsole("Power :");
    writeconsoleln(plora->setOutputPower(power));
  }
  
  if(bw>0) {
    writeconsole("BW :");
    writeconsoleln(plora->setBandwidth(bw));
  }
  if(sf>0) {
    writeconsole("SF :");
    writeconsoleln(plora->setSpreadingFactor(sf));
  }
} 


void CRadio::setup() {
  init();

  float freq= LORA_RADIO_FREQUENCY;


  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
    writeconsole(name());writeconsole("   Initializing Radio on Portenta... Freq:");writeconsoleln(freq);
  #else    
    writeconsoleln("Initializing Radio on new TTGO ... ");
  #endif


  for (int retries=0;retries<15;retries++){
    #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
 
    writeconsole("CUBESAT ... "); writeconsoleln(LORACHIP);
    //state =  plora->begin(float freq = 434.0, float bw = 125.0, uint8_t sf = 9, uint8_t cr = 7, uint8_t syncWord = RADIOLIB_SX126X_SYNC_WORD_PRIVATE, int8_t power = 10, uint16_t preambleLength = 8, float tcxoVoltage = 1.6, bool useRegulatorLDO = false);   
    _radioState =  plora->begin(getFrequency(), getBW(),  getSF(),  getCR(),  RADIOLIB_SX127X_SYNC_WORD  , LORA_OUTPUT_POWER,  LORA_PREAMBLE_LENGTH,  2.4,  false);   //RADIOLIB_SX126X_SYNC_WORD_PRIVATE
    if (plora->setTCXO(2.4) == RADIOLIB_ERR_INVALID_TCXO_VOLTAGE)    {      writeconsoleln(F("Selected TCXO voltage is invalid for this module!"));    }

    #elif defined(ESP32_GATEWAY)
    writeconsole("GATEWAY ... "); writeconsoleln(LORACHIP);    
    _radioState =  plora->begin(getFrequency(), getBW(),  getSF(),  getCR(),  LORA_CODING_RATE, RADIOLIB_SX127X_SYNC_WORD, LORA_OUTPUT_POWER_TTGO,  LORA_PREAMBLE_LENGTH);
    
    #else    
    writeconsole("TTGO ... "); writeconsoleln(LORACHIP);
    _radioState =  plora->begin(getFrequency(), getBW(),  getSF(),  getCR(), RADIOLIB_SX127X_SYNC_WORD, LORA_OUTPUT_POWER_TTGO,  LORA_PREAMBLE_LENGTH);    
    #endif

    _modemChangedOn=getTime();
    if(_radioState == RADIOLIB_ERR_NONE) {
      
    
        // set the function that will be called when packet transmission or reception is finished   
      setActions();

      setState(_PLAY);
      writeconsole("Radio success!   success!   success!   success!   success!   success!   Time on Air:");
      #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
        writeconsole((int) plora->getTimeOnAir(250));
      #endif

      delay(1000);
      SetRadioReceive();          
     return;
      } else {
        writeconsole("Start radio failed, code ");        writeconsole(_radioState);  writeconsoleln(errorDescription(_radioState));  
        delay(2000);        
      }

  delay(50);
  }
  setState(_ERROR);
} 



void CRadio::setActions(){
  if(name()==_RADIO2){
  #if defined(TTGO1278)
    plora->setDio0Action(setFlag2);
  #elif defined(TTGO1262)
    plora->setDio1Action(setFlag2);
  #elif defined(TTGO1268)
    plora->setDio1Action(setFlag2);
   
  #elif defined(ESP32_GATEWAY)
    plora->setDio0Action(setFlag2);
  #else
    plora->setDio1Action(setFlag2);
   // plora->setRfSwitchPins(R2_RXEN, R2_TXEN);
  #endif
  }
  else {
  #if defined(TTGO1278)
    plora->setDio0Action(setFlag);
  #elif defined(TTGO1262) 
    plora->setDio1Action(setFlag);    
  #elif  defined(TTGO1268)
    plora->setDio1Action(setFlag);    
  #elif defined(ESP32_GATEWAY)
    plora->setDio0Action(setFlag);
  #else
    plora->setDio1Action(setFlag);
   // plora->setRfSwitchPins(R1_RXEN, R1_TXEN);
  #endif
  }
}


void CRadio::SendAck(CMsg &m){
  resetAck();
  
  if(m.needACK()){
    mACK.set(_TO,m.get(_FROM));
    mACK.set(_SYS,m.get(_SYS));
    mACK.set(_MCID,m.get(_MCID));
    mACK.confirmACK();
    
    }
}


void CRadio::TransmitPacket(CMsg &m){   
  //if(m.vectorLen()) {       TransmitPacket(m.vectorData(),m.vectorLen());        }
  m.writetoconsoleln();
  if (0){

  }
  else  {
    CMsg tm=m;    
    TransmitPacket(tm.TransmitData(),m.needACK());
    if(m.needACK()){
      _waitForACK=true;
      _delayTransmit=RADIOWAITFORACK;
      //writeconsoleln("need ACK");
      
    }
    else
      _delayTransmit=getTXDelay();
  }

}


void CRadio::TransmitPacket(std::string str, bool bAck){ 
  writeconsole("ToTransmit: ");writeconsole(str);writeconsoleln((long) str.length());
  TransmitPacket((unsigned char *)str.c_str(),str.length(), bAck);

}

//void CRadio::TransmitPacket(const unsigned char *buf, int len, bool bAck){
  void CRadio::TransmitPacket( unsigned char *buf, int len, bool bAck){
 //unsigned char buffer[300];

  if(len>255) {
    writeconsole("ERROR ERROR Sending   Packet too big! :");    writeconsoleln(len);     
    len=254;
  }

  //memcpy(buffer,buf,len);    //Put back  removed for testing
    
  // set mode to transmission
  //~enableTX();//setRfMode(true);
  
  *_penableInterrupt=false;
  
  _m.set(_MODE,_MODETX);
  _waitForACK=bAck;
  writeconsole("Starting Transmission  :"); writeconsoleln(name());
  //_radioState = plora->startTransmit(buffer,len);  //Asynch  Comes right back and sends on its own
  _radioState = plora->startTransmit(buf,len);  //Asynch  Comes right back and sends on its own

   
  if (_radioState!=RADIOLIB_ERR_NONE){

    writeconsoleln("ERROR in Transmission  Trying again");

    _radioState = plora->startTransmit(buf,len);  //Asynch  Comes right back and sends on its own
    if (_radioState!=RADIOLIB_ERR_NONE){

      writeconsoleln("ERROR in Transmission  Second time  IncErrorCOunt");
      incErrorCount();  //Reset
    }

  }
  
  *_penableInterrupt=true;

  
  _completedTransmit=getTime()+RADIOWAITFORCOMPLETE;   //Put a time that it should have finished by
  
  _lastPing = getTime();
  _lastTransmit= getTime();  
  

}



//Packet received.  Place in Rcvd queue.

std::string CRadio::isBinary( unsigned char *buffer, int len){  
  std::string str;
  if(len<20)
    return str;    

  if(  (buffer[17]==32)&&(buffer[18]==32)&&(buffer[19]==32)  ){    //32 is space
      writeconsoleln("Definitely a Binary");  
  
    for(int count=0;count<20;count++)  {  
      char c=(char)buffer[count];      
      if (c==' ')
        break;
      str+=c;
    }

  }  
  return str;
}


void CRadio::receivedLogic( unsigned char *buffer, int len){
  std::string filename=isBinary(buffer,  len);
  std::string tmpstr; 



  if(!filename.size()){
    for (int count=0;count<len;count++) tmpstr+=buffer[count];              //Convert byte buffer to string
  }

  CMsg robj(tmpstr.c_str());  //plora->getRSSI(), plora->getSNR()
  robj.set(_RSSI,_m.get(_RSSI,0.0));
  robj.set(_SNR,_m.get(_SNR,0.0));
  writeconsole("Received a packet ...................   ");
  robj.writetoconsoleln();

  
  if(robj.get(_ACK)=="1"){// This is an acknowledgement of a requested ACK.  No need to push to reeceived list
    if((_nextTransmit>(getTime()+getTXDelay())) ||(_delayTransmit>(getTime()+getTXDelay()))){    //No need to wait longer as we got the ack
      _nextTransmit=getTime()+getTXDelay();
      _delayTransmit=getTXDelay();
      
    }
  }
  else{
    if(!filename.size()){      
      addReceivedList(robj);
    }
    else
      robj.saveFile();
    
    if(robj.needACK()&&robj.get(_TO)==getIAM())  SendAck(robj);
  }
}

void CRadio::ReceivedPacket(){
  tic();
  unsigned char buffer[500]={0};

  int len=plora->getPacketLength();
  _radioState = plora->readData(buffer,len);
  
  if(_radioState == RADIOLIB_ERR_NONE) {    
    
    float r=plora->getRSSI();
    float s=plora->getSNR();
    writeconsoleln("RSSI/SNR>");
    writeconsoleln(r);
    writeconsoleln(s);
    _m.set(_RSSI,r);
    _m.set(_SNR, s);
    receivedLogic(buffer,len);    
  } else if (_radioState == RADIOLIB_ERR_CRC_MISMATCH) {        
    writeconsoleln("CRC error!"); 

  } else {
    
    writeconsole("failed, code ");     writeconsoleln(_radioState);
  }
  _lastPing = getTime();  //Added new to push out Ack send -----------------------------------------------------
  _lastReceive=getTime();
  if(_nextTransmit<(getTime()+getTXDelay())){
    _nextTransmit=getTime()+getTXDelay();
  }

}

void CRadio::TransmitCmd(){
         
  if(MMM.TransmitList.size()){
      CMsg tObj = MMM.TransmitList.front();
      MMM.TransmitList.pop_front();
      writeconsole("List Size:");      writeconsoleln((long)MMM.TransmitList.size());
        
      TransmitPacket(tObj);
      
      MMM.TransmittedList.push_front(tObj);
      
    }  
  }



void CRadio::SetRadioReceive(){
  //~enableRX();

  *_pbFlag=false;
  

  _radioState = plora->startReceive();
  
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)   
  // writeconsole("Data Rate BPS: "); writeconsoleln(plora->getDataRate());
  #endif
  if (_radioState == RADIOLIB_ERR_NONE) {    
    _m.set(_MODE,_MODERX);
  
    writeconsoleln("Listening Started successfully!");
    if(_waitForACK){
      _nextTransmit=getTime()+_delayTransmit;
    }
    if(_nextTransmit<getTime()+getTXDelay()){
      _nextTransmit=getTime()+getTXDelay();
    }
  }
 
   else {
    writeconsole("Failed to start reception, code ");    writeconsoleln(_radioState); 
    incErrorCount();  //Reset
    return;
  }
  
  *_penableInterrupt=true;
  
}




bool CRadio::readyToTransmit(){
  long ct=getTime();
  if (!_bTransmitter) return false;
  if (_m.get(_MODE)==_MODETX) return false;
  if ((MMM.TransmitList.size()<1) &&!isAck()) return false;
  if(ct>_obj._lastDebug+100){
    _obj._lastDebug=ct;
    writeconsole("Ready to Transmit.  Get Time: ");writeconsole(getTime());writeconsole("     Next Time: ");writeconsole(_nextTransmit);writeconsole("     Delay Time: ");writeconsoleln(_delayTransmit);    
  }
  if (getTime()>_nextTransmit) {    
    return true;  //Not yet time to transmit
  }
  return false;
}
  
               
  void CRadio::callCustomFunctions(CMsg &msg){   //Calls a specific function directly
    
  
  std::string act=msg.get(_ACT);

  mapcustommsg(TransmitPacket)
  mapcustommsg(setPower)
  mapcustommsg(setModem)
  mapcustommsg(resetPower)

  /*
  if (act=="TRANSMITPACKET") TransmitPacket(msg);                     
  if (act=="POWER") setPower(msg);
  if (act=="MODEM") setModem(msg);    

  if (act=="RESETPOWER") resetPower(msg);
  */
  CSystemObject::callCustomFunctions(msg);  
  }


void CRadio::incBadInterrupt(){
  _badInterruptCount++;
  if(_badInterruptCount>500){
    writeconsoleln("Radio Interrupt did not fire.  Calling Rebooting");
    
    delay(2000);
    reboot();
    return;
  }
  writeconsoleln("Radio Interrupt did not fire.  Calling setActions");
  setActions();
  delay(1000);
}

void CRadio::checkModeTX(){   //Puts radio back to receive Mode if stuck in Transmit Mode
  if (_m.get(_MODE)==_MODETX){    
    if(*_pbFlag){
      writeconsoleln("Transmission Complete:  Flag Fired");      
      _nextTransmit=getTime()+_delayTransmit;  //Need to account for waiting for ACK in here somehow
      if(_waitForACK)
        _nextTransmit+=RADIOWAITFORACK;
      SetRadioReceive();
      writeconsoleln("   CheckMode:  Transmission Complete:  Flag Fired");
      return;
    }
    if(getTime()>_completedTransmit){
      writeconsoleln("   CheckMode:  getTime()>completedTransmit    Transmission did not fire flag.  Finishing anyway!");
      SetRadioReceive();
      writeconsoleln("   CheckMode: Timeout  Transmission Complete");
      incBadInterrupt();
      return;
    }
  }    
}

void CRadio:: checkModeRX(){   
  if(*_pbFlag) { // check if the previous operation finished     this is set by interrupt    
    *_penableInterrupt=false; // disable the interrupt service routine when processing the data      
    *_pbFlag=false;
    if (_m.get(_MODE)==_MODERX){
      if(getReceiver())  
        ReceivedPacket();
      else        
        writeconsoleln(">>>>> NOT set as Receiver ");
      }
      SetRadioReceive();
      return;
    }
    
}



void CRadio::loopRadio(){
  chkModem();
  checkModeTX();
  checkModeRX();
  

  if (readyToTransmit()) {  // check if it's time to transmit a ping packet
    if(isAck()){ 
      TransmitPacket(mACK);
      resetAck();
    }
    else  
      TransmitCmd();      
  }
}

void CRadio::loop() {  
  if (stillSleeping()){
    writeconsole("Radio is sleeping.  Time: ");writeconsoleln(getTime());
    return;
  }
  //writeconsoleln(name());
  loopRadio();  
}



/*
WARNING!!!!!
Other than Power, may need a new Begin for other parameters to actually be used!!!
Need to check!!!!
*/

void CRadio::setPower(CMsg &m){
  writeconsoleln("Set Power");
  int power=m.get(_VALUE,LORA_OUTPUT_POWER);
  plora->setOutputPower(power);
}

void CRadio::resetPower(CMsg &m){
  plora->setOutputPower(LORA_OUTPUT_POWER);  
  }

void CRadio::setModem(CMsg &m){
  writeconsoleln("--------------------------------------Set Modem---------------------------------Speed Has Changed-");
  _modem=m.get(_VALUE,_BLANK);
  _modemChangedOn=getTime();
  setState(_BLANK);  
} 


void CRadio::chkModem(){
  if((_modem==_NORMALBW)||(_modem==_BLANK)){
    return;
  }

  if(getTime()>_modemChangedOn+MODEMCHANGEMAXTIME){
    CMsg m;
    m.set(_VALUE,_BLANK);
    setModem(m);
    writeconsoleln("Times up  Set Modem to Blank");  
  }
}


float CRadio::getFrequency(){
  if(name()==_RADIO2)
    return LORA_RADIO2_FREQUENCY;   

  return LORA_RADIO_FREQUENCY;
}

int CRadio::getBW(){
  
  if (_modem==_MEGABW)
    return LORA_BANDWIDTHMEGA;

  if (_modem==_ULTRABW)
    return LORA_BANDWIDTHULTRA;

  if (_modem==_HIGHBW)
    return LORA_BANDWIDTHHIGH;

  if (_modem==_MEDIUMBW)
    return LORA_BANDWIDTHMEDIUM;

  if (_modem==_LOWBW)
    return LORA_BANDWIDTHLOW;

  return LORA_BANDWIDTH;
}


int CRadio::getSF(){
    
  if (_modem==_MEGABW)
    return LORA_SPREADING_FACTORMEGA;

  if (_modem==_ULTRABW)
    return LORA_SPREADING_FACTORULTRA;

  if (_modem==_HIGHBW)
    return LORA_SPREADING_FACTORHIGH;

  if (_modem==_MEDIUMBW)
    return LORA_SPREADING_FACTORMEDIUM;

  if (_modem==_LOWBW)
    return LORA_SPREADING_FACTORLOW;

  return LORA_SPREADING_FACTOR;
}


int CRadio::getCR(){  
  if (_modem==_MEGABW)
    return LORA_CODING_RATEMEGA;

  if (_modem==_ULTRABW)
    return LORA_CODING_RATEULTRA;

  if (_modem==_HIGHBW)
    return LORA_CODING_RATEHIGH;

  if (_modem==_MEDIUMBW)
    return LORA_CODING_RATEMEDIUM;

  if (_modem==_LOWBW)
    return LORA_CODING_RATELOW;

  return LORA_CODING_RATE;
}


int CRadio::getTXDelay(){  
  if (_modem==_MEGABW)
    return LORA_TXDELAYMEGA;

  if (_modem==_ULTRABW)
    return LORA_TXDELAYULTRA;

  if (_modem==_HIGHBW)
    return LORA_TXDELAYHIGH;

  if (_modem==_MEDIUMBW)
    return LORA_TXDELAYMEDIUM;

  if (_modem==_LOWBW)
    return LORA_TXDELAYLOW;

  return LORA_TXDELAY;
}


CMsg CRadio::rawStats(CMsg &msg){

  CMsg m;
  //m=CSystemObject::rawStats(m);      Makes the message too big


  std::string n="STATS";
  n+=name();
  m.set(_NAME,n);
  
  m.set("LSTPING",_lastPing);// timestamp of the last PING packet  
  m.set("NXTX",_nextTransmit);
  
  m.set("LSTTX",_lastTransmit);
  m.set("LSTRX",_lastReceive);
  m.set("MODEM",_NORMALBW);
  
  m.set("FREQ",getFrequency());
  m.set("BW",getBW());
  m.set("CR",getCR());
  m.set("SF",getSF());

  m.set(_RSSI,_m.get(_RSSI,0.0));
  m.set(_SNR,_m.get(_SNR,0.0));
  
  return m;
}
