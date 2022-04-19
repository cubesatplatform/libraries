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

protected:
  
public:
  std::vector<unsigned char> byteVector;
  std::map<std::string, std::string> Parameters;

  CMsg() {_tc=getTime();_ID++;};
  CMsg(std::string &s) { _str = s; _tc=getTime(); deserialize(); _ID++;}
  CMsg(const char* s) { _str = s; _tc=getTime(); deserialize(); _ID++;}
  CMsg(const char* s, float frssi , float fsnr = 0.0) { _str = s; _tc=getTime(); deserialize();  _rssi = frssi; fsnr = fsnr; _ID++;}
  
  void decompose(const char* s){ _str = s; }
  void clear(){_str="";_rssi = 0.0;_snr = 0.0;_refID = 0; _tc=getTime();Parameters.clear();byteVector.clear();}
  
  std::string serialize();
  std::string serializeout();
  void writetoconsole();
  void deserialize();
  int serialSize(){std::string s=serialize();return(s.size());}
  
  std::string getMsg() {return _str;}
  std::string getSys() { return  getSYS(); }

  std::string getACT() { return  Parameters["ACT"]; }
  std::string getACK() { return  Parameters["ACK"]; }
  std::string getSYS() { return  Parameters["SYS"]; }
  std::string getSAT() { return  Parameters["SAT"]; }
  std::string getMODE() { return  Parameters["MODE"]; }
  std::string getID() { return  Parameters["ID"]; }
  std::string getCID() { return  Parameters["CID"]; } 
  std::string getDATA() { return  Parameters["D"]; } 
  std::string getOFFSET() { return  Parameters["O"]; } 
  std::string getTABLE() { return  Parameters["T"]; }
  std::string getEVENT() { return  Parameters["E"]; } 
  std::string getERROR() { return  Parameters["0"]; } 
  std::string getINFO() { return  Parameters["I"]; } 
  std::string getVALUE() { return  Parameters["V"]; } 
  std::string getCOMMENT() { return  Parameters["C"]; } 
  std::string getPANEL() { return  Parameters["P"]; } 
  bool checkPWD();

  void requestACK(){setACK("0");}
  void confirmACK(){setACK("1");}
  void setACT(std::string str) { Parameters["ACT"]=str; }
  void setACK(std::string str) { Parameters["ACK"]=str; }
  void setSYS(std::string str) { Parameters["SYS"]=str; }
  void setSAT(std::string str="") { Parameters["SAT"]=str; }
  void setDATA(std::string str="") { Parameters["D"]=str; }
  void setEVENT(std::string str="") { Parameters["E"]=str; }
  void setERROR(std::string str="") { Parameters["0"]=str; }
  void setINFO(std::string str="") { Parameters["I"]=str; }
  void setOFFSET(std::string str="") { Parameters["O"]=str; }
  void setOFFSET(int tmp) { Parameters["O"]=tostring(tmp); }
  void setTABLE(std::string str) { Parameters["T"]=str; }
  void setPANEL(std::string str) { Parameters["P"]=str; }
  void setCOMMENT(std::string str) { Parameters["C"]=str; }
  void setCID(std::string str="") { Parameters["CID"]=str; }
  bool setPWD();  //Returns false if parameters are not there for a real pwd    
  
  void setMODE(std::string str) { Parameters["MODE"]=str; }
  void setID(std::string str) { Parameters["ID"]=str; }  
  void appendParams(std::map<std::string, std::string> &Params);

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

  //std::string Offset() {  return  Parameters["OFFSET"]; }
  //std::string StringOffset() { return Parameters["OFFSET"]; }
  //void Offset(std::string str) {  Parameters["OFFSET"] = str; }
  
  std::string Data() { return _str; }
  void Data(std::string s) { _str = s; }
  
  std::string TransmitData();
  bool needACK();
   
  int REFID() { return _refID; }
  void REFID(int r) { _refID = r; }

  void initArray(unsigned char* myRawArray, int byteCount) {
    byteVector.reserve(byteCount);
    byteVector.insert(byteVector.begin(), myRawArray, myRawArray + byteCount);    
  }
  unsigned char *vectorData(){return byteVector.data();};
  int vectorLen(){return byteVector.size();}
  void cleanup();
};
