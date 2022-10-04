#include "funcs.h"
#include <Arduino.h>

long getTime() {
  return millis();
}

std::string getTimeString(){
  unsigned long tmp=millis();
  return(tostring(tmp));
}



std::string tostring(unsigned long val) {
  std::string s;
  char buffer [300];
  int cx;
  
  cx = snprintf ( buffer, 30, "%lu", val );
  if((cx>0)&&(cx<100)) s=buffer;
  return s;
}


std::string tostring(long val) {
  std::string s;
  char buffer [300];
  long cx;
  
  cx = snprintf ( buffer, 30, "%li", val );
  if((cx>0)&&(cx<100)) s=buffer;
  return s;
}

std::string tostring(int val) {
  std::string s;
  char buffer [300];
  int cx;
  
  cx = snprintf ( buffer, 30, "%d", val );
  if((cx>0)&&(cx<100)) s=buffer;
  return s;
}

std::string tostring(unsigned int val) {
  std::string s;
  char buffer [300];
  int cx;
  
  cx = snprintf ( buffer, 30, "%d", val );
  if((cx>0)&&(cx<100)) s=buffer;
  return s;
}

std::string tostring(float val) {
  std::string s;
  char buffer [300];
  int cx;
  
  cx = snprintf ( buffer, 30, "%f", val );
  if((cx>0)&&(cx<100)) s=buffer;
  return s;
}

std::string tostring(double val) {
  std::string s;
  char buffer [300];
  int cx;
  
  cx = snprintf ( buffer, 30, "%f", val );
  if((cx>0)&&(cx<100)) s=buffer;
  return s;
}

std::string replaceall(std::string str, std::string from, std::string to) {
  size_t start_pos = str.find(from);
  while(start_pos != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos = str.find(from, start_pos + to.length());
  }
  return str;
}


/*Usage
  std::string url="/updatecmd?sent=1&cid=$cid&bsid=$bsid";
  std::string cid="14";
  url=strReplace(url,"$cid",cid);
 */

std::string strReplace(std::string original, const char * search, std::string val){

    String org=original.c_str();
    String searchstr=search;
    String valstr=val.c_str();

    org.replace(searchstr,valstr);
    return (std::string(org.c_str()));  
}
