#pragma once

#include <radio_defs.h>
#include <RadioLib.h>
#include <systemobject.h>
#include <messages.h>



#include <list>

#if defined(PORTENTA_E22_900M30S) || defined(PORTENTA_E22_400M30S)
  #include <pinDefinitions.h>
#else
  #include <boards.h>
#endif



///////////////////////////
/*
NSS & BUSY Necessary to initialize, other two are not
*/
//////////////////////////


class CRadio: public CSystemObject{
#if defined(TTGO)
  #define LORACHIP "TTGO  SX1278"                                              //TBeam us TTGO
  SX1278 radio1 = new Module(TTGO_SS, TTGO_DIO0, TTGO_RST, TTGO_DIO1);  
  SX1278 * plora=&radio1;

  
#elif defined(TTGO1)
  #define LORACHIP "TTGO  SX1262"
  SX1262 radio1 =  new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);  
  SX1262 * plora=&radio1;

/*
#elif defined(ESP32_GATEWAY)
  //SPIClass spi(HSPI);
  //RFM96 lora = new Module(NSS_BUILTIN, DIO0_BUILTIN, NRST_BUILTIN, DIO1_BUILTIN, spi);
  RFM96 lora = new Module(ESP32_GATEWAY_NSS_BUILTIN, ESP32_GATEWAY_DIO0_BUILTIN, ESP32_GATEWAY_NRST_BUILTIN, ESP32_GATEWAY_DIO1_BUILTIN);
  RFM96& loraT=lora;
  RFM96& loraR=lora;
 // SX1280 lora = new Module(25, 27, 23, 19);


*/


#elif defined(PORTENTA_E22_400M30S)

//  Worked!!!
#define LORACHIP "PORTENTA_E22_400M30S SX1268"
#define NSS PinNameToIndex(PC_6) //  PI_0
#define BUSY PinNameToIndex(PC_7) //  PC_7
#define NRST PinNameToIndex(PK_1) // PG_7 
#define DIO1 PinNameToIndex(PH_15) //  PC_6 Worked

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
#define NSS_1 PinNameToIndex(PC_13) //7
#define NRST_1 PinNameToIndex(PC_15) //3
#define BUSY_1 PinNameToIndex(PD_4) //4
#define DIO1_1 PinNameToIndex(PE_3) //5

#define NSS_2 PinNameToIndex(PI_4) //7
#define NRST_2 PinNameToIndex(PH_14) //3
#define BUSY_2 PinNameToIndex(PH_10) //4
#define DIO1_2 PinNameToIndex(PI_5) //5
SX1268 radio1 = new Module(NSS_1, DIO1_1, NRST_1, BUSY_1);
SX1268 radio2 = new Module(NSS_2, DIO1_2, NRST_2, BUSY_2);

SX1268 *plora=&radio1;

  
#endif

  std::list<unsigned long> transmitLog;
  int _radioState = RADIOLIB_ERR_NONE;  // save transmission state between loops
  unsigned long _lastPing = 0;// timestamp of the last PING packet
  
  unsigned long _nextTransmit=0;
  unsigned long _completedTransmit=0;
  
  CMsg mACK;
  bool _sendACK=true;
  bool _waitForACK=false;
  unsigned long _delayTransmit=RADIOTXDELAY;
  unsigned long _modemChangedOn=0;
  CMessages *pMsgs;
  bool volatile  *_pbFlag;
  bool volatile  *_penableInterrupt;
  bool _bTransmitter=true;
  bool _bReceiver=true;
  unsigned long _sleepTime=0;

  unsigned long _lastTransmit=0;
  unsigned long _lastReceive=0;

  int _badInterruptCount=0;

  std::string _modem="";
  

public:

  CRadio();
  void init();
  void setMessages(CMessages *ptr){pMsgs=ptr;}
  void setRfMode(bool transmit);// function to set RF mode to transmit or receive
  void enableRX();
  void enableTX();
  void receivedLogic(unsigned char *buffer, int len);
  std::string isBinary(unsigned char *buffer, int len);
  void setup();
  bool isTransmitTime();
  void TransmitCmd();
  
  void TransmitPacket(CMsg &m);
  void TransmitPacket(std::string str="ACK",bool bAck=false);
  void TransmitPacket(const unsigned char *buf, int len,bool bAck=false);
  void SetRadioReceive();
  void ReceivedPacket();
  void loopRadio();
  void loop();
  void SendAck(CMsg &m);
  bool isAck(){if (mACK.Parameters.size()) return true; return false;}
  bool readyToTransmit();
  void checkModeTX();
  void checkModeRX();
  void incBadInterrupt();
  void setACK(bool tmp){_sendACK=tmp;}
  void setTransmitter(bool tmp){_bTransmitter=tmp;}
  bool getTransmitter(){return _bTransmitter;}

  void setReceiver(bool tmp){_bReceiver=tmp;}
  bool getReceiver(){return _bReceiver;}

  void resetAck(){mACK.clear();};
  void Update(CMsg &msg); 
  void sleep(bool tmp);
  bool stillSleeping();
  void clearSleep();

  void setPower(CMsg &m);  
  void setModem(CMsg &m);  
  std::string getModem(){return _modem;}
  void chkModem();
  void callCustomFunctions(CMsg &m)override;   
  void resetPower(CMsg &m);   

  int getBW();
  int getSF();
  int getCR();
  float getFrequency();

  unsigned long getLastTransmit(){return _lastTransmit=0;}
  unsigned long getLastReceive(){return _lastReceive=0;}
  int getRadioState(){return(_radioState);}
  
};
