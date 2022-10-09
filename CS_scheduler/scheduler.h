#pragma once

#include <msg.h>
#include <funcs.h>
#include <list>
#include <string>

#include <systemobject.h>

#include <unordered_set>



class CScheduler:public CSystemObject{
  
  std::list<CMsg> Scheduler;      
  std::map<std::string, std::list<CMsg>> Commands;      
    
public:
  
  CScheduler(); 
  ~CScheduler();

  void initSat();
  void initBS(){};

  void setup();
  void loop();
    
  void addTask(CMsg &msg);
  void deleteTask(CMsg &msg);
  void pauseTask(CMsg &msg);
  void unpauseTask(CMsg &msg);
  void showScheduler(CMsg &msg);
  
  void showCommands();
    
  void controlPlan();
  
  //void getData(CMsg &msg);
  void scheduleData(CMsg &msg);
  
  void Output(CMsg &msg);  
  void callCustomFunctions(CMsg &msg) override;
  void SendCmdToScheduler(CMsg &msg);
};













  
