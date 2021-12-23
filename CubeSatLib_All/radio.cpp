#include "radio.h"
#include "funcs.h"


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


volatile bool EnableInterrupt(std::string Name="RADIO"){
  if(Name=="RADIO2") return enableInterrupt;
  return enableInterrupt;
  }
volatile void EnableInterrupt(bool tmp,std::string Name="RADIO"){
  if(Name=="RADIO2") enableInterrupt2=tmp;
  else enableInterrupt=tmp;
  }

volatile bool SRFlag(std::string Name="RADIO"){
  if(Name=="RADIO2") return bFlag2;
  return bFlag;
  }
volatile void SRFlag(bool tmp,std::string Name="RADIO"){
  if(Name=="RADIO2") bFlag2=tmp;
  else  bFlag=tmp;
  }

void setFlag(void) {  // this function is called when a complete packet is received or when a transmission is finished
  // check if the interrupt is enabled
  writeconsoleln("");
  writeconsoleln("SetFlag Called A");
  if(!EnableInterrupt()) {
    writeconsoleln("SetFlag Called B");
    return;
  }

  writeconsoleln("SetFlag Called C");

  // we sent or received a packet, set the flag
  
  SRFlag(true);
}


void setFlag2(void) {  // this function is called when a complete packet is received
  // check if the interrupt is enabled
  writeconsoleln("SetFlag 2 Called");
  if(!EnableInterrupt(std::string("RADIO2"))) {
    return;
  }

  // we sent or received a packet, set the flag
  
  SRFlag(true,std::string("RADIO2"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CRadio::CRadio():CSystemObject() {
  Name("RADIO");
  init();
  };

void CRadio::init(){
  CSystemObject::init();
  setForever();
  setInterval(100);
  pMsgs=getMessages();

}

// function to set RF mode to transmit or receive
void CRadio::setRfMode(bool transmit) {   //True is transmit
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
  if(Name()=="RADIO"){
    if(transmit) {
      digitalWrite(R1_RXEN, LOW);
      digitalWrite(R1_TXEN, HIGH);
    } else {
      digitalWrite(R1_RXEN, HIGH);
      digitalWrite(R1_TXEN, LOW);
    }
  }

  
  if(Name()=="RADIO2"){
    if(transmit) {
      digitalWrite(R2_RXEN, LOW);
      digitalWrite(R2_TXEN, HIGH);
    } else {
      digitalWrite(R2_RXEN, HIGH);
      digitalWrite(R2_TXEN, LOW);
    }
  }
  delay(20);  //100
  #endif
}


void  CRadio::Update(CMsg &msg){
  CSystemObject::Update(msg);


} 


void CRadio::setup() {
  init();
  

  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
  writeconsole("Initializing Radio on Portenta... ");
  if(Name()=="RADIO"){
    pinMode(R1_RXEN, OUTPUT);
    pinMode(R1_TXEN, OUTPUT);
  }
  if(Name()=="RADIO2"){
    pinMode(R2_RXEN, OUTPUT);
    pinMode(R2_TXEN, OUTPUT);
    lora=radio_2;
  }
  #else    
    writeconsoleln("Initializing Radio on new TTGO ... ");
    
    #ifdef TTGO1
      writeconsoleln("");      writeconsoleln("InitBoard TTGO1111 ... ");
      initBoard();
      delay(1500);
    #endif  
    
    

  #endif


  
  int state;

  for (int retries=0;retries<5;retries++){
    #ifdef TTGO
  //int state = lora.begin();
    writeconsole("TTGO ... ");
    //state = lora.begin(LORAFREQUENCY, BANDWIDTH, SPREADING_FACTOR);// , CODING_RATE, SYNC_WORD, OUTPUT_POWER, CURRENT_LIMIT, PREAMBLE_LENGTH);
    state = lora.begin(443.0);

    #elif defined(TTGO1)
    writeconsole("TTGO1 ... ");
    //state = lora.begin(LORAFREQUENCY, BANDWIDTH, SPREADING_FACTOR);// , CODING_RATE, SYNC_WORD, OUTPUT_POWER, CURRENT_LIMIT, PREAMBLE_LENGTH);
    state = lora.begin(886.0);
      #if defined(LoRa_frequency)
          state = lora.begin(LoRa_frequency);
      #else
          state = lora.begin(868.0);
      #endif
      
    #elif defined(ESP32_GATEWAY)
        state = lora.begin(LORAFREQUENCY, BANDWIDTH, SPREADING_FACTOR, CODING_RATE, SYNC_WORD, OUTPUT_POWER, CURRENT_LIMIT, PREAMBLE_LENGTH);  
    #else
    
        //     int16_t begin(float freq = 434.0, float bw = 125.0,   uint8_t sf = 9, uint8_t cr = 7, uint8_t syncWord = SX126X_SYNC_WORD_PRIVATE, int8_t power = 10, uint16_t preambleLength = 8, float tcxoVoltage = 1.6, bool useRegulatorLDO = false);
        //state =  lora.begin      (LORAFREQUENCY,        BANDWIDTH, SPREADING_FACTOR,    CODING_RATE,                                   SYNC_WORD,      OUTPUT_POWER,             PREAMBLE_LENGTH,            TCXO_VOLTAGE);
        state =  lora.begin();
    #endif

    if(state == RADIOLIB_ERR_NONE) {
      
    
        // set the function that will be called when packet transmission or reception is finished
      if(Name()=="RADIO2"){
      #if defined(TTGO)
        lora.setDio0Action(setFlag2);
      #elif defined(TTGO1)
        lora.setDio1Action(setFlag2);
      #elif defined(ESP32_GATEWAY)
        lora.setDio0Action(setFlag2);
      #else
        lora.setDio1Action(setFlag2);
      #endif
      }
      else {
      #if defined(TTGO)
        lora.setDio0Action(setFlag);
      #elif defined(TTGO1)
        lora.setDio1Action(setFlag);
      #elif defined(ESP32_GATEWAY)
        lora.setDio0Action(setFlag);
      #else
        lora.setDio1Action(setFlag);
      #endif
      }

      setState("PLAY");
      writeconsoleln("Radio success!   success!   success!   success!   success!   success!   success!   success!    success!   success!   success!");
      delay(3000);
      SetRadioReceive();      
      
     return;
      } else {
        writeconsole("Start radio failed, code ");
        writeconsoleln(state);
        delay(2000);
      
        
      }

  delay(50);
  }
    setState("ERROR");
} 



void CRadio::SendAck(CMsg &m){
  resetAck();
  
  if(m.needACK()){
    mACK.setSAT(m.getSAT());
    mACK.setSYS(m.getSYS());
    mACK.setCMDID(m.getCMDID());
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
  char buffer[255];

  if(len>255) {
    writeconsole("ERROR ERROR Sending   Packet too big! :");
    writeconsoleln(len);    
   
    
    len=254;
  }

  memcpy(buffer,buf,254);
    
  // set mode to transmission
  setRfMode(true);
  EnableInterrupt(false,Name());

  setMode("TX");
  _waitForACK=bAck;
  transmissionState = lora.startTransmit((uint8_t*)buffer,len);  //Asynch  Comes right back and sends on its own
  if (transmissionState!=RADIOLIB_ERR_NONE){
    writeconsoleln("ERROR in Transmission");
  }
  EnableInterrupt(true,Name());
 
  completedTransmit=getTime()+RADIOWAITFORCOMPLETE;   //Put a time that it should have finished by
  lastPing = getTime();
  
}



//Packet received.  Place in Rcvd queue.
void CRadio::receivedLogic(unsigned char *buffer, int len){
  // print RSSI (Received Signal Strength Indicator)
  writeconsole("RSSI:\t\t"); writeconsole(lora.getRSSI()); writeconsole(" dBm ");

  // print SNR (Signal-to-Noise Ratio)
  writeconsole("SNR:\t\t");  writeconsole(lora.getSNR()); writeconsoleln(" dB ");        

  std::string tmpstr; 
  for (int count=0;count<len;count++) tmpstr+=buffer[count];              //Convert byte buffer to string

  // print data
  writeconsole("Data:\t\t"); writeconsoleln(tmpstr.c_str());

  CMsg robj(tmpstr.c_str(), loraR.getRSSI(), loraR.getSNR());
  if(robj.getACK()=="1"){// This is an acknowledgement of a requested ACK.  No need to push to reeceived list
    if(nextTransmit>(getTime()+RADIOTXDELAY)){    //No need to wait longer as we got the ack
      nextTransmit=getTime()+RADIOTXDELAY;
      _delayTransmit=RADIOTXDELAY;
    }
  }
  else{
    pMsgs->addReceivedList(robj, thisSat());
 // if ((robj.Parameters["SYS"]=="ACK")||(tmpstr.substr(0,3)=="ACK")) {}   //Don't ACK an ACK
  //  else  SendAck(robj.CID());
    if(robj.needACK()&&robj.getSAT().size()&&(robj.getSAT()==thisSat()))  SendAck(robj);
  }
}

void CRadio::ReceivedPacket(){
  tic();
  writeconsoleln("Received packet!");

  unsigned char buffer[300]={NULL};
  //int state = lora.readData(str);
  int len=lora.getPacketLength();
  int state = lora.readData(buffer,len);
  if(state == RADIOLIB_ERR_NONE) {    
    receivedLogic(buffer,len);    
  } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {        
    writeconsoleln("CRC error!"); 

  } else {
    
    writeconsole("failed, code "); // some other error ocurred
    writeconsoleln(state);

  }
  lastPing = getTime();  //Added new to push out Ack send -----------------------------------------------------
  if(nextTransmit<(getTime()+RADIOTXDELAY)){
    nextTransmit=getTime()+RADIOTXDELAY;
  }

}

void CRadio::TransmitCmd(){
         
  if(pMsgs->TransmitList.size()){
      CMsg tObj = pMsgs->TransmitList.front();
      pMsgs->TransmitList.pop_front();
      writeconsole("List Size:");
      writeconsoleln((long)pMsgs->TransmitList.size());
        
      TransmitPacket(tObj);
      pMsgs->TransmittedList.push_back(tObj);
    }  
  }



void CRadio::SetRadioReceive(){
  setRfMode(false);
  SRFlag(false,Name());
  
  

  int state = lora.startReceive();
  writeconsoleln("");
  if (state == RADIOLIB_ERR_NONE) {    
    setMode("RX");
    writeconsoleln(" Listening Started successfully!");
    if(_waitForACK){
      nextTransmit=getTime()+_delayTransmit;
    }
    if(nextTransmit<getTime()+RADIOTXDELAY){
      nextTransmit=getTime()+RADIOTXDELAY;
    }

    EnableInterrupt(true,Name());
  } else {
    writeconsole(" Failed to start reception, code ");
    writeconsoleln(state); 
  }
}




void CRadio::loopRadio(){

  checkMode();
  
  if(SRFlag(Name())) { // check if the previous operation finished     this is set by interrupt    
    EnableInterrupt(false,Name()); // disable the interrupt service routine while processing the data      
    SRFlag(false,Name());
    if (Mode()=="RX")
      ReceivedPacket();
    SetRadioReceive();
    return;
  }
  
  if (readyToTransmit()) {  // check if it's time to transmit a ping packet
    if(isAck()){ 
      TransmitPacket(mACK);
      resetAck();
    }
    else  
      TransmitCmd();      
  }

}


void CRadio:: checkMode(){   //Puts radio back to receive Mode if stuck in Transmit Mode
  if (Mode()=="TX"){
    if(SRFlag()){
      writeconsoleln("   Transmission Complete:  Flag Fired");      
      nextTransmit=getTime()+_delayTransmit;  //Need to account for waiting for ACK in here somehow
      if(_waitForACK)
        nextTransmit+=RADIOWAITFORACK;
      SetRadioReceive();
      return;
    }
    if(getTime()>completedTransmit){
      writeconsoleln("   CheckMode:  getTime()>completedTransmit    Transmission did not fire flag.  Finishing anyway!");
      SetRadioReceive();
      return;
    }
  }
}


bool CRadio::readyToTransmit(){
  unsigned long ct=getTime();
  if (Mode()=="TX") return false;
  if ((pMsgs->TransmitList.size()<1) &&!isAck()) return false;
  if(ct>_lastDebug+100){
    _lastDebug=ct;
    writeconsole("Ready to Transmit.  Get Time: ");writeconsole(getTime());writeconsole("     Next Time: ");writeconsole(nextTransmit);writeconsole("     Delay Time: ");writeconsoleln(_delayTransmit);    
  }
  if (getTime()>nextTransmit) {    
    return true;  //Not yet time to transmit
  }
  return false;
}


void CRadio::loop() {  
  loopRadio();
}
