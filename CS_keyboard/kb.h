#pragma once

#include <systemobject.h>



class CKeyboard:public CSystemObject {
private:
  std::map<std::string, std::string> _simpleCMDs;
  char _input;
  std::string _cmdstr;
  long _last=0;
  int _id=0;
  long _dlast=0;

public:
  CKeyboard(){myInit();}
  void loop();
  void myInit();
  void sendCmd(std::string str);
  void sendCmd(CMsg &msg); 
  void displayCMDs();
  void dataLoop();  
  void callCustomFunctions(CMsg &msg); 
};


