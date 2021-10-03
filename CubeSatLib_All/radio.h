#pragma once

#include "defs.h"
#include <RadioLib.h>
#include "systemobject.h"
#include "messages.h"



#include <list>

#if defined(PORTENTA_E22_900M30S) || defined(PORTENTA_E22_400M30S)
#include <pinDefinitions.h>
#endif



///////////////////////////
/*
NSS & BUSY Necessary to initialize, other two are not

*/
//////////////////////////


class CRadio: public CSystemObject{
#ifdef TTGO
  SX1278 radio = new Module(TTGO_SS, TTGO_DIO0, TTGO_RST, TTGO_DIO1);
  SX1278 radio2 = new Module(TTGO_SS, TTGO_DIO0, TTGO_RST, TTGO_DIO1);
  SX1278& lora=radio;
  SX1278& loraT=radio;
  SX1278& loraR=radio;


#elif defined(ESP32_GATEWAY)
  //SPIClass spi(HSPI);
  //RFM96 lora = new Module(NSS_BUILTIN, DIO0_BUILTIN, NRST_BUILTIN, DIO1_BUILTIN, spi);
  RFM96 lora = new Module(ESP32_GATEWAY_NSS_BUILTIN, ESP32_GATEWAY_DIO0_BUILTIN, ESP32_GATEWAY_NRST_BUILTIN, ESP32_GATEWAY_DIO1_BUILTIN);
  RFM96& loraT=lora;
  RFM96& loraR=lora;
 // SX1280 lora = new Module(25, 27, 23, 19);


#elif defined(PORTENTA_E22_900M30S)
/*
  //SX1262 radio = new Module(NSS, DIO1, NRST, BUSY);
  //SX1262 lora = new Module(RADIO1_NSS, RADIO1_DIO1, RADIO1_RST, RADIO1_BUSY);
  #define NSS PinNameToIndex(PI_4) //7
#define NRST PinNameToIndex(PH_14) //3
#define BUSY PinNameToIndex(PH_10) //4
#define DIO1 PinNameToIndex(PI_5) //5

  SX1262 radio = new Module(NSS, DIO1, NRST, BUSY);
  SX1262 radio2 = new Module(NSS, DIO1, NRST, BUSY);
  SX1262& lora=radio;
  SX1262& loraT=radio;
  SX1262& loraR=radio;
*/


#elif defined(PORTENTA_E22_400M30S)
  //SX1262 radio = new Module(NSS, DIO1, NRST, BUSY);
  //SX1262 lora = new Module(RADIO1_NSS, RADIO1_DIO1, RADIO1_RST, RADIO1_BUSY);

/*
//  Worked!!!
#define NSS PinNameToIndex(PC_6) //  PI_0
#define BUSY PinNameToIndex(PC_7) //  PC_7
#define NRST PinNameToIndex(PK_1) // PG_7 
#define DIO1 PinNameToIndex(PH_15) //  PC_6 Worked
*/
/*
#define NSS PinNameToIndex(PI_0) //7
#define BUSY PinNameToIndex(PJ_11) //4
#define NRST PinNameToIndex(PG_7) //3
#define DIO1 PinNameToIndex(PH_15) //5
*/


/*  OBC Radio 2 Worked
#define NSS PinNameToIndex(PI_4) //7
#define NRST PinNameToIndex(PH_14) //3
#define BUSY PinNameToIndex(PH_10) //4
#define DIO1 PinNameToIndex(PI_5) //5
*/


//OBC Radio 1 Worked
#define NSS PinNameToIndex(PC_13) //7
#define NRST PinNameToIndex(PC_15) //3
#define BUSY PinNameToIndex(PD_4) //4
#define DIO1 PinNameToIndex(PE_3) //5
  SX1268 radio = new Module(NSS, DIO1, NRST, BUSY);
//  SX1268 radio2 = new Module(NSS, DIO1, NRST, BUSY);
  SX1268& lora=radio;
  SX1268& loraT=radio;
  SX1268& loraR=radio;
  
#endif


//SX1268 lora = new Module(NSS, DIO1, NRST, BUSY);
//SX1268& loraT=lora;
//SX1268& loraR=lora;

    std::list<unsigned long> transmitLog;
    int transmissionState = ERR_NONE;  // save transmission state between loops
    unsigned long lastPing = 0;// timestamp of the last PING packet
    
    unsigned long nextTransmit=0;
    unsigned long completedTransmit=0;
    
    std::string strAck;
    bool _sendACK=true;
    unsigned long _delayTransmit=500;
    CMessages *pMsgs;
 
  public:

    CRadio():CSystemObject() {Name("RADIO");_forever=true;pMsgs=getMessages();};
    void setMessages(CMessages *ptr){pMsgs=ptr;}
    void setRfMode(bool transmit);// function to set RF mode to transmit or receive
    void setup();
    bool isTransmitTime();
    void TransmitCmd();
    void TransmitPacket(String str);
    void TransmitPacket(const unsigned char *buf, int len);
    void TransmitPacket(CMsg &m);
    void SetRadioReceive();
    void ReceivedPacket();
    void loopRadio();
    void loop();
    void logTransmit();
    void SendAck(std::string str);
    bool isAck(){if (strAck.length()>1) return true; return false;}
    bool readyToTransmit();
    void checkMode();
    void setACK(bool tmp){_sendACK=tmp;}
    void resetAck(){strAck="";};
    void callNewFunction(CMsg &msg){   //Calls a specific function directly
      std::string act=msg.getParameter("ACT");
      if (act=="TRANSMITPACKET") TransmitPacket(msg);      
    }



};
