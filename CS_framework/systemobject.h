#pragma once

#include <vector>
#include <cctype>

#include "msg.h"
#include "messages.h"

#include "consoleio.h"


#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  #include "Wire.h" 
  using namespace mbed;
#else
  typedef int PinName; 
  #define Wire2 Wire
#endif

 


#define mapcustom(f)   if(myCompare(act,#f)){f();return;}
#define mapcustommsg(f)   if(myCompare(act,#f)){f(msg);return;}


bool myCompare(std::string a,const char *f);

struct CTimeStruct {
  bool bOn=false;
  long _createdTime=0;
  long _currentTime=0;
  long _startTime=0;
  long _stopTime=0;
  long _maxTime=5000;
  long _minTime=0;
  long _modifiedTime=0;  
  long _prevTime=0;
  long _lastUse=0;

  long _enter_time;
	long _exit_time;	
	long _starttimeoffset = 0;
	long _lastcleanuptime=0;
  long _lastDebug=0;
  long _lastDebug1=0;

  bool outOfTime(){
   	if ( ((_currentTime - _startTime) > _maxTime)&&((_currentTime - _startTime) > _minTime)) {   //Play ->out of Time
    	return true;
  	}
    return false;
  }
  
};


class CSystemObject {  
  bool success;  //true,false

  
  std::string _cid;
  static int sid;
  static std::string _IAM;
  static std::string _defaultTO;
  static long _lastLowPowerMsg;  
  int _sid;
  

  long _interval=10;  //loop time interval in MicroSeconds,  0 means no delay  
  long _loopCount=0;
  long _procCount=0;


  long _lastStateTime=0;  
  int _retryCount=0;
  bool _forever = true;
  
  int _step=0;
  
  int _errorCount=0;
  int _errorThreshold=15;

  std::string _inUse;
  long _inUseOn=0;


protected:
  std::string _name;
  CTimeStruct _obj;
  CTimeStruct _echo;  

  std::map<std::string, std::string> Callback;  
  CMsg _m;

  static CMessages MMM;
  static std::map<std::string,CSystemObject *> SysMap;

  static std::map<std::string, PinName> Pins;
  static std::map<std::string, PinName> pwmPins;
  

public:
  
  CSystemObject();
  virtual ~CSystemObject() {}

  virtual void setup() {setState(_PLAY);};
  virtual void loop()  {setState(_PLAY);};
  virtual void test(CMsg &msg)  {};
  virtual void runOnce(CMsg &msg)  {};
  virtual void config(CMsg &msg)  {};
  virtual void output(CMsg &msg){};

          
  virtual void start() {CSystemObject::init();setup(); if(state()==_BLANK) setState(_PLAY); };   //It sets the state to the next state if nothing to do.  Your setup can override
  virtual void play() {if(state()==_PAUSE) setState(lastState()); loop(); }
  virtual void pause() {setState(_PAUSE);}  
  virtual void stop()  {setState(_STOP);};  
  virtual void off()  {};  
  virtual void init();

  void restart(){setState(_BLANK);}
  virtual void setMsg(CMsg &msg);
  
  std::string IAM(){return _IAM;}
  std::string defaultTO(){return _defaultTO;}
  std::string Mode(){return _m.get(_MODE);}
  void Mode(std::string str){ _m.set(_MODE,str);}
  void mode(CMsg &msg);

  PinName getPin(std::string str);
  

  bool outOfTime();
 
  void setup(CMsg &msg) { setup(); };
  void loop(CMsg &msg)  {loop();};
  
  void tic(){_obj._lastUse=getTime();if(_m.get(_STATE)==_PAUSE)setState(_PLAY);}
  long lastUsed(){return _obj._lastUse;}

  void setForever(bool tmp=true){  _forever=tmp;}
  bool getForever(){ return _forever;}

  //int getErrorCount(int tmp=0){  _errorCount=tmp;}
  void resetErrorCount(){  _errorCount=0;}
  bool incErrorCount(){  _errorCount++;  if(_errorCount>_errorThreshold){_errorCount=0;setState(_ERROR); return true;}return false;}
  int getErrorCount(){  return _errorCount;}
  void setErrorThreshold(int tmp=1){  _errorThreshold=tmp;}
  int getErrorThreshold(){  return _errorThreshold;}

  void setStep(int tmp=0){  _step=tmp;}
  int getStep(){ return _step;}
  int incStep(){ _step++;return _step;}

  void setMaxTime(long tmp){_obj._maxTime=tmp;}
  long getMaxTime(){return(_obj._maxTime);}

  void setInterval(long tmp){_interval=tmp;}
  long getInterval(){return _interval;}

  void state(CMsg &msg); 
  std::string state() { return _m.get(_STATE); }
  std::string lastState() { return _m.get(_STATELAST); }  
  virtual void update(CMsg &msg);

  
  void newMsg(CMsg &msg);
  virtual void newMode(CMsg &msg);
  void sendError();  

  
  long startTime(){return _obj._startTime;};
  void startTime(long tmp){_obj._startTime=tmp;};
  long stopTime(){return _obj._stopTime;};
  void stopTime(long tmp){_obj._stopTime=tmp;};
  long modifiedTime(){return _obj._modifiedTime;};
  
  virtual void callCustomFunctions(CMsg &msg){writeconsoleln("                 XXXXXX   Unknown Message  CallCustom                           XXXXX"); msg.writetoconsole();};   //Override to calls any system specific function directly
  
  void timeStamp();
 
  virtual void stats(CMsg &ms);
  void run(long runtime);                    
  void run();          
  virtual void run(CMsg &m){long runtime=m.get(_RUNTIME,100); run(runtime);}                                                                               //Next state calls State Transition Functions
  void setState(std::string str);        //Only set the state  Dont call any functions.  The Next state should take care of that
  void name(const char* s) { std::string str= s; name(str); }
  void name(std::string s);
 
  void addMessageList(CMsg &m );
  void addReceivedList(CMsg &m );
  void addCloudList(CMsg &m );
  void addTransmitList(CMsg &m );  
  void addDataMap(std::string key, CMsg &m); 
  void addDataMap(CMsg &m); 
  void setDataMap(CMsg &m){addDataMap(m);}
  void setDataMap(std::string key, CMsg &m){addDataMap(key,m);}; 
  CMsg getDataMap(std::string key); 


  virtual void sendData(CMsg &msg );
  
  std::string getDataString(std::string sname,std::string sfield);
  void setDataString(std::string sname,std::string sfield, std::string sval);
  double getDataValue(std::string sname,std::string sfield);
  //double getDataValue(const char *name,char _axis);    
  //void statusUpdate(CMsg &m);
  void fillMetaMSG(CMsg *m);

  //void goLowPowerState();
  void goState(std::string val);
  void goState(CMsg &msg);

  int getRetryCount(){return _retryCount; }
  void clearRetryCount(){_retryCount=0; }
  long getCreatedTime() {return _obj._createdTime;}
  long getModifiedTime(){return _obj._modifiedTime=0;}
  long getCurrentTime() {return _obj._currentTime;}
  long getPrevTime(){return _obj._prevTime;}
  void setModifiedTime(long tmp){_obj._modifiedTime=tmp;}
  
  long StartTime(){return _obj._startTime=0;}
  long StopTime(){return _obj._stopTime=0;}

  long timeSinceStateChange(){return getTime()-_lastStateTime;}
  std::string name() { return _name; }
  std::string getCID() { return _cid; }
  std::string getIAM() { return _IAM; }
  void setIAM(std::string str) { _IAM=str; }


  bool isNextCycle();
  bool isSuccess() { return success; }
  bool isComplete(){ 
    if ((_m.get(_STATE) == _STOP)|| (_m.get(_STATE) == _ERROR)) return true; 
    return false;   ///   false;  Dne for debuggin
  }


  std::string outputStatus(long val=100000);
  
  void transmitError(const char *tmp);
  void reboot();

  void transmitResult(std::string act,std::string result);
  void transmitResult(std::string act,CMsg msg);

  void setInUse(std::string s){_inUse=s;_inUseOn=getTime();}
  std::string inUse(){if(_inUseOn+90000000<getTime()) _inUse=""; return _inUse;}

  void enablePin(std::string str);
  void disablePin(std::string str);

  virtual void enter(){};
	virtual void exit(){};
  void initPins();

  CSystemObject *getSystem(const char *sys, const char *comment=_BLANK);  

};


void switchRadios();









