#include "messages.h"
#include "consoleio.h"
#include <arduino.h>




void CMessages::moveReceived() {
  
  while (ReceivedList.size()){
    CMsg m=ReceivedList.back(); 
    ReceivedList.pop_back();
    
    MessageList.push_front(m);    
  }  
}

void  CMessages::sendData(CMsg &msg){
  //SYS:SAT
  //ACT:SENDDATA

  CMsg m;
  std::string d=msg.getDATA();
  if(d=="")
    return;
  int count=msg.getParameter("COUNT",1);
  m.setSYS(d);  
 
  if(d=="TIME"){
    m.setSYS("SAT");
    m.setParameter("TIME",getTime());
    addTransmitList(m);
    return;
  }

  if(d=="IRARRAY"){
    CMsg x1=DataList.find("IRARRAYX1");
    addTransmitList(x1);
    CMsg x2=DataList.find("IRARRAYX2");
    addTransmitList(x2);

    CMsg y1=DataList.find("IRARRAYY1");
    addTransmitList(y1);
    CMsg y2=DataList.find("IRARRAYY2");
    addTransmitList(y1);

    CMsg z1=DataList.find("IRARRAYZ1");
    addTransmitList(z1);
    CMsg z2=DataList.find("IRARRAYZ2");
    addTransmitList(z1);
    return;

  }
  if(d=="TEMP"){
    CMsg x1=DataList.find("TEMPX1");
    CMsg x2=DataList.find("TEMPX2");

    CMsg y1=DataList.find("TEMPY1");
    CMsg y2=DataList.find("TEMPY2");

    CMsg z1=DataList.find("TEMPZ1");
    CMsg z2=DataList.find("TEMPZ2");
    
    m.setParameter("TEMPX1",x1.getParameter("TEMP"));
    m.setParameter("TEMPX1T",x1.getParameter("TIME"));

    m.setParameter("TEMPY1",y1.getParameter("TEMP"));
    m.setParameter("TEMPY1T",y1.getParameter("TIME"));

    m.setParameter("TEMPZ1",z1.getParameter("TEMP"));
    m.setParameter("TEMPZ1T",z1.getParameter("TIME"));
    addTransmitList(m);
  }

m=DataList.find(d);    
addTransmitList(m);

}

CMsg CMessages::findinDataList(std::string filename,std::string block){
  CMsg m;
  /*
  m.Parameters["FOUND"]="0";

  for(auto x:DataList){
    if((filename==x->Parameters["FILE"])&&(block==x->Parameters["BLK"])){
      x->Parameters["FOUND"]="1";
      return *x;
    }
  }
  */
  return m;
}


void  CMessages::movetoTransmitList(CMsg &msg){
  /*
  std::string filename=msg.Parameters["FILE"];
  std::string block=msg.Parameters["BLK"];
  std::string blockend=msg.Parameters["BLKEND"];


  if (filename.size()&&block.size()&&blockend.size()){
    writeconsoleln("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ FOUND END ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
    int startBlock=msg.getParameter("BLK",0);
    int endBlock=msg.getParameter("BLKEND",0);

    for (int x=startBlock;x<=endBlock;x++){
      CMsg m=findinDataList(filename,tostring(x));
        if(m.Parameters["FOUND"]=="1"){
          push_backTransmitList(m);
          writeconsoleln("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ FOUND IT ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");          
        }
    }
    
    return;
  }


  if (filename.size()&&block.size()){
    writeconsoleln("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ FOUND ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
    CMsg m=findinDataList(filename,block);
      if(m.Parameters["FOUND"]=="1"){
        push_backTransmitList(m);
        writeconsoleln("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ FOUND IT ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
        
      }
    return;

  }


  int count=0;
  int items=msg.getParameter("ITEMS",100);
  while(count<items&&DataList.size()){
    CMsg m=frontDataList();
    pop_frontDataList();
    push_backTransmitList(m);
    count++;
  }
  */
}



void CMessages::prune() {
  const int size = 100;
  const int newsize=60;
  if(TransmitList.size()>size) while (TransmitList.size() > newsize) TransmitList.pop_front();
  if(TransmittedList.size()>size) while (TransmittedList.size() > newsize) TransmittedList.pop_front();
  if(ReceivedList.size()>size) while (ReceivedList.size() > newsize) ReceivedList.pop_front();
  if(MessageList.size()>size) while (MessageList.size() > newsize) MessageList.pop_front();
  if(DataList.size()>size) while (DataList.size() > newsize) DataList.pop_front();
}

void CMessages::displayList(int option=0){
   
}




void CMessages::moveDataToTransmit(){

   while (DataList.size()){
    CMsg m=DataList.back();
    DataList.pop_back();
    TransmitList.push_front(m);
  }
  DataList.clear();
}


void  CMessages::subscribe(std::string str){
    Subscribers[str]=Subscribers[str]+1;

}

int  CMessages::subscribers(std::string str){
    return(Subscribers[str]);

}



void  CMessages::unsubscribe(std::string str){
  Subscribers[str]=Subscribers[str]-1;
  if(Subscribers[str]<0)
    Subscribers[str]=0;
  
}

/////////////////////////////////////////////////////////////////
  


CMessageList::CMessageList(){}
CMessageList::~CMessageList(){}


CMsg CMessageList::find(std::string str){
std::string tmpstr;
CMsg *pmsg;
CMsg m;
for (auto it = MList.begin(); it != MList.end(); ++it) {  
  pmsg=*it;

  
  
  tmpstr=pmsg->getSYS();
  if(str==tmpstr){    
    return *pmsg;
    }
  
  }		
  return m;		
}

CMsg CMessageList::findwRemove(std::string str){
std::string tmpstr;
CMsg m;
CMsg *pmsg;
for (auto it = MList.begin(); it != MList.end(); ++it) {  
  pmsg=*it;
  
  tmpstr=pmsg->getSYS();
  if(str==tmpstr){      
    m=*pmsg;
    MList.erase(it);   //Check to make sure this works		
    return m;      
    }
  }		
  return m;		
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

  CMsg *pM=new CMsg;  
  *pM=m;
  while(MList.size()>_maxsize){
    pop_front();     
    } 
  MList.push_back(pM);
}



void CMessageList::push_front(CMsg &m){

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
  
  MessageList.push_front(m);
  return;

}

void CMessages::addReceivedList(CMsg &s){
  
  if(!s.checkPWD()){    
    CMsg m;
    m.setCOMMENT("Message PWD Invalid   Dropping   ------- Problably should add to some Log");
    writeconsoleln(s.serializeout());
    //addTransmitList(s);
    return;
  }
  
  
  if((s.getSAT()==SATNAME)|| (s.getSAT()=="") ){
     ReceivedList.push_front(s);
     return;
  }
  else {
    CMsg m;
    m.setCOMMENT("Message Received not for this Satellite   Dropping");
    writeconsoleln(s.serializeout());
    //addTransmitList(s);
  }
  return;
}


void CMessages::_addTransmit(CMsg &m){     
    
    
    m.setPWD();

    m.cleanup();
    TransmitList.push_front(m);
    return;
}




std::list<CMsg> CMessages::splitMsg(CMsg &m){
  std::list<CMsg> lM;

  std::string strSYS=m.getSYS();
  std::string strACT=m.getACT();
  std::string strSAT=m.getSAT();
  

  CMsg cm;
  int totsize=0;
  int part=0;
  
  
  for(auto x:m.Parameters){
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
      cm.setSAT(strSAT);
      cm.setParameter("PT",part);
      lM.push_front(cm);
      part++;
      cm.clear();
    }
  }


  if(cm.serialize().size())
    {
      cm.setSYS(strSYS);
      cm.setACT(strACT);
      cm.setSAT(strSAT);
      cm.setParameter("PT",part);
      lM.push_front(cm);
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
      lM.push_front(cm);

      count+=MAXDATALEN;
      offset++;
    }
  }
  else{
    lM.push_front(m);
  }
  return lM;
}



void CMessages::addTransmitList(CMsg &m){

std::list<CMsg> lMD;

lMD=splitMsgData(m);

for(auto x:lMD){
  std::list<CMsg> lM=splitMsg(x);  
    for(auto y:lM){
    _addTransmit(y);  
    }  
  }
  return;
}



void CMessages::addDataList(CMsg &m) {
  
  DataList.push_front(m);
  return;
  }

