#include "defs.h"
#include "funcs.h"
#include "consoleio.h"
#include <arduino.h>

std::list<std::string> qLog;


#define REALTIME

void writeOut(){
  while(qLog.size()){
    std::string str=qLog.front();
    qLog.pop_front();
    if(!str.size())
      Serial.println();
    else  
      Serial.print(str.c_str());
  }
}


void writeconsole(const std::string s) {  
  #ifdef  REALTIME
    Serial.print(s.c_str());
  #else
  qLog.push_back(s);  
  #endif
}


void writeconsoleln(const std::string s){   
  #ifdef  REALTIME
  Serial.println(s.c_str());
  #else
   std::string blank;
   qLog.push_back(s);
   qLog.push_back(blank);
   #endif
  }



void writeconsole(CMsg &m){
  writeconsoleln(m.Data().c_str());
  
}

void writeconsole(const char* s) {
  std::string str=s;
   writeconsole(str);

}

void writeconsole(char s){
  std::string str;
  str=s;
  writeconsole(str);

  }

void writeconsole(int s){
  std::string str=tostring(s);
   writeconsole(str);

  }

void writeconsole( long s){
   std::string str=tostring(s);
   writeconsole(str);
  }

void writeconsole( unsigned long s){
    std::string str=tostring(s);
   writeconsole(str);
  }

void writeconsole(float s){
    std::string str=tostring(s);
   writeconsole(str);
  }

void writeconsoleln(){
  std::string str;
   writeconsole(str);
  }



void writeconsoleln(const char* s){
  std::string str=s;
   writeconsoleln(str);
  }

void writeconsoleln(int s){
    std::string str=tostring(s);
   writeconsoleln(str);
  }

void writeconsoleln( long s){
   std::string str=tostring(s);
   writeconsoleln(str);
  }

void writeconsoleln( unsigned long s){
   std::string str=tostring(s);
   writeconsoleln(str);
  }

void writeconsoleln(float s){
   std::string str=tostring(s);
   writeconsoleln(str);
  }

