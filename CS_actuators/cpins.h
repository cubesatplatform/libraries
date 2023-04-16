#pragma once

#include <systemobject.h>
#include <consoleio.h>

#include "cpin.h"





class CPins:public CSystemObject{
private:

  std::map<std::string,CPin *> _pins;

  
public:
  

  CPins(); //{setInterval(5);_channel=channel;channel++;};
  ~CPins();

  void setup(){};
  void loop();
  
  void addpin(CMsg &msg);
  void deletepin(CMsg &msg);  
  void changepin(CMsg &msg);  

  CPin *find(std::string name);
  void callCustomFunctions(CMsg &msg);

};
