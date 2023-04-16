#pragma once


#include "Arduino.h"
#include <systemdefs.h>
#include <msgconstants.h>
#include <map>
#include <string>
#include <vector>
#include "funcs.h"
#include <mbase64.h>





class CMsg  {
  std::string _str;


  static int _id;       //Count of objects created
  long _tc=getTime();

protected:
  
public:
  std::vector<unsigned char> byteVector;
  std::map<std::string, std::string> Parameters;

  CMsg() {_tc=getTime();_id++;};
  CMsg(std::string s) { config(s);_id++;}
  CMsg(const char* s) { std::string str = s; config(s);_id++;}


  void config(std::string s) { _str = s; _tc=getTime(); deserialize(); }
  
  void decompose(const char* s){ _str = s; deserialize();}
  void clear(){_str=_BLANK; _tc=getTime();Parameters.clear();//byteVector.clear();
  }
  
  std::string serialize();
  std::string serializeout(char delim='\n');
  void writetoconsole();
  void writetoconsoleln();
  void deserialize();
  std::string serializeFile(const char * path=NULL);
  void saveFile();
  void deserializeFile(const char * path);
  int serialSize(){std::string s=serialize();return(s.size());}
  
  std::string getMsg() {return _str;}


  int getStaticID(){return _id;}

  std::string getStringID(){return tostring((long) _id); }

  void remap();

  bool isReadyToProcess();
  bool checkPWD();

  bool isAck() { 
    if  (get(_ACK)== "1") return true; 
    return false; 
    }

  void requestACK(){set(_ACK,"0");}
  void confirmACK(){set(_ACK,"1");}
  
  void setTIMESTAMP();
  
  bool setPWD();  //Returns false if parameters are not there for a real pwd      

  void setREFID() { 
    std::string str=get(_REFID);
    if (!str.size())
      set(_REFID, _id); 
    }  

  void appendParams(std::map<std::string, std::string> &Params);

    

  std::string get(const char * str){return get(std::string(str));}
  std::string get(const char * str, const char * val){return get(std::string(str),val);}
  int get(const char * str,int val){return get(std::string(str),val);}
  long get(const char * str,long val){return get(std::string(str),val);}  
  float get(const char * str,float val){return get(std::string(str),val);}
  double get(const char * str,double val){return get(std::string(str),val);}
  char get(const char * str,char val){return get(std::string(str),val);}


  std::string  get(std::string str){    if (Parameters.find(str) != Parameters.end()) return Parameters[str];     return std::string(_BLANK);    }
  std::string get(std::string str, std::string val);
  std::string get(std::string str, const char * val);
  int get(std::string str,int val);
  long get(std::string str,long val);  
  float get(std::string str,float val);
  double get(std::string str,double val);
  char get(std::string str,char val);


  void set(const char * str,const char * val){ set(std::string(str),std::string(val));    }  
  void set(const char * str,std::string val ) {  set(std::string(str),val);    }  
  void set(const char * str,int val ) {  set(std::string(str),val);    }  
  void set(const char * str,long val ) {  set(std::string(str),val);    }    
  void set(const char * str,float val ) {  set(std::string(str),val);    }  
  void set(const char * str,double val ) {  set(std::string(str),val);    }  
  void set(const char * str,char val ) {  set(std::string(str),val);    }  


  void set(std::string str,const char * val){ Parameters[str]=std::string(val);    return;    }  
  void set(std::string str,std::string val ) { Parameters[str]=val; }
  void set(std::string str,int val ) { Parameters[str]=tostring((long) val); }
  void set(std::string str,long val ) { Parameters[str]=tostring( val); }  
  void set(std::string str,float val ) { Parameters[str]=tostring( val); } 
  void set(std::string str,double val ) { Parameters[str]=tostring( val); } 
  void set(std::string str,char val ) { Parameters[str]=tostring( val); }   


  std::string Data() { return _str; }
  void Data(std::string s) { _str = s; }
  
  std::string TransmitData();
  bool needACK();
     
  void initArray(unsigned char* myRawArray, int byteCount);

  //unsigned char *vectorData(){return byteVector.data();};  
  //int vectorLen(){return byteVector.size();}


  void cleanup();
  std::string payload();
  std::string supabase(std::string act="eq.");

  CMsg operator+=(const CMsg& b) {    
      //for(auto s:b.byteVector){
      //  byteVector.push_back(s);
      //}

    for(auto s:b.Parameters){
      set(s.first,s.second);
    }

    return *this;
  }
};
