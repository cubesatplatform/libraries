#pragma once


#include <map>
#include <string>
#include <vector>
#include "funcs.h"



class CMsg  {
  std::string _str;

  float _rssi = 0.0;
  float _snr = 0.0;
  static int _ID;       //Count of objects created
  unsigned long _tc=getTime();

protected:
  
public:
  std::vector<unsigned char> byteVector;
  std::map<std::string, std::string> Parameters;

  CMsg() {_tc=getTime();_ID++;};
  CMsg(std::string &s) { _str = s; _tc=getTime(); deserialize(); _ID++;}
  CMsg(const char* s) { _str = s; _tc=getTime(); deserialize(); _ID++;}
  CMsg(const char* s, float frssi , float fsnr = 0.0) { _str = s; _tc=getTime(); deserialize();  _rssi = frssi; fsnr = fsnr; _ID++;}
  
  void decompose(const char* s){ _str = s; deserialize();}
  void clear(){_str="";_rssi = 0.0;_snr = 0.0; _tc=getTime();Parameters.clear();byteVector.clear();}
  
  std::string serialize();
  std::string serializeout();
  void writetoconsole();
  void deserialize();
  std::string serializeFile(const char * path=NULL);
  void saveFile();
  void deserializeFile(const char * path);
  int serialSize(){std::string s=serialize();return(s.size());}
  
  std::string getMsg() {return _str;}
  std::string getSys() { return  getSYS(); }

  int getStaticID(){return _ID;}

  std::string getACT() { return  Parameters["ACT"]; }
  std::string getACK() { return  Parameters["ACK"]; }
  std::string getSYS() { return  Parameters["SYS"]; }
  std::string getFROM() { return  Parameters["FR"]; }
  std::string getTO() { return  Parameters["TO"]; }
  std::string getto() { return  Parameters["to"]; }
  std::string getMODE() { return  Parameters["MODE"]; }
  std::string getID() { return  Parameters["ID"]; }
  std::string getREFID() { return  Parameters["REFID"]; }
  std::string getCID() { return  Parameters["CID"]; } 
  std::string getDATA() { return  Parameters["D"]; } 
  std::string getOFFSET() { return  Parameters["O"]; } 
  std::string getTABLE() { return  Parameters["T"]; }
  std::string getEVENT() { return  Parameters["E"]; } 
  std::string getKEY() { return  Parameters["K"]; } 
  std::string getERROR() { return  Parameters["0"]; } 
  std::string getINFO() { return  Parameters["I"]; } 
  std::string getVALUE() { return  Parameters["V"]; } 
  std::string getCOMMENT() { return  Parameters["C"]; } 
  std::string getPANEL() { return  Parameters["P"]; } 
  std::string getLOG() { return  Parameters["L"]; } 
  std::string getTIME() { return  Parameters["TM"]; } 
  bool checkPWD();

  void requestACK(){setACK("0");}
  void confirmACK(){setACK("1");}
  void setACT(std::string str) { Parameters["ACT"]=str; }
  void setACK(std::string str) { Parameters["ACK"]=str; }
  void setSYS(std::string str) { Parameters["SYS"]=str; }
  void setFROM(std::string str="") { Parameters["FR"]=str; }
  void setTO(std::string str="") { Parameters["TO"]=str; }
  void setDATA(std::string str="") { Parameters["D"]=str; }
  void setEVENT(std::string str="") { Parameters["E"]=str; }
  void setKEY(std::string str="") { Parameters["K"]=str; }
  void setERROR(std::string str="") { Parameters["0"]=str; }
  void setINFO(std::string str="") { Parameters["I"]=str; }
  void setVALUE(std::string str="") { Parameters["V"]=str; }
  void setOFFSET(std::string str="") { Parameters["O"]=str; }
  void setOFFSET(int tmp) { Parameters["O"]=tostring(tmp); }
  void setTABLE(std::string str) { Parameters["T"]=str; }
  void setPANEL(std::string str) { Parameters["P"]=str; }
  void setLOG(std::string str) { Parameters["L"]=str; }
  void setCOMMENT(std::string str) { Parameters["C"]=str; }
  void setCID(std::string str="") { Parameters["CID"]=str; }
  void setTIME(unsigned long tmp) { Parameters["TM"]=tostring(tmp); }
  bool setPWD();  //Returns false if parameters are not there for a real pwd    
  
  void setMODE(std::string str) { Parameters["MODE"]=str; }
  void setID(std::string str) { Parameters["ID"]=str; }  
  void setREFID(std::string str) { Parameters["REFID"]=str; }  
  void setREFID() { 
    std::string str=Parameters["REFID"];
    if (!str.size())
      Parameters["REFID"]=tostring((long) _ID); 
    }  
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

  
  std::string Data() { return _str; }
  void Data(std::string s) { _str = s; }
  
  std::string TransmitData();
  bool needACK();
   
  

  void initArray(unsigned char* myRawArray, int byteCount) {
    byteVector.reserve(byteCount);
    byteVector.insert(byteVector.begin(), myRawArray, myRawArray + byteCount);    
  }
  unsigned char *vectorData(){return byteVector.data();};
  int vectorLen(){return byteVector.size();}
  void cleanup();


  CMsg operator+=(const CMsg& b) {
    //writeconsole("ALERT  Not copying Byte Vector");
    

    for(auto s:b.Parameters){
      setParameter(s.first,s.second);
    }

    
    return *this;
  }
};
