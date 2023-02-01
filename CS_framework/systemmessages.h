#pragma once

#include <vector>
#include <list>
#include <map>
#include "systemobject.h"
#include "messages.h"



class CMessagesObj:public CSystemObject {
  
public:

  CMessagesObj(){MMM.TransmittedList.maxSize(15);setInterval(10);}

  void saveToCloud(bool tmp=true){MMM.saveToCloud(tmp);}  
  void saveToCloud(CMsg &msg);
  void MsgPump();
  void loop();
  void stats(CMsg &msg);
  void callCustomFunctions(CMsg &msg);
};
