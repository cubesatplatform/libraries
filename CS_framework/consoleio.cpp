
#include "funcs.h"
#include "consoleio.h"


//std::list<std::string> qLog;

bool _echoConsole=true;


#define REALTIME

void echoOn(){
  _echoConsole=true;
}

void echoOff(){
  _echoConsole=false;
}

void writeOut(){
  /*
  while(qLog.size()){
    std::string str=qLog.front();
    qLog.pop_front();
    if(!str.size()){
      if(Serial) Serial.println();
    }
    else  {
      if(Serial) Serial.print(str.c_str());
    }
  }
  */
}



void writeconsole(double s){writeconsole((float) s);};

void writeconsoleln(double s){writeconsoleln((float) s);};


void writeconsole(const std::string s) {  
  if(_echoConsole){
    if(Serial) Serial.print(s.c_str());
  }
  else{
  //qLog.push_back(s);  
  }
 
}

void writeconsole(String s) {  
  if(_echoConsole){
    if(Serial) Serial.print(s.c_str());
  }
  else{
  std::string str=s.c_str();
  //qLog.push_back(str);  
  }
}


void writeconsoleln(const std::string s){   
  if(_echoConsole){
  if(Serial) Serial.println(s.c_str());
  }
  else{
   std::string blank;
   //qLog.push_back(s);
   //qLog.push_back(blank);
   }
  }

 void writeconsoleln(String s){   
  if(_echoConsole){
  if(Serial) Serial.println(s.c_str());
  }
  else{
   std::string blank;
   std::string str=s.c_str();
   //qLog.push_back(str);  
   //qLog.push_back(blank);
   }
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



void writeconsoleln(float s){
   std::string str=tostring(s);
   writeconsoleln(str);
  }

   
//Encoding formulas
//y=2x+<seed>
//x=(y-<seed>)/2

int charEncode(int x, int seed){
  int mseed=seed%10;
  int y=2*x+mseed;
  return y;
}

int charDecode(int y, int seed){
  int mseed=seed%10;
  int x=(y-mseed)/2;
  return x;
}


std::string stringEncode(std::string str,int seed){
  std::string newstr;
  int c;

  for(auto x: str){
    c=charEncode(x,seed);
    newstr+=c;    
  }
  return newstr;
}

std::string stringDecode(std::string str,int seed){
  std::string newstr;
  int c;

  for(auto x: str){
    c=charDecode(x,seed);
    newstr+=c;    
  }
  return newstr;
}
