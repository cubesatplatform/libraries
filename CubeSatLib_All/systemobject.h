#pragma once
#include <vector>
#include "msg.h"


#include "consoleio.h"

//SYS:TURNX,TURNY,TURNZ,MAG,DETUMBLE
//ACT:START,STOP,PAUSE,UPDATE,FUNCTION


class CSystemObject {
  
  
  bool success;  //true,false

  std::string _name;
  std::string _cid;
  static int sid;
  int _sid;
  unsigned long _createdTime;
  unsigned long _modifiedTime=0;
  unsigned long _prevTime;
  

protected:
  std::map<std::string, std::string> Callback;
  std::string _ostate;   //"NONE","INIT","READY","PLAY","PAUSE","STOP","ERROR", "IDLE"
  unsigned long _currentTime;
  CMsg _cmsg;
  bool _forever = false;
  unsigned long _lastStateTime=0;
  
  unsigned long _maxTime=0;
  unsigned long _minTime=0;
  unsigned long _INTERVAL;  //loop time interval in MicroSeconds,  0 means no delay
  unsigned long _lastUse=0;
  unsigned long _startTime=0;
  unsigned long _stopTime=0;
  
  
  unsigned long _transmitINTERVAL;
  unsigned long _transmitPREVTIME;

  unsigned long _loopCount=0;
  unsigned long _procCount=0;
  std::string _Mode;   //This determines what you are doing   Set to IDLE when done and nothing left to do  When u set new mode need to get it out of IDLE


public:
  CSystemObject();
  virtual ~CSystemObject() {}

  virtual void none() { setState("ON"); };
  virtual void on() { setState("READY");setup(); };   //It sets the state to the next state if nothing to do.  Your setup can override
  virtual void idle() { setState("IDLE"); }
  virtual void ready() { setState("PLAY"); }
  virtual void play() { loop(); }
  virtual void pause() { setState("PAUSE"); }
  virtual void setup() { setState("READY"); };
  virtual void loop()  {setState("COMPLETE");};
  virtual void stop()  {setState("COMPLETE");};
  virtual void complete()  {setState("OFF");}
  virtual void off() { setState("REMOVE"); };
  virtual void restart() { setState("PLAY"); }
  virtual bool run() {return false;}

  void Done()  {setState("COMPLETE");};
  bool outOfTime();
  bool Forever();
  void Forever(bool tmp){_forever=tmp;};
  void setup(CMsg &msg) { setup(); };
  void loop(CMsg &msg)  {loop();};
  void tic(){_lastUse=getTime();if(_ostate=="PAUSE")setState("PLAY");}
  unsigned long lastUsed(){return _lastUse;}


  void State(CMsg &msg); 
  virtual void Update(CMsg &msg);
  virtual void Output(CMsg &msg){Output();}
  virtual void Output(){}
  void newMsg(CMsg &msg);
  void newMode(CMsg &msg);
  virtual void initMode(){};

  unsigned long startTime(){return _startTime;};
  unsigned long stopTime(){return _stopTime;};
  unsigned long modifiedTime(){return _modifiedTime;};
  

  virtual void callCustomFunctions(CMsg &msg){};   //Override to calls any system specific function directly
  
  void timeStamp();
  std::string currentState() { return _ostate; }
  virtual void stats();
  void nextState();                                                                                         //Next state calls State Transition Functions
  void setState(std::string str) { _ostate = str; _lastStateTime=getTime();};                                 //Only set the state  Dont call any functions.  The Next state should take care of that
  void Name(const char* s) { _name = s; }
  void Name(std::string s) { _name = s; }
  bool isTransmitTime();
  bool isTransmitTimeNoUpdate();
  bool addTransmitList(const char* s,bool override=false);
  bool addDataList(const char* s);
  bool addTransmitList(CMsg &m ,bool override=false);
  bool addMessageList(const char* s);
  bool addMessageList(CMsg &m );
  bool addTransmitList(std::vector<CMsg> &M ,bool override=false);
  bool addDataList(CMsg &m);
  void statusUpdate(CMsg &m);
  void fillMetaMSG(CMsg *m);
  long timeSinceStateChange(){return getTime()-_lastStateTime;}
  std::string Name() { return _name; }
  std::string CID() { return _cid; }

  bool isNextCycle();
  bool isSuccess() { return success; }
  bool isComplete(){ 
    if ((_ostate == "STOP")|| (_ostate == "ERROR")|| (_ostate == "COMPLETE")|| (_ostate == "OFF")) return true; 
    return false;   ///   false;  Dne for debuggin
  }

  std::string outputStatus(long val=100000);
  void respondCallBack(CMsg &m);
};

CSystemObject *getSystem(const char *sys);

