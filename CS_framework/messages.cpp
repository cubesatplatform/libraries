#include "messages.h"
#include "consoleio.h"


long CMessages::displayFreeHeap(){   ///NO Add TransmitList here
long lFree=0;
 #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
 #else
    lFree=ESP.getFreeHeap();
    writeconsole("Free Heap: "); writeconsoleln((long)lFree);    
  #endif
  return lFree;
}

void CMessages::moveReceived() {
  
  while (ReceivedList.size()){
    CMsg m=ReceivedList.front(); 
    ReceivedList.pop_front();
    if(m.get(_WRITE)=="1"){
      m.saveFile();
    }
    else {
      MessageList.push_back(m);    
      if(_saveToCloud){
        std::string strs=m.get(_SYS);
        if(strs.size()) m.set("_SYS",strs);

        std::string stra=m.get(_ACT);
        if(stra.size()) m.set("_ACT",stra);

        m.set(_SYS,_CLOUD);
        m.set(_ACT,_SAVE);
        MessageList.push_back(m);    
      }

    }
  } 
  

}


void CMessages::prune() { 
  TransmitList.prune();
  TransmittedList.prune();
  ReceivedList.prune();
  MessageList.prune();
 // DataMap.prune();
}

void CMessages::displayList(int option=0){
   
}



/////////////////////////////////////////////////////////////////
  


CMessageList::CMessageList(){_mid=0;}
CMessageList::~CMessageList(){}





CMsg CMessageList::find(std::string str,std::string field){
std::string tmpstr;

CMsg m,msg;
for (auto it = MList.begin(); it != MList.end(); ++it) {  
  m=*it;

  
  
  tmpstr=m.get(field);
  if(str==tmpstr){    
    return m;
    }
  
  }		
  return msg;		
}

CMsg CMessageList::findwRemove(std::string str,std::string field){
std::string tmpstr;
CMsg m,msg;

for (auto it = MList.begin(); it != MList.end(); ++it) {  
  msg=*it;
  
  tmpstr=msg.get(field);
  if(str==tmpstr){      
    m=msg;
    //MList.erase(it);   //Check to make sure this works		
    return m;      
    }
  }		
  return m;		
}

void CMessageList::prune(){
  if(size()>_maxsize){
    while (size() > (_maxsize-15)) pop_front();
  }
}

int CMessageList::size(){
  return MList.size();
}

CMsg CMessageList::front(){    
  if(MList.size()==0){
    CMsg m;
    return m;
  }

  return MList.front();
};

CMsg CMessageList::back(){    
  if(MList.size()==0){
    CMsg m;
    return m;
  }
  
  return MList.back();
};

void CMessageList::pop_front(){
  
  if(MList.size()==0){
    return;
  }

  MList.pop_front();

}



void CMessageList::pop_back(){
  
  if(MList.size()==0){
    return;
  }

  MList.pop_back();

}

void CMessageList::push_back(CMsg &m){  
  if(m.Parameters.size()==0){
    writeconsoleln("Empty Message Push_Back");
    return;
  }
  _mid++;


  if((int)MList.size()>=_maxsize){
    prune();     
    } 
  MList.push_back(m);
  return;
}



void CMessageList::push_front(CMsg &m){  
  if(m.Parameters.size()==0){
    writeconsoleln("Empty Message Push_Front");
    return;
  }

  _mid++;


   if((int)MList.size()>=_maxsize){
    prune();     
    } 
 MList.push_front(m);
}


void CMessageList::clear(){
  

  MList.clear();
}




//////////////////////////////////////////////////////////////////////////////////
void CMessages::addMList(CMsg &m){
  writeconsoleln("addMessageList");
  m.writetoconsole();   
  #if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
  displayFreeHeap();
  #endif
  _lastMessage=getTime();
  m.set(_RECEIVEDTS,_lastMessage);
  m.set(_MID,m.getStaticID());
  MessageList.push_back(m);
  return;

}

void CMessages::addRList(CMsg &s,std::string strIAM){
  _lastReceived=getTime();
  s.set(_RECEIVEDTS,_lastReceived);
  setReceivedTimestamp();
  if(!s.checkPWD()){    
    s.set(_COMMENT,"Message PWD Invalid   Dropping   ------- Problably should add to some Log");
    s.writetoconsole();    
    return;
  }
  
  if(s.get(_TO)==_CLOUD) {s.set(_SYS,_CLOUD);s.set(_ACT,_SAVE);}
  

  ReceivedList.push_back(s);
  return;
  }
  


void CMessages::_addTransmit(CMsg &m){     
    _lastTransmit=getTime();
     //Not setting pwd or anything   need some logic in there
    //if(m.vectorLen()){        TransmitList.push_back(m);        return;    }
    
    m.setPWD();

    m.cleanup();
    TransmitList.push_back(m);
    return;
}




std::list<CMsg> CMessages::splitMsg(CMsg &m){
  //writeconsoleln("splitMsg");
  std::list<CMsg> lM;

  std::string strSYS=m.get(_SYS);
  std::string strACT=m.get(_ACT);
  std::string strFROM=m.get(_FROM);
  std::string strTO=m.get(_TO);
  
  CMsg cm;
  int totsize=0;
  int part=0;

  long ltmp=getTime();
  ltmp=ltmp%1000;

  
  for(auto x:m.Parameters){
    cm.set(_UID,tostring(ltmp)+std::string("_")+tostring(part));
    std::string tmpstr;
    tmpstr=cm.serialize();
    totsize=tmpstr.size();
    
    std::string str=x.first+x.second+"~:";
    int size=str.size();

    if((size+totsize)<200){  //&&(x==m.Parameters.end())){
  
      std::string sfirst=x.first;
      std::string ssecond=x.second;
      cm.set(sfirst,ssecond);  

    }
    else{
      
      cm.set(_SYS,strSYS);
      cm.set(_ACT,strACT);
      cm.set(_FROM,strFROM);
      cm.set(_TO,strTO);
      //cm.set(PT,part);
      lM.push_back(cm);
      part++;
      cm.clear();
    }
  }


  if(cm.serialize().size())
    {
      cm.set(_SYS,strSYS);
      cm.set(_ACT,strACT);
      cm.set(_FROM,strFROM);
      cm.set(_TO,strTO);
      //cm.set(PT,part);
      lM.push_back(cm);
      part++;
      cm.clear();
    }
  
  return lM;
}

std::list<CMsg> CMessages::splitMsgData(CMsg &m){
  int offset=0;
  std::list<CMsg> lM;

  CMsg cm;
  cm=m;

  std::string tmpstr;
  std::string data=m.get(_DATA);

  int datasize=data.size();

  if(datasize>0){
    for(int count=0;count<datasize;){
      int len=MAXDATALEN;
      if(len>(datasize-count))
        len=(datasize-count);

      tmpstr=data.substr(count,len);
      cm.set(_DATA,tmpstr);
      cm.set(_OFFSET,offset);
      lM.push_back(cm);

      count+=MAXDATALEN;
      offset++;
    }
  }
  else{
    lM.push_back(m);
  }
  return lM;
}



void CMessages::addTList(CMsg &m){  
displayFreeHeap(); 
std::list<CMsg> lMD;


lMD=splitMsgData(m);

for(auto x:lMD){
  
  std::list<CMsg> lM=splitMsg(x);  
    for(auto y:lM){
    y.set(_MID,m.get(_MID));    
    _addTransmit(y);  
    }  
  }
  return;
  
}



void CMessages::addDMap(std::string key,CMsg &m) {
  _lastData=getTime();
  DataMap[key]=m;  
  }

void CMessages::addDMap(CMsg &m) {  
  std::string key=m.get(_NAME);
  if (key==_BLANK) key=m.getStringID();
  addDataMap(key,m);
  }  


void  CMessages::sendDataMap(CMsg &msg){   
    for (auto x:DataMap){
       CMsg m=DataMap[x.first];
       writeconsoleln(x.first);
       m.writetoconsole();
       addTransmitList(m);
   
  }
}

void  CMessages::sendData(CMsg &msg){   
  std::string key=msg.get(_NAME);

  if(key.size()==0)
    return;

  int start=msg.get(_START,-1);
  int end=msg.get(_END,-1);

  if((start<0)||(end<0)){
    CMsg m=DataMap[key];
    if(m.Parameters.size()){
      m.set(_NAME,key);
      addTransmitList(m);
      }
    }
  else{
    for(int count=start;count<end;count++){
      std::string key1=key+tostring(count);
      CMsg m=DataMap[key1];
      if(m.Parameters.size()){
        m.set(_NAME,key1);
        addTransmitList(m);
      }
    }
  }
}




void CMessages::clearDataMap(){
  DataMap.clear();


}



  
void CMessages::addTransmitList(CMsg &m ){
  
  //fillMetaMSG(&m);    
    
  addTList(m);


  if(_saveToCloud){
    m.set(_SYS,_CLOUD);
    m.set(_ACT,_SAVE);
    addMList(m);
  }


  return;

}



void CMessages::addDataMap(std::string key,CMsg &m){


  if(key.size()>1)    
    addDMap(key,m);  
  else  
    addDMap(m);  
    
}

void CMessages::addDataMap(CMsg &m){
  std::string key=m.get(_NAME);
  addDataMap(key, m);  
}

CMsg CMessages::getDataMap(std::string key){
  
  CMsg m;
  if(key.size()>1)    
    m=DataMap[key];
  if(m.Parameters.size()==0){
    m.set(_NAME,key);
  }
  
  return m;
}

void CMessages::addMessageList(CMsg &m ){

  addMList(m);
  
}

void CMessages::addReceivedList(CMsg &m ){
  
 // addRList(m.getIAM());
  

}