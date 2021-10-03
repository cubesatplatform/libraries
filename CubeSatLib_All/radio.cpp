
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



volatile bool receivedFlag = false;  // flag to indicate that a packet was sent or received
volatile bool enableInterrupt = true;  // disable interrupt when it's not needed

volatile bool receivedFlag2 = false;  // flag to indicate that a packet was sent or received
volatile bool enableInterrupt2 = true;  // disable interrupt when it's not needed


volatile bool EnableInterrupt(std::string Name="RADIO"){
  if(Name=="RADIO2") return enableInterrupt;
  return enableInterrupt;
  }
volatile void EnableInterrupt(bool tmp,std::string Name="RADIO"){
  if(Name=="RADIO2") enableInterrupt2=tmp;
  else enableInterrupt=tmp;
  }

volatile bool ReceivedFlag(std::string Name="RADIO"){
  if(Name=="RADIO2") return receivedFlag2;
  return receivedFlag;
  }
volatile void ReceivedFlag(bool tmp,std::string Name="RADIO"){
  if(Name=="RADIO2") receivedFlag2=tmp;
  else  receivedFlag=tmp;
  }

void setFlag(void) {  // this function is called when a complete packet is received
  // check if the interrupt is enabled
  //writeconsole("SetFlag called ... ");
  if(!EnableInterrupt()) {
    return;
  }

  // we sent or received a packet, set the flag
  ReceivedFlag(true);
}


void setFlag2(void) {  // this function is called when a complete packet is received
  // check if the interrupt is enabled
  //writeconsole("SetFlag called ... ");
  if(!EnableInterrupt(std::string("RADIO2"))) {
    return;
  }

  // we sent or received a packet, set the flag
  ReceivedFlag(true,std::string("RADIO2"));
}


// function to set RF mode to transmit or receive
void CRadio::setRfMode(bool transmit) {
  //#if !defined(ESP32_GATEWAY)
  #ifdef PORTENTA 
  if(transmit) {
    digitalWrite(R1_RXEN, LOW);
    digitalWrite(R1_TXEN, HIGH);
  } else {
    digitalWrite(R1_RXEN, HIGH);
    digitalWrite(R1_TXEN, LOW);
  }
  delay(20);  //100
  #endif
}

void CRadio::logTransmit(){
   writeconsoleln("");
  writeconsoleln("");

  unsigned long tot=0;
  transmitLog.push_back(getTime());
  while(transmitLog.size()>10)  transmitLog.pop_front();


  for(auto x:transmitLog){
    tot+=x;
  }
  tot/=10;

  writeconsoleln(tot);

  if(getTime()>(tot+25000))
    _delayTransmit=500;
  else  
    _delayTransmit=5000;

}

void CRadio::setup() {
   #ifdef PORTENTA 
  pinMode(R1_RXEN, OUTPUT);
  pinMode(R1_TXEN, OUTPUT);
   #endif

  // initialize the radio
  writeconsole("Initializing ... ");
  
  int state;

  for (int retries=0;retries<5;retries++){
    #ifdef TTGO
      //int state = lora.begin();
        writeconsole("TTGO ... ");
        //state = lora.begin(LORAFREQUENCY, BANDWIDTH, SPREADING_FACTOR);// , CODING_RATE, SYNC_WORD, OUTPUT_POWER, CURRENT_LIMIT, PREAMBLE_LENGTH);
        state = lora.begin();
    #elif ESP32_GATEWAY
        state = lora.begin(LORAFREQUENCY, BANDWIDTH, SPREADING_FACTOR, CODING_RATE, SYNC_WORD, OUTPUT_POWER, CURRENT_LIMIT, PREAMBLE_LENGTH);  
    #else
        //state =  lora.begin(LORAFREQUENCY, BANDWIDTH, SPREADING_FACTOR, CODING_RATE, SYNC_WORD, OUTPUT_POWER, CURRENT_LIMIT, PREAMBLE_LENGTH, TCXO_VOLTAGE);
        state =  lora.begin();
    #endif

    if(state == ERR_NONE) {
      writeconsoleln("success!");
      setState("READY");
        // set the function that will be called when packet transmission or reception is finished
      if(Name()=="RADIO2"){
      #ifdef TTGO
        lora.setDio0Action(setFlag2);
      #elif ESP32_GATEWAY
        lora.setDio0Action(setFlag2);
      #else
        lora.setDio1Action(setFlag2);
      #endif
      }
      else {
      #ifdef TTGO
        lora.setDio0Action(setFlag);
      #elif ESP32_GATEWAY
        lora.setDio0Action(setFlag);
      #else
        lora.setDio1Action(setFlag);
      #endif
      }

    
      break;
      } else {
        writeconsole("Start radio failed, code ");
        writeconsoleln(state);
        delay(2000);
        setState("ERROR");
        
      }


  }
  SetRadioReceive();
} 


void CRadio::SendAck(std::string str){
  if(_sendACK){
    writeconsole(" ACK ACK ACK ");
    strAck="ACK:"+str;
    }
}


void CRadio::TransmitPacket(CMsg &m){ 
  //Serial.println(F("----------------------------------------------- TransmitPacket(CMessage m) "));
  if(m.vectorLen()) {
      TransmitPacket(m.vectorData(),m.vectorLen());
      writeconsoleln("----------------------------------------------- TransmitVVVVEctor(CMessage m) ");
  }
  else  {
    CMsg tm=m;
    tm.serialize();
    TransmitPacket(String(tm.TransmitData().c_str()));
  }

}


void CRadio::TransmitPacket(String str="ACK"){
  
  TransmitPacket((const unsigned char *)str.c_str(),str.length());

}

void CRadio::TransmitPacket(const unsigned char *buf, int len){
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

  transmissionState = lora.startTransmit((uint8_t*)buffer,len);
  //transmissionState = lora.transmit("HELLO");
  logTransmit();
  completedTransmit=getTime()+500+len*15;
  nextTransmit=getTime()+_delayTransmit+len*15;  //lora.getTimeOnAir(len)+100;   //Doesnt exist for SX172X
  lastPing = getTime();
  _Mode="TX";
}



//Packet received.  Place in Rcvd queue.
void CRadio::ReceivedPacket(){
      tic();
       writeconsoleln("Received packet!");

      unsigned char buffer[300]={NULL};
      //int state = lora.readData(str);
      int len=lora.getPacketLength();
      int state = lora.readData(buffer,len);
      if(state == ERR_NONE) {
       
        // print RSSI (Received Signal Strength Indicator)
        writeconsole("RSSI:\t\t");
        writeconsole(lora.getRSSI());
        writeconsole(" dBm              ");
 
        // print SNR (Signal-to-Noise Ratio)
        writeconsole("SNR:\t\t");
        writeconsole(lora.getSNR());
        writeconsoleln(" dB");
  
        

        std::string tmpstr; 
        for (int count=0;count<len;count++) tmpstr+=buffer[count];              //Convert byte buffer to string
        // print data
        
        writeconsole("Data:\t\t");
        writeconsoleln(tmpstr.c_str());
  
        CMsg robj(tmpstr.c_str(), loraR.getRSSI(), loraR.getSNR());
      
      pMsgs->ReceivedList.push_back(robj);
      if ((robj.Parameters["SYS"]=="ACK")||(tmpstr.substr(0,3)=="ACK")) {}   //Don't ACK an ACK
      else  SendAck(robj.CID());
 

      } else if (state == ERR_CRC_MISMATCH) {        
        writeconsoleln("CRC error!"); 
 
      } else {
        
        writeconsole("failed, code "); // some other error occurred
        writeconsoleln(state);
 
      }
     lastPing = getTime();  //Added new to push out Ack send -----------------------------------------------------
    nextTransmit=getTime()+2000;

}

void CRadio::TransmitCmd(){
  
    CMsg tObj = pMsgs->TransmitList.front();
    pMsgs->TransmitList.pop_front();
      
    TransmitPacket(tObj);
    pMsgs->TransmittedList.push_back(tObj);
  }



void CRadio::SetRadioReceive(){
  setRfMode(false);
  ReceivedFlag(false,Name());
  
  

  int state = lora.startReceive();
  
  if (state == ERR_NONE) {    
    _Mode="RX";
    writeconsoleln(" Listening Started successfully!");
     EnableInterrupt(true,Name());
  } else {
    writeconsole(" Failed to start reception, code ");
    writeconsoleln(state); 
  }
}




void CRadio::loopRadio(){

  checkMode();
  
  if(ReceivedFlag(Name())) { // check if the previous operation finished     this is set by interrupt    
    enableInterrupt = false; // disable the interrupt service routine while processing the data      
    ReceivedFlag(false,Name());
    ReceivedPacket();
    SetRadioReceive();
    return;
  }
  
  if (readyToTransmit()) {  // check if it's time to transmit a ping packet
  if(isAck()){ 
    TransmitPacket(String(strAck.c_str()));
    resetAck();
  }
  else  TransmitCmd();      
  }

}


void CRadio:: checkMode(){
  if (_Mode=="TX"){
    if(getTime()>completedTransmit){
       writeconsoleln(" CheckMode  Transmission should be done!");
      SetRadioReceive();
    }
  }
}


bool CRadio::readyToTransmit(){

  if (getTime()<nextTransmit) return false;  //Not yet time to transmit
  if ((pMsgs->TransmitList.size()<1) &&!isAck()) return false;
  return true;

}


void CRadio::loop() {
  //writeconsoleln("xx");
  loopRadio();
}
