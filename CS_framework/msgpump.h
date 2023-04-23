#pragma once

#include <vector>
#include <list>
#include <map>
#include "systemobject.h"
#include "messages.h"



class CMsgPump:public CSystemObject {

public:

  CMsgPump(){MMM.TransmittedList.maxSize(15);setInterval(10);}
  void pump();
  void loop();
  CMsg rawStats(CMsg &msg);
  void callCustomFunctions(CMsg &msg);
};
