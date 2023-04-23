#pragma once


#include <systemobject.h>

//#include <unordered_set>


#define S_ACT "S_ACT"
#define S_SYS "S_SYS"
#define S_START "S_START"
#define S_STOP "S_STOP"   
#define S_INTERVAL "S_INTERVAL"
#define S_MAXTIME "S_MAXTIME"
#define S_MINTIME "S_MINTIME"
#define S_LASTUSE "S_LASTUSE"
#define S_LASTCLEANUP "S_LASTCLEANUP"
#define S_LASTDEBUG "S_LASTDEBUG"
#define S_INTERVAL "S_INTERVAL"






class CScheduler:public CSystemObject{
  
  std::list<CMsg> Scheduler;      

    
public:
  
  CScheduler(); 
  ~CScheduler();


  void initSat();
  
  void initBS(){};
  void controlPlan();
  void setup();
  void loop();
    
  void addTask(CMsg &msg);
  void deleteTask(CMsg &msg);
  void deleteAll(CMsg &msg);
  void pauseTask(CMsg &msg);
  void unpauseTask(CMsg &msg);
  void showScheduler(CMsg &msg);
  CMsg convertTasktoMessage(CMsg &msg);
  CMsg convertMessagetoTask(CMsg &msg);
  
  void cloudDataMap(CMsg &msg);
  
  void output(CMsg &msg);  
  void schedule(std::string sys,std::string act, std::string name=std::string(""),long interval=60000, long start=0, long stop=STOPTASKMAX);
  void schedule(CMsg &msg,long interval=60000, long start=0, long stop=STOPTASKMAX);
  void callCustomFunctions(CMsg &msg) override;
  void sendCmdToScheduler(CMsg &msg);
  
};













  
