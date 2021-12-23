#pragma once
#include "defs.h"
#include <string>
#include <map>

#ifdef PORTENTAXXX 



class CI2CInfo{
std::map<int,std::string> info;
  
public:
  CI2CInfo(){setup();}
  void setup();
  void addMap(int channel,char address,const char *desc);
  std::string getInfo(int channel,char address);
  void loopWire(TwoWire *wire,int channel);
};

#endif

