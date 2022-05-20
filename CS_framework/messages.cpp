#include "messages.h"
#include "consoleio.h"

#define ALL "ALL"

void CMessages::moveReceived() {
  
  while (ReceivedList.size()){
    CMsg m=ReceivedList.back(); 
    ReceivedList.pop_front();
    
    MessageList.push_back(m);    
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
  


CMessageList::CMessageList(){}
CMessageList::~CMessageList(){}


CMsg CMessageList::find(std::string str,std::string field){
std::string tmpstr;
CMsg *pmsg;
CMsg m;
for (auto it = MList.begin(); it != MList.end(); ++it) {  
  pmsg=*it;

  
  
  tmpstr=pmsg->getParameter(field);
  if(str==tmpstr){    
    return *pmsg;
    }
  
  }		
  return m;		
}

CMsg CMessageList::findwRemove(std::string str,std::string field){
std::string tmpstr;
CMsg m;
CMsg *pmsg;
for (auto it = MList.begin(); it != MList.end(); ++it) {  
  pmsg=*it;
  
  tmpstr=pmsg->getParameter(field);
  if(str==tmpstr){      
    m=*pmsg;
    MList.erase(it);   //Check to make sure this works		
    return m;      
    }
  }		
  return m;		
}

void CMessageList::prune(){
  if(size()>_maxsize){
    while (size() > (_maxsize-15)) pop_back();
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
  CMsg *pM=MList.front();
  return *pM;
};

CMsg CMessageList::back(){    
  if(MList.size()==0){
    CMsg m;
    return m;
  }
  CMsg *pM=MList.back();
  return *pM;
};

void CMessageList::pop_front(){
  
  if(MList.size()==0){
    return;
  }
  CMsg *pM=MList.front();
  
  delete pM;
  MList.pop_front();
}



void CMessageList::pop_back(){
  
  if(MList.size()==0){
    return;
  }
  CMsg *pM=MList.back();
  
  delete pM;
  MList.pop_back();
}

void CMessageList::push_back(CMsg &m){
  if(m.Parameters.size()==0){
    writeconsoleln("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Empty Message Push_Back xxxxxxxxxxxxxxxxxxxxxxx");
    return;
  }

  CMsg *pM=new CMsg;  
  *pM=m;
  while(MList.size()>_maxsize){
    pop_front();     
    } 
  MList.push_back(pM);
}



void CMessageList::push_front(CMsg &m){
  if(m.Parameters.size()==0){
    writeconsoleln("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Empty Message Push_Front xxxxxxxxxxxxxxxxxxxxxxx");
    return;
  }

  CMsg *pM=new CMsg;  
  *pM=m;
  while(MList.size()>_maxsize){
    pop_back();     
    } 
  MList.push_front(pM);
}


void CMessageList::clear(){
  for(auto pM:MList){
    delete pM;
  }
  MList.clear();
}




//////////////////////////////////////////////////////////////////////////////////
void CMessages::addMessageList(CMsg &m){
  _lastMessage=getTime();
  MessageList.push_back(m);
  return;

}

void CMessages::addReceivedList(CMsg &s,std::string strIAM){
  _lastReceived=getTime();
  if(!s.checkPWD()){    
    s.setCOMMENT("Message PWD Invalid   Dropping   ------- Problably should add to some Log");
    writeconsoleln(s.serializeout());    
    return;
  }
  
  
  if((s.getTO()==strIAM)||(s.getTO()==ALL)){
     ReceivedList.push_back(s);
     return;
  }
  else {
    s.setCOMMENT("Message Received not for this Satellite   Dropping");
    writeconsoleln(s.serializeout());    
  }
  return;
}


void CMessages::_addTransmit(CMsg &m){     
    _lastTransmit=getTime();
    
    m.setPWD();

    m.cleanup();
    TransmitList.push_back(m);
    return;
}




std::list<CMsg> CMessages::splitMsg(CMsg &m){
  std::list<CMsg> lM;

  std::string strSYS=m.getSYS();
  std::string strACT=m.getACT();
  std::string strFROM=m.getFROM();
  std::string strTO=m.getTO();
  
  CMsg cm;
  int totsize=0;
  int part=0;

  long ltmp=getTime();
  ltmp=ltmp%1000;

  
  for(auto x:m.Parameters){
    cm.setParameter("UID",tostring(ltmp)+std::string("_")+tostring(part));
    std::string tmpstr;
    tmpstr=cm.serialize();
    totsize=tmpstr.size();
    
    std::string str=x.first+x.second+"~:";
    int size=str.size();

    if((size+totsize)<200){  //&&(x==m.Parameters.end())){
  
      std::string sfirst=x.first;
      std::string ssecond=x.second;
      cm.setParameter(sfirst,ssecond);  

    }
    else{
      
      cm.setSYS(strSYS);
      cm.setACT(strACT);
      cm.setFROM(strFROM);
      cm.setTO(strTO);
      //cm.setParameter("PT",part);
      lM.push_back(cm);
      part++;
      cm.clear();
    }
  }


  if(cm.serialize().size())
    {
      cm.setSYS(strSYS);
      cm.setACT(strACT);
      cm.setFROM(strFROM);
      cm.setTO(strTO);
      //cm.setParameter("PT",part);
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
  std::string data=m.getDATA();

  int datasize=data.size();

  if(datasize>0){
    for(int count=0;count<datasize;){
      int len=MAXDATALEN;
      if(len>(datasize-count))
        len=(datasize-count);

      tmpstr=data.substr(count,len);
      cm.setDATA(tmpstr);
      cm.setOFFSET(offset);
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



void CMessages::addTransmitList(CMsg &m){

std::list<CMsg> lMD;

lMD=splitMsgData(m);
//writeconsoleln(m.serializeout());

for(auto x:lMD){
  //writeconsoleln(x.serializeout());
  std::list<CMsg> lM=splitMsg(x);  
    for(auto y:lM){
    _addTransmit(y);  
    }  
  }
  return;
}



void CMessages::addDataMap(std::string key,CMsg &m) {
  _lastData=getTime();
  DataMap[key]=m;  
  }


void  CMessages::sendData(CMsg &msg){    
  
 
  std::string key=msg.getKEY();
  CMsg m=DataMap[key];
  if(m.Parameters.size())
    addTransmitList(m);
}

