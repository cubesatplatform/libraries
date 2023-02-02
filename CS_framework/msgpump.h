#pragma once

#include <vector>
#include <list>
#include <map>
#include "systemobject.h"
#include "messages.h"



class CMsgPump:public CSystemObject {

public:

  
  CMsgPump(){MMM.TransmittedList.maxSize(15);setInterval(10);}

  void saveToCloud(bool tmp=true){MMM.saveToCloud(tmp);}  
  void saveToCloud(CMsg &msg);
  void pump();
  void loop();
  void stats(CMsg &msg);
  void callCustomFunctions(CMsg &msg);
};
