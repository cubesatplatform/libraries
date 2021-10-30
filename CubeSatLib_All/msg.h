#pragma once
#include "defs.h"
#include <map>
#include <string>
#include <vector>
#include "funcs.h"



class CMsg  {
  std::string _str;

  float _rssi = 0.0;
  float _snr = 0.0;
  static int _ID;       //Count of objects created
  int _refID = 0;       //May use this as a passed reference ID
  unsigned long _tc=getTime();
  
public:
  std::vector<unsigned char> byteVector;
  std::map<std::string, std::string> Parameters;

  CMsg() {_tc=getTime();};
  CMsg(std::string &s) { _str = s; _tc=getTime(); deserialize(); }
  CMsg(const char* s) { _str = s; _tc=getTime(); deserialize(); }
  CMsg(const char* s, float frssi , float fsnr = 0.0) { _str = s; _tc=getTime(); deserialize();  _rssi = frssi; fsnr = fsnr; _ID++;}
  void decompose(const char* s){ _str = s; }
  
  std::string serialize();
  void deserialize();
  int serialSize(){std::string s=serialize();return(s.size());}
  
  std::string getMsg() {return _str;}
  std::string getSys() { return  getSYS(); }

  std::string getACT() { return  Parameters["ACT"]; }
  std::string getSYS() { return  Parameters["SYS"]; }
  std::string getSTATE() { return  Parameters["STATE"]; }
  std::string getMODE() { return  Parameters["MODE"]; }
  std::string getID() { return  Parameters["ID"]; }
  std::string getCMDID() { return  Parameters["CMDID"]; } 
  std::string CID() { return getCMDID(); }  


  void setACT(std::string str) { Parameters["ACT"]=str; }
  void setSYS(std::string str) { Parameters["SYS"]=str; }
  void setSTATE(std::string str) { Parameters["SYS"]="CORE";Parameters["ACT"]="STATE";Parameters["STATE"]=str; }
  void setMODE(std::string str) { Parameters["MODE"]=str; }
  void setID(std::string str) { Parameters["ID"]=str; }
  void setCMDID(std::string str) { Parameters["CMDID"]=str; } 
  void appendParams(std::map<std::string, std::string> &Params);

 /*
  void setACT(const char * str) { Parameters["ACT"]=str; }
  void setSYS(const char * str) { Parameters["SYS"]=str; }
  void setMODE(const char * str) { Parameters["MODE"]=str; }
  void setID(const char * str) { Parameters["ID"]=str; }
  void setCMDID(const char * str) { Parameters["CMDID"]=str; } 
  */


  std::string getParameter(std::string str) { return Parameters[str]; }
  std::string getParameter(std::string str,std::string val);
  int getParameter(std::string str,int val);
  long getParameter(std::string str,long val);
  unsigned long getParameter(std::string str,unsigned long val);
  float getParameter(std::string str,float val);
  double getParameter(std::string str,double val);
  char getParameter(std::string str,char val);
  

  
  void setParameter(std::string str,std::string val ) { Parameters[str]=val; }
  void setParameter(std::string str,int val ) { Parameters[str]=tostring((long) val); }
  void setParameter(std::string str,long val ) { Parameters[str]=tostring( val); }
  void setParameter(std::string str,unsigned long val ) { Parameters[str]=tostring( val); }
  void setParameter(std::string str,float val ) { Parameters[str]=tostring( val); } 
  void setParameter(std::string str,double val ) { Parameters[str]=tostring( val); } 
  void setParameter(std::string str,char val ) { Parameters[str]=tostring( val); } 
  void CID(std::string str) { Parameters["CID"]= str; }

  std::string Offset() {  return  Parameters["OFFSET"]; }
  std::string StringOffset() { return Parameters["OFFSET"]; }
  void Offset(std::string str) {  Parameters["OFFSET"] = str; }
  

  std::string Data() { return _str; }
  void Data(std::string s) { _str = s; }
  
  std::string TransmitData();
  
 
  int REFID() { return _refID; }
  void REFID(int r) { _refID = r; }

/*
  std::string tostring(long val);
  std::string tostring(unsigned long val);
  std::string tostring(float val);
  std::string tostring(char val);
*/
  void initArray(unsigned char* myRawArray, int byteCount) {
    byteVector.reserve(byteCount);
    byteVector.insert(byteVector.begin(), myRawArray, myRawArray + byteCount);
    
  }
  unsigned char *vectorData(){return byteVector.data();};
  int vectorLen(){return byteVector.size();}
};
