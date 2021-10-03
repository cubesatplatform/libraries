#include "funcs.h"
#include "arduino.h"

long getTime() {
  return millis();
}

std::string getTimeString(){
  unsigned long tmp=millis();
  return(tostring(tmp));
}



std::string tostring(unsigned long val) {
  std::string s;
  char buffer [30];
  int cx;
  
  cx = snprintf ( buffer, 30, "%lu", val );
  if((cx>0)&&(cx<100)) s=buffer;
  return s;
}


std::string tostring(long val) {
  std::string s;
  char buffer [30];
  int cx;
  
  cx = snprintf ( buffer, 30, "%li", val );
  if((cx>0)&&(cx<100)) s=buffer;
  return s;
}

std::string tostring(int val) {
  std::string s;
  char buffer [30];
  int cx;
  
  cx = snprintf ( buffer, 30, "%d", val );
  if((cx>0)&&(cx<100)) s=buffer;
  return s;
}

std::string tostring(float val) {
  std::string s;
  char buffer [30];
  int cx;
  
  cx = snprintf ( buffer, 30, "%f", val );
  if((cx>0)&&(cx<100)) s=buffer;
  return s;
}
