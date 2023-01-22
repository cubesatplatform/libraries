#pragma once

#include <vector>
#include <list>
#include <map>
#include "systemobject.h"
#include "messages.h"



class CMessagesObj:public CSystemObject {
public:

  CMessagesObj(){MMM.TransmittedList.maxSize(15);setInterval(10);}
  void MsgPump();
  void loop();
  void stats(CMsg &msg);
  void callCustomFunctions(CMsg &msg);
};
