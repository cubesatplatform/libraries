#include "messages.h"
#include "consoleio.h"
#include <arduino.h>




void CMessages::moveReceived() {
  
  while (ReceivedList.size()){
    CMsg m=ReceivedList.front(); 
    ReceivedList.pop_front();
    
    MessageList.push_back(m);    
  }  
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
    CMsg m=DataList.front();
    DataList.pop_front();
    TransmitList.push_back(m);
  }
  DataList.clear();
}

/////////////////////////////////////////////////////////////////
  


CMessageList::CMessageList(){}
CMessageList::~CMessageList(){}

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

void CMessageList::push_back(CMsg &m){

  CMsg *pM=new CMsg;  
  *pM=m;
  while(MList.size()>_maxsize){
    pop_front();     
    } 
  MList.push_back(pM);
}

void CMessageList::clear(){
  for(auto pM:MList){
    delete pM;
  }
  MList.clear();
}
