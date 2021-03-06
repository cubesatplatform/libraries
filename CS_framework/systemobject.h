#pragma once

#include <vector>
#include "msg.h"

#include "consoleio.h"


#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  #include "Wire.h" 
  using namespace mbed;
#else
  typedef int PinName; 
  #define Wire2 Wire
#endif


#define STOPTASKLIMIT 1234567890 
#define STOPTASKMAX   2147483647


class CSystemObject {  
  bool success;  //true,false

  std::string _name;
  std::string _cid;
  static int sid;
  static std::string _IAM;
  static std::string _defaultTO;
  static unsigned long _lastLowPowerMsg;
  int _sid;
  
  unsigned long _modifiedTime=0;  
  unsigned long _prevTime=0;
  unsigned long _interval=0;  //loop time interval in MicroSeconds,  0 means no delay  
  unsigned long _loopCount=0;
  unsigned long _procCount=0;

  std::string _ostate;   //"" || "START","PLAY","PAUSE","STOP","ERROR" 
  std::string _olaststate;

  unsigned long _lastStateTime=0;  
  unsigned long _lastUse=0;
  int _retryCount=0;
  bool _forever = false;
  int _step=0;
  std::string _mode;   //This determines what you are doing   Set to IDLE when done and nothing left to do  When u set new mode need to get it out of IDLE
  int _errorCount=0;
  int _errorThreshold=5;

protected:
  
  unsigned long _createdTime=0;
  unsigned long _currentTime=0;
  unsigned long _startTime=0;
  unsigned long _stopTime=0;
  unsigned long _maxTime=5000;
  unsigned long _minTime=0;
  std::map<std::string, std::string> Callback;  
  CMsg _cmsg;
  

public:
  unsigned long _lastDebug=0;
  unsigned long _lastDebug1=0;
  CSystemObject();
  virtual ~CSystemObject() {}

  virtual void setup() {setState("PLAY");};
  virtual void loop()  {setState("PLAY");};
  virtual void test(CMsg &msg)  {};
  virtual void runOnce(CMsg &msg)  {};
  virtual void config(CMsg &msg)  {};
          
  virtual void start() {CSystemObject::init();setup(); if(State()=="") setState("PLAY"); };   //It sets the state to the next state if nothing to do.  Your setup can override
  virtual void play() {if(State()=="PAUSE") setState(lastState()); loop(); }
  virtual void pause() {setState("PAUSE");}  
  virtual void stop()  {setState("STOP");};  
  virtual void off()  {};  
  virtual void init();

  void restart(){setState("");}
  
  std::string IAM(){return _IAM;}
  std::string defaultTO(){return _defaultTO;}
  bool outOfTime();
 
  void setup(CMsg &msg) { setup(); };
  void loop(CMsg &msg)  {loop();};
  
  void tic(){_lastUse=getTime();if(_ostate=="PAUSE")setState("PLAY");}
  unsigned long lastUsed(){return _lastUse;}

  void setForever(bool tmp=true){  _forever=tmp;}
  bool getForever(){ return _forever;}

  int getErrorCount(int tmp=0){  _errorCount=tmp;}
  void resetErrorCount(){  _errorCount=0;}
  bool incErrorCount(){  _errorCount++;  if(_errorCount>_errorThreshold){_errorCount=0;setState("ERROR"); return true;}return false;}
  void setErrorThreshold(int tmp=1){  _errorThreshold=tmp;}
  int getErrorThreshold(){  return _errorThreshold;}

  void setStep(int tmp=0){  _step=tmp;}
  bool getStep(){ return _step;}
  bool incStep(){ _step++;}

  void setMaxTime(unsigned long tmp){_maxTime=tmp;}
  unsigned long getMaxTime(){return(_maxTime);}

  void setInterval(unsigned long tmp){_interval=tmp;}
  unsigned long getInterval(){return _interval;}

  void State(CMsg &msg); 
  std::string State() { return _ostate; }
  std::string lastState() { return _olaststate; }  
  virtual void Update(CMsg &msg);
  virtual void echoData(CMsg &msg){}
  
  void newMsg(CMsg &msg);
  virtual void newMode(CMsg &msg);
  void sendError();  

  virtual void setMode(std::string tmp){_mode=tmp;}
  std::string Mode(){return _mode;}

  unsigned long startTime(){return _startTime;};
  void startTime(unsigned long tmp){_startTime=tmp;};
  unsigned long stopTime(){return _stopTime;};
  void stopTime(unsigned long tmp){_stopTime=tmp;};
  unsigned long modifiedTime(){return _modifiedTime;};
  
  virtual void callCustomFunctions(CMsg &msg){};   //Override to calls any system specific function directly
  
  void timeStamp();
 
  void stats(CMsg &ms);
  void Run(unsigned long runtime);                    
  void Run();          
  void Run(CMsg &m){unsigned long runtime=m.getParameter("RUNTIME",10); Run(runtime);}                                                                               //Next state calls State Transition Functions
  void setState(std::string str);        //Only set the state  Dont call any functions.  The Next state should take care of that
  void Name(const char* s) { std::string str= s; Name(str); }
  void Name(std::string s);
 
  void addTransmitList(CMsg &m );
  void addDataMap(std::string key, CMsg &m); 
  void addMessageList(CMsg &m );
  void addReceivedList(CMsg &m );
  CMsg getDataMap(std::string key); 
    
  //void statusUpdate(CMsg &m);
  void fillMetaMSG(CMsg *m);

  void goLowPowerState();
  void goNormalState(){    CMsg m;    m.setSYS("SAT");    m.setACT("NORMAL");    addMessageList(m);  }

  int getRetryCount(){return _retryCount; }
  void clearRetryCount(){_retryCount=0; }
  unsigned long getCreatedTime() {return _createdTime;}
  unsigned long getModifiedTime(){return _modifiedTime=0;}
  unsigned long getCurrentTime() {return _currentTime;}
  unsigned long getPrevTime(){return _prevTime;}
  void setModifiedTime(unsigned long tmp){_modifiedTime=tmp;}
  
  unsigned long StartTime(){return _startTime=0;}
  unsigned long StopTime(){return _stopTime=0;}

  long timeSinceStateChange(){return getTime()-_lastStateTime;}
  std::string Name() { return _name; }
  std::string getCID() { return _cid; }
  std::string getIAM() { return _IAM; }


  bool isNextCycle();
  bool isSuccess() { return success; }
  bool isComplete(){ 
    if ((_ostate == "STOP")|| (_ostate == "ERROR")) return true; 
    return false;   ///   false;  Dne for debuggin
  }


  std::string outputStatus(long val=100000);
  unsigned long getReceivedTimestamp();
  void transmitError(const char *tmp);
  void reboot();

  void transmitResult(std::string act,std::string result);
  void transmitResult(std::string act,CMsg msg);

};

extern std::map<std::string,CSystemObject *> SysMap;
extern std::map<std::string, PinName> Pins;
extern std::map<std::string, PinName> pwmPins;
extern std::map<std::string, std::string> I2CMap;
std::string getSatState();
void switchRadios();

CSystemObject *getSystem(const char *sys, const char *comment="");
CSystemObject *getIMU(const char *sys="IMUI2C");





