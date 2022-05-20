#include "radio.h"
#include <funcs.h>
#include <SPI.h>

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


void setFlag(void) {  // this function is called when a complete packet is received or when a transmission is finished
  // check if the interrupt is enabled 
  if(!enableInterrupt) {    
    return;
  }  

  // we sent or received a packet, set the flag
  bFlag=true;
}


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
  Name("radio");
  init();
  };

void CRadio::init(){
  CSystemObject::init();
  setForever();
  setInterval(10);
  setErrorThreshold(5);
  pMsgs=getMessages();

  if(Name()=="RADIO"){
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
  }











// function to set RF mode to transmit or receive
void CRadio::setRfMode(bool transmit) {   //True is transmit

  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
  if(Name()=="RADIO"){
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
  if(Name()=="RADIO"){
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
  if(Name()=="RADIO"){    
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



void  CRadio::Update(CMsg &msg){
  CSystemObject::Update(msg);
  writeconsoleln("Radio Update Parameters ..............");
  int power=0, bw=0, sf=0;
  power =msg.getParameter("POWER",0);
  bw =msg.getParameter("BW",0);
  sf =msg.getParameter("SF",0);
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
  SPI.begin();

  init();

  float freq= RADIOLORAFREQUENCY;
  if (Name()!="RADIO")
     freq= RADIO2LORAFREQUENCY;



  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
  writeconsole(Name());writeconsole("   Initializing Radio on Portenta... Freq:");writeconsoleln(freq);

  #else    
    writeconsoleln("Initializing Radio on new TTGO ... ");
    
    #ifdef TTGO1
      writeconsoleln("");      writeconsoleln("InitBoard TTGO 1 ... ");
      initBoard();
      delay(1500);
    #endif  
  #endif


  for (int retries=0;retries<15;retries++){
    #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
 
    writeconsole("CUBESAT ... "); writeconsoleln(LORACHIP);
    //state =  plora->begin(float freq = 434.0, float bw = 125.0, uint8_t sf = 9, uint8_t cr = 7, uint8_t syncWord = RADIOLIB_SX126X_SYNC_WORD_PRIVATE, int8_t power = 10, uint16_t preambleLength = 8, float tcxoVoltage = 1.6, bool useRegulatorLDO = false);

    _radioState =  plora->begin(freq, BANDWIDTH,  SPREADING_FACTOR,  CODING_RATE,  RADIOLIB_SX127X_SYNC_WORD  , OUTPUT_POWER,  PREAMBLE_LENGTH,  2.4,  false);   //RADIOLIB_SX126X_SYNC_WORD_PRIVATE
    if (plora->setTCXO(2.4) == RADIOLIB_ERR_INVALID_TCXO_VOLTAGE)    {      writeconsoleln(F("Selected TCXO voltage is invalid for this module!"));    }
    
    #elif defined(TTGO)  
    
    writeconsole("TTGO ... "); writeconsoleln(LORACHIP);

    _radioState =  plora->begin(freq, BANDWIDTH,  SPREADING_FACTOR,  CODING_RATE, RADIOLIB_SX127X_SYNC_WORD, OUTPUT_POWER_TTGO,  PREAMBLE_LENGTH);

    #elif  defined(TTGO1)
    writeconsole("TTGO1 ... "); writeconsoleln(LORACHIP);
    _radioState =  plora->begin(freq, BANDWIDTH,  SPREADING_FACTOR,  CODING_RATE, RADIOLIB_SX127X_SYNC_WORD, OUTPUT_POWER_TTGO,  PREAMBLE_LENGTH);
      
    #elif defined(ESP32_GATEWAY)
    writeconsole("GATEWAY ... "); writeconsoleln(LORACHIP);    
    _radioState =  plora->begin(434.0, BANDWIDTH,  SPREADING_FACTOR,  CODING_RATE, RADIOLIB_SX127X_SYNC_WORD, OUTPUT_POWER_TTGO,  PREAMBLE_LENGTH);
    #endif

    if(_radioState == RADIOLIB_ERR_NONE) {
      
    
        // set the function that will be called when packet transmission or reception is finished   
      if(Name()=="RADIO2"){
      #if defined(TTGO)
        plora->setDio0Action(setFlag2);
      #elif defined(TTGO1)
        plora->setDio1Action(setFlag2);
      #elif defined(ESP32_GATEWAY)
        plora->setDio0Action(setFlag2);
      #else
        plora->setDio1Action(setFlag2);
      #endif
      }
      else {
      #if defined(TTGO)
        plora->setDio0Action(setFlag);
      #elif defined(TTGO1)
        plora->setDio1Action(setFlag);
      #elif defined(ESP32_GATEWAY)
        plora->setDio0Action(setFlag);
      #else
        plora->setDio1Action(setFlag);
      #endif
      }

      setState("PLAY");
      writeconsoleln("Radio success!   success!   success!   success!   success!   success!   success!   success!    success!   success!   success!");
      delay(3000);
      SetRadioReceive();          
     return;
      } else {
        writeconsole("Start radio failed, code ");        writeconsoleln(_radioState);
        delay(2000);        
      }

  delay(50);
  }
  setState("ERROR");
} 



void CRadio::SendAck(CMsg &m){
  resetAck();
  
  if(m.needACK()){
    mACK.setTO(m.getFROM());
    mACK.setSYS(m.getSYS());
    mACK.setCID(m.getCID());
    mACK.confirmACK();
    
    }
}


void CRadio::TransmitPacket(CMsg &m){ 
  writeconsoleln("----------------------------------------------- TransmitPacket(CMessage m) ------------------------------------------");
  if(m.vectorLen()) {
      TransmitPacket(m.vectorData(),m.vectorLen());
      writeconsoleln("----------------------------------------------- TransmitVVVVEctor(CMessage m) ------------------------------------------");
  }
  else  {
    CMsg tm=m;    
    TransmitPacket(tm.TransmitData(),m.needACK());
    if(m.needACK()){
      _waitForACK=true;
      _delayTransmit=RADIOWAITFORACK;
      writeconsoleln("----------------------------------------------- need ACK ---------------------------------------------------- ");
      
    }
    else
      _delayTransmit=RADIOTXDELAY;
  }

}


void CRadio::TransmitPacket(std::string str, bool bAck){ 
  TransmitPacket((const unsigned char *)str.c_str(),str.length(), bAck);

}

void CRadio::TransmitPacket(const unsigned char *buf, int len, bool bAck){
  unsigned char buffer[300];

  writeconsole("Radio Transmitting :  ");writeconsoleln(Name());

  if(len>255) {
    writeconsole("ERROR ERROR Sending   Packet too big! :");    writeconsoleln(len);    
   
    len=254;
  }

  memcpy(buffer,buf,254);
    
  // set mode to transmission
  enableTX();//setRfMode(true);

  
  *_penableInterrupt=false;
  

  setMode("TX");
  _waitForACK=bAck;
  _radioState = plora->startTransmit((uint8_t*)buffer,len);  //Asynch  Comes right back and sends on its own
  if (_radioState!=RADIOLIB_ERR_NONE){
    writeconsoleln("ERROR in Transmission");
    incErrorCount();  //Reset
    return;
  }
  
  *_penableInterrupt=true;
  
  _completedTransmit=getTime()+RADIOWAITFORCOMPLETE;   //Put a time that it should have finished by
  _lastPing = getTime();
  _lastTransmit= getTime();
  
}



//Packet received.  Place in Rcvd queue.
void CRadio::receivedLogic(unsigned char *buffer, int len){
  std::string tmpstr; 
  for (int count=0;count<len;count++) tmpstr+=buffer[count];              //Convert byte buffer to string

  CMsg robj(tmpstr.c_str(), plora->getRSSI(), plora->getSNR());
  robj.setParameter("RSSI",plora->getRSSI());
  robj.setParameter("S/N",plora->getSNR());
  writeconsoleln("------------------------------RADIO MSG--------------------------------");
  writeconsoleln(robj.serializeout());
  writeconsoleln("------------------------------RADIO MSG END--------------------------------");
  if(robj.getACK()=="1"){// This is an acknowledgement of a requested ACK.  No need to push to reeceived list
    if((_nextTransmit>(getTime()+RADIOTXDELAY)) ||(_delayTransmit>(getTime()+RADIOTXDELAY))){    //No need to wait longer as we got the ack
      _nextTransmit=getTime()+RADIOTXDELAY;
      _delayTransmit=RADIOTXDELAY;
      
    }
  }
  else{
    addReceivedList(robj);
    
    if(robj.needACK()&&robj.getTO()==getIAM())  SendAck(robj);
  }
  //writeconsoleln("Received Logic End");
}

void CRadio::ReceivedPacket(){
  tic();
  writeconsoleln("Received packet!");

  unsigned char buffer[300]={0};

  int len=plora->getPacketLength();
  _radioState = plora->readData(buffer,len);
  if(_radioState == RADIOLIB_ERR_NONE) {    
    receivedLogic(buffer,len);    
  } else if (_radioState == RADIOLIB_ERR_CRC_MISMATCH) {        
    writeconsoleln("CRC error!"); 

  } else {
    
    writeconsole("failed, code ");     writeconsoleln(_radioState);
  }
  _lastPing = getTime();  //Added new to push out Ack send -----------------------------------------------------
  _lastReceive=getTime();
  if(_nextTransmit<(getTime()+RADIOTXDELAY)){
    _nextTransmit=getTime()+RADIOTXDELAY;
  }

}

void CRadio::TransmitCmd(){
         
  if(pMsgs->TransmitList.size()){
      CMsg tObj = pMsgs->TransmitList.front();
      pMsgs->TransmitList.pop_front();
      writeconsole("List Size:");      writeconsoleln((long)pMsgs->TransmitList.size());
        
      TransmitPacket(tObj);
      pMsgs->TransmittedList.push_front(tObj);
    }  
  }



void CRadio::SetRadioReceive(){
  enableRX();

  *_pbFlag=false;
  

  _radioState = plora->startReceive();
  writeconsoleln("");
  if (_radioState == RADIOLIB_ERR_NONE) {    
    setMode("RX");
  
    writeconsoleln("Listening Started successfully!____________________________");
    if(_waitForACK){
      _nextTransmit=getTime()+_delayTransmit;
    }
    if(_nextTransmit<getTime()+RADIOTXDELAY){
      _nextTransmit=getTime()+RADIOTXDELAY;
    }
  }
 
   else {
    writeconsole(" Failed to start reception, code ");    writeconsoleln(_radioState); 
    incErrorCount();  //Reset
    return;
  }
  
  *_penableInterrupt=true;
  
}




bool CRadio::readyToTransmit(){
  unsigned long ct=getTime();
  if (!_bTransmitter) return false;
  if (Mode()=="TX") return false;
  if ((pMsgs->TransmitList.size()<1) &&!isAck()) return false;
  if(ct>_lastDebug+100){
    _lastDebug=ct;
    writeconsole("Ready to Transmit.  Get Time: ");writeconsole(getTime());writeconsole("     Next Time: ");writeconsole(_nextTransmit);writeconsole("     Delay Time: ");writeconsoleln(_delayTransmit);    
  }
  if (getTime()>_nextTransmit) {    
    return true;  //Not yet time to transmit
  }
  return false;
}
  

  void CRadio::callCustomFunctions(CMsg &msg){   //Calls a specific function directly
writeconsoleln("hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh");
  writeconsoleln("void CRadio::callCustomFunctions(CMsg &msg)");
  msg.writetoconsole();
    std::string act=msg.getParameter("ACT");
    if (act=="TRANSMITPACKET") TransmitPacket(msg);                   
    
    if (act=="POWER") setPower(msg);
    if (act=="CR") setCR(msg);
    if (act=="BW") setBW(msg);
    if (act=="SF") setSF(msg);

    if (act=="RESETPARAMS") resetParams(msg);

  }



void CRadio::checkModeTX(){   //Puts radio back to receive Mode if stuck in Transmit Mode
  if (Mode()=="TX"){    
    if(*_pbFlag){
      writeconsoleln("   Transmission Complete:  Flag Fired");      
      _nextTransmit=getTime()+_delayTransmit;  //Need to account for waiting for ACK in here somehow
      if(_waitForACK)
        _nextTransmit+=RADIOWAITFORACK;
      SetRadioReceive();
      return;
    }
    if(getTime()>_completedTransmit){
      writeconsoleln("   CheckMode:  getTime()>completedTransmit    Transmission did not fire flag.  Finishing anyway!");
      SetRadioReceive();
      return;
    }
  }    
}

void CRadio:: checkModeRX(){   
  if(*_pbFlag) { // check if the previous operation finished     this is set by interrupt    
    *_penableInterrupt=false; // disable the interrupt service routine when processing the data      
    *_pbFlag=false;
    if (Mode()=="RX")
      ReceivedPacket();
    SetRadioReceive();
    return;
  }
}


void CRadio::loopRadio(){
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
  if (stillSleeping())
    return;
  //writeconsoleln(Name());
  loopRadio();  
}


void CRadio::setPower(CMsg &m){
  writeconsoleln("Set Power");
  int power=m.getParameter("VAL",OUTPUT_POWER);
  plora->setOutputPower(power);
}


void CRadio::setCR(CMsg &m){
  writeconsoleln("Set CR");
  int cr=m.getParameter("VAL",CODING_RATE);
  plora->setCodingRate(cr);
}

void CRadio::setBW(CMsg &m){
  writeconsoleln("Set BW");
  float bw=m.getParameter("VAL",BANDWIDTH);
  plora->setBandwidth(bw);
  }

void CRadio::setSF(CMsg &m){
  writeconsoleln("Set SF");
  int sf=m.getParameter("VAL",SPREADING_FACTOR);
  plora->setSpreadingFactor(sf);
  }

void CRadio::resetParams(CMsg &m){
  plora->setOutputPower(OUTPUT_POWER);
  plora->setCodingRate(CODING_RATE);
  plora->setBandwidth(BANDWIDTH);
  plora->setSpreadingFactor(SPREADING_FACTOR);
  }