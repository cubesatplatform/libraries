#include "msg.h"
#include "consoleio.h"

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

double CMsg::getParameter(std::string str,double val){
  std::string res=getParameter(str);
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
  if(getACK()=="0")
    flag=true;
    return flag;
}

std::string CMsg::TransmitData() { 
  std::string ss=serialize(); 
  return ss;
  }

std::string CMsg::serialize() {
  //writeconsoleln("x");
  cleanup();
  //writeconsoleln("y");
  std::string str1;
 
    for (auto it : Parameters  ){
      if(it.second.size()){
        std::string s = it.first + ":" + it.second;
        str1 = s + "~"+str1;
      }
  }
  return str1;
}

std::string CMsg::serializeout() {
  cleanup();
  std::string str1,str2;
    for (auto it : Parameters  ){
      if(it.second.size()){
        std::string s = it.first + ":" + it.second;    
        str1 = s + "~"+str1;
      }
  }

str1="~vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv~"+str1+"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^~";
for(auto c:str1){
  if (c=='~') c='\n';
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
        Parameters[str1.substr(0, i)] = str1.substr(i + 1, 100000);
        break;
      }
    }
  }
  cleanup();
}


bool CMsg::checkPWD(){
  std::string key;

  key=getSYS();  
  key+=getACT();

  if(key.size()==0)
    return false;
  
  std::string pwd,dwp;
  dwp=getParameter("DWP","");
  int seed=getParameter("SD",1);

  pwd=stringDecode(dwp,seed);
  if(key==pwd)
    return true;
  return false;
}

bool CMsg::setPWD(){
  std::string key;
  key=getSYS();
  
  key+=getACT();
  if(key.size()==0)
    return false;

  setParameter("SD",_ID%10);
  std::string pwd=stringEncode(key,_ID);
  setParameter("DWP",pwd);
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