#include  <string>
#include "system_mgr.h"
#include "messages.h"
#include "defs.h"
#include "powerup.h"

   
void CSystemMgr::initCommands(){
  CMsg m;
  std::list<CMsg> ml;
  
  ml.clear();

  m.clear();  
  m.setSYS("GPS");
  m.setACT("START");
  ml.push_back(m);

  m.clear();  
  m.setSYS("GPS");
  m.setACT("OUTPUT");
  m.setParameter("_START",3000L);
  m.setParameter("_STOP",30000L);
  m.setParameter("_INTERVAL",10000L);  
  ml.push_back(m);

  m.clear();  
  m.setSYS("GPS");
  m.setACT("STOP");
  m.setParameter("_START",33000L);
  m.setParameter("_STOP", 33000L);  
  ml.push_back(m);

  Commands["CMD_GPS_OUTPUT"]=ml;
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  ml.clear();

  m.clear();  
  m.setSYS("IRX1");
  m.setACT("START");
  ml.push_back(m);

  m.clear();  
  m.setSYS("IRX1");
  m.setACT("OUTPUT");
  m.setParameter("_START",3000L);
  m.setParameter("_STOP",300000L);
  m.setParameter("_INTERVAL",10000L);   
  ml.push_back(m);

  m.clear();  
  m.setSYS("IRX1");
  m.setACT("STOP");
  m.setParameter("_START",10000L);
  m.setParameter("_STOP",10000L);  
  ml.push_back(m);
  Commands["CMD_IRX1_OUTPUT"]=ml;



}

void CSystemMgr::showScheduler(){
  writeconsoleln("");
  writeconsoleln("ShowScheduler xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");  
  for(auto m:Scheduler){    
    writeconsoleln(m.serialize());  
  }
   writeconsoleln("");
}

void CSystemMgr::showCommands(){
  writeconsoleln("");
  writeconsoleln("ShowCommands xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");  
  
  std::list<CMsg> ml;
  std::string namestr;
  for (auto it = Commands.begin(); it != Commands.end(); ++it) {    
    namestr = it->first;
    writeconsoleln(namestr);
    ml=it->second;  
    for(auto m:ml){    
      writeconsoleln(m.serialize());  
    }
  }
  writeconsoleln("");
}
  
