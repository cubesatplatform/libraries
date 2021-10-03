#include "msg.h"
#include "consoleio.h"
#include <arduino.h>

int CMsg::_ID = 0;




std::string CMsg::getParameter(std::string str,std::string val){
  std::string res=getParameter(str);
  if (res.size()==0) res=val;
  return res;
}

char CMsg::getParameter(std::string str,char val){
  std::string res=getParameter(str);
  if (res.size()==0) res=val;
  return res[0];
}



long CMsg::getParameter(std::string str,long val){
  std::string res=getParameter(str);
  if (res.size()==0) return(val);
  return atol(res.c_str());
}

unsigned long CMsg::getParameter(std::string str,unsigned long val){
  std::string res=getParameter(str);
  if (res.size()==0) return(val);
  return atol(res.c_str());
}

int CMsg::getParameter(std::string str,int val){
  std::string res=getParameter(str);
  if (res.size()==0) return(val);
  return atol(res.c_str());
}

float CMsg::getParameter(std::string str,float val){
  std::string res=getParameter(str);
  if (res.size()==0) return(val);
  return atof(res.c_str());
}

void CMsg::appendParams(std::map<std::string, std::string> &Params){
  for (auto x:Params){
    Parameters[x.first]=x.second;
  }
}


/*

std::string CMsg::tostring(long val) {
  std::string s;
  char buffer [30];
  int cx;
  
  cx = snprintf ( buffer, 30, "%li", val );
  if((cx>0)&&(cx<100)) s=buffer;
  return s;
}

std::string CMsg::tostring(unsigned long val) {
  std::string s;
  char buffer [30];
  int cx;
  
  cx = snprintf ( buffer, 30, "%lu", val );
  if((cx>0)&&(cx<100)) s=buffer;
  return s;
}

std::string CMsg::tostring(float val) {
  std::string s;
  char buffer [30];
  int cx;
  
  cx = snprintf ( buffer, 30, "%f", val );
  if((cx>0)&&(cx<100)) s=buffer;
  return s;
}

std::string CMsg::tostring(char val) {
  std::string s;
  s=val;

  return s;
}

*/

std::string CMsg::TransmitData() { 
 // std::string ss=str + "&" + "ID=" + getID() + "&CID=" + getCMDID() + StringOffset()+"&TS="+tostring(getTime())+"&TC="+tostring(tc); 
 std::string ss=serialize();
 ss+="~TS:"+tostring(getTime())+"~TC:"+tostring(_tc); 
  writeconsole(ss);
  return ss;
  }

std::string CMsg::serialize() {
  std::string str1;
 // for (std::map<std::string, std::string>::iterator it = Parameters.begin(); it != Parameters.end(); ++it) {
    //for (auto it = Parameters.begin(); it != Parameters.end(); ++it) {
    for (auto it : Parameters  ){
    std::string s = it.first + ":" + it.second;
    //str1 += s + "~";
    str1 = s + "~"+str1;
  }
  return str1;
}

void CMsg::deserialize() {

  if (_str.size() == 0) return;
  if (_str[_str.size() - 1] != '~')_str += '~';
  std::string  str1;

  std::vector<std::string> components;
  char c;
  char tok1 = '~';
  char tok2 = ':';
  int stop = 0, start = 0;

  for (int i = 0; i < _str.size(); i++) {
    c = _str[i];
    if (c == tok1) {
      stop = i;
      components.push_back(_str.substr(start, stop - start));

      start = i + 1;
    }
  }


  for (int x = 0; x < components.size(); x++) {
    str1 = components[x];
    for (int i = 0; i < str1.size(); i++) {
      c = str1[i];

      if (c == tok2) {
        stop = i;
        Parameters[str1.substr(0, i)] = str1.substr(i + 1, 100);
        break;
      }
    }
  }
}
