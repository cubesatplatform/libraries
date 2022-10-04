#pragma once

#include <systemobject.h>

#define CUBESAT "ADR1"

class CKeyboard:public CSystemObject {
private:
  std::map<std::string, std::string> _simpleCMDs;
  char _input;
  std::string _cmdstr;
  long _last=0;
  int _id=0;
  unsigned long _dlast=0;

public:
  CKeyboard(){Name("KEYBOARD");myInit();}
  void loop();
  void myInit();
  void sendCmd(std::string str);
  void displayCMDs();
  void dataLoop();
  void sendSerial(const char* cmd);  
  void callCustomFunctions(CMsg &msg); 
};
