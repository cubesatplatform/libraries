#include "msg.h"
#include "consoleio.h"

int CMsg::_id = millis()%1000;

void CMsg::setTIMESTAMP(){ 
  long tmp=millis();
  set(_TIME,tmp); 
}

void CMsg::writetoconsole(){
  std::string str=serializeout();
  if(str.size()>255) {
    str=str.substr(0,255);
    str+="...";
  }
  writeconsoleln(str);
  }


void CMsg::writetoconsoleln(){
  std::string str=serializeout('\t');
  if(str.size()>255) {
    str=str.substr(0,255);
    str+="...";
  }
  writeconsoleln(str);
  }

std::string CMsg::get(std::string str,const char * val){
  std::string res=get(str);
  if (res.size()==0) res=val;
  return res;
}

std::string CMsg::get(std::string str,std::string val){
  std::string res=get(str);
  if (res.size()==0) res=val;
  return res;
}

char CMsg::get(std::string str,char val){
  std::string res=get(str);
  if (res.size()==0) res=val;
  return res[0];
}

long CMsg::get(std::string str,long val){
  std::string res=get(str);
  if (res.size()==0) return(val);
  return atol(res.c_str());
}



int CMsg::get(std::string str,int val){
  std::string res=get(str);
  if (res.size()==0) return(val);
  return atol(res.c_str());
}

float CMsg::get(std::string str,float val){
  std::string res=get(str);
  if (res.size()==0) return(val);
  return atof(res.c_str());
}

double CMsg::get(std::string str,double val){
  std::string res=get(str);
  if (res.size()==0) return(val);
  return atof(res.c_str());
}

void CMsg::appendParams(std::map<std::string, std::string> &Params){
  for (auto x:Params){
    if(x.second.size())
      Parameters[x.first]=x.second;
  }
}

bool CMsg::needACK(){
  bool flag=false;
  if(get(_ACK)=="0"){
    flag=true;
  }
  return flag;
}

bool CMsg::isReadyToProcess(){
  long ts=get(_RECEIVEDTS,0L);
  long pt=get(_PROCESSTIME,0L);
  long ct=getTime();

  if((ts+pt)< ct)
    return true;
  return false;
}

std::string CMsg::TransmitData() { 
  std::string ss=serialize(); 
  return ss;
  }

std::string CMsg::serialize() {

  cleanup();

  std::string str1;
 
    for (auto it : Parameters  ){
      if(it.second.size()){
        std::string s = it.first + ":" + it.second;
        str1 = s + "~"+str1;
      }
  }
  return str1;
}


std::string CMsg::payload() {
  std::string str="{";
  for (auto x : Parameters ){
    if(x.second.size()){
      str+="\"";
      str+=x.first.c_str();
      str+="\":";
      str+="\"";
      str+=x.second.c_str();          
      str+="\"";
      str+=",";
    }
  }

  if(str.size()>1)
    str=str.substr(0,str.size()-1);

  str+="}";
  return str;
}




std::string CMsg::supabase(std::string act) {
  std::string str;
  for (auto x : Parameters ){
    if(x.second.size()){
      
      str+=x.first.c_str();
      str+="=";
      str+=act;
    //  str+="\"";
      str+=x.second.c_str();          
    //  str+="\"";
      str+="&";
    }
  }
  
  return str;
}

std::string CMsg::serializeout(char delim) {
  cleanup();
  std::string str1,str2;
    for (auto it : Parameters  ){
      if(it.second.size()){
        std::string s = it.first + ":" + it.second;    
        str1 = s + "~"+str1;
      }
  }

//str1="~vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv~"+str1+"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^~";
for(auto c:str1){
  if (c=='~') c=delim;
  str2+=c;
}

  return str2;
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

  for (unsigned int i = 0; i < _str.size(); i++) {
    c = _str[i];
    if (c == tok1) {
      stop = i;
      components.push_back(_str.substr(start, stop - start));

      start = i + 1;
    }
  }


  for (unsigned int x = 0; x < components.size(); x++) {
    str1 = components[x];
    for (unsigned int i = 0; i < str1.size(); i++) {
      c = str1[i];

      if (c == tok2) {
        stop = i;
        Parameters[str1.substr(0, i)] = str1.substr(i + 1, 100000);
        break;
      }
    }
  }
  if (Parameters.size() == 0) {
    std::string name="XX";
    name+=getStringID();
    set(_NAME,name);
    set(_VALUE,_str);
  }
  cleanup();
}


bool CMsg::checkPWD(){
  //if(vectorLen())     return true;
  std::string key;

  key=get(_SYS,"NONE");  
  key+=get(_ACT,"NONE");

  if(key.size()==0)
    return false;
  
  std::string pwd,dwp;
  dwp=get(_PWD,_BLANK);
  int seed=get(_SD,1);

  pwd=stringDecode(dwp,seed);
  if(key==pwd)
    return true;
  return false;
}

bool CMsg::setPWD(){
  std::string key;
  key=get(_SYS,"NONE");  
  key+=get(_ACT,"NONE");
  if(key.size()==0)
    return false;

  set(_SD,_id%10);
  std::string pwd=stringEncode(key,_id);
  set(_PWD,pwd);
  return true;
}

void CMsg::cleanup(){

for (auto it = Parameters.begin(); it != Parameters.end(); ++it) {  
  std::string str;
  str=it->second;
  if(str.size()<1){
    Parameters.erase(it);   //Check to make sure this works			 
  }
  }				
}



std::string CMsg::serializeFile(const char * path){
  std::string strFN;
  
  #if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
  if(path==NULL){
    std::string strID=tostring(getStaticID());
    std::string strSys=get(_SYS);

    strFN=strSys;
    strFN+=std::string("_");
    strFN+=strID;
    strFN+=std::string(".msg");
  }
  else
    strFN=path;

  std::string str=serialize();
  /*
  if(vectorLen()){
    bool flag=false;
    for(auto count=0;count<strFN.size();count++){
      if(strFN[count]=='.') flag=true;
    }
    if(!flag) strFN+=".bin";
    writeFile(strFN.c_str(), byteVector.data(), vectorLen());
  }
  else{
    writeFile(strFN.c_str(), (const unsigned char *)str.c_str(), str.size());
    
  }
*/
 // writeFile(strFN.c_str(), (const unsigned char *)str.c_str(), str.size());
  #endif
  return strFN;

}


void CMsg::deserializeFile(const char * path){
#if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
writeconsoleln(path);
  std::string str; //str=readFile(path);
  if(str.size()){
    writeconsoleln(str);
    decompose(str.c_str());
  }
#endif
}

void CMsg::saveFile(){
  writeconsoleln("Call to Write File");
  std::string filename=get(_FILENAME);
  if(filename.size())
    serializeFile(filename.c_str());
  else
    serializeFile();
}



void CMsg::initArray(unsigned char* myRawArray, int byteCount) {
  writeconsoleln("Init Arraying");

  /*
    for(int count=0;count<byteCount;count++){
     // byteVector.push_back(buffer[count]);
      unsigned char c=myRawArray[count];
      byteVector.push_back(c);       
      writeconsole((char)myRawArray[count]);
  } 
  */ 
}

void CMsg::remap(){
  std::string sys = get(_SYS);
	std::string act = get(_ACT);

  if(sys.length()){
    set(_SYS,sys);
    set(_SYS,_BLANK);
  }

  if(act.length()){
    set(_ACT,act);
    set(_ACT,_BLANK);
  }
}