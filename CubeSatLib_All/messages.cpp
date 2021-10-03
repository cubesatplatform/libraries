#include "messages.h"
#include "consoleio.h"
#include <arduino.h>


void CMessages::newMessage(const char* s) {
  CMsg m(s);

  ReceivedList.push_back(m);

}


void CMessages::moveReceived() {
 // for (std::list<CMsg>::iterator it = ReceivedList.begin(); it != ReceivedList.end(); ++it) {
   // for (auto it = ReceivedList.begin(); it != ReceivedList.end(); ++it) {
   // CMsg m = *it;
   for (auto m:ReceivedList){
    MessagesList.push_back(m);
  }
  ReceivedList.clear();
}

CMsg CMessages::findinDataList(std::string filename,std::string block){
  CMsg m;
  m.Parameters["FOUND"]="0";


  for(auto x:DataList){
    if((filename==x.Parameters["FILE"])&&(block==x.Parameters["BLK"])){
      x.Parameters["FOUND"]="1";
      return x;
    }
  }
  return m;
}


void  CMessages::movetoTransmitList(CMsg &msg){
  
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
          TransmitList.push_back(m);
          writeconsoleln("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ FOUND IT ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");          
        }
    }
    return;
  }


  if (filename.size()&&block.size()){
    writeconsoleln("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ FOUND ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
    CMsg m=findinDataList(filename,block);
      if(m.Parameters["FOUND"]=="1"){
        TransmitList.push_back(m);
        writeconsoleln("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ FOUND IT ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
        
      }
    return;
  }



  
  int count=0;
  int items=msg.getParameter("ITEMS",100);
  while(count<items&&DataList.size()){
    CMsg m=DataList.front();
    DataList.pop_front();
    TransmitList.push_back(m);
    count++;
  }
}

void CMessages::newMessage(CMsg &m) {
  MessagesList.push_back(m);
}


void CMessages::newTransmitMessage(const char* s) {
  CMsg m(s);

  newTransmitMessage(m);

}



void CMessages::newTransmitMessage(CMsg &s) {
  if(s.serialSize()<TRANSMITMSGLENGTH ){
    TransmitList.push_back(s);
  }
  else  {
   newTransmitMessageLarge(s);
  }
}

void CMessages::newTransmitMessageLarge(CMsg &s) {
  std::string tablestr = "table:" + s.Parameters["table"]+ "~";
  int size=0;
  CMsg m;
  m.Parameters["table"]=s.Parameters["table"];
  size+=tablestr.size();
    

  for (auto it = s.Parameters.begin(); it != s.Parameters.end(); ++it) {    
    std::string tmpstr = it->first + ":" + it->second+ "~";

    if(size>TRANSMITMSGLENGTH ){
      TransmitList.push_back(m);
      //writeconsoleln("TransmitMessageLarge");
      m.Parameters.clear();
      size=0;

      m.Parameters["table"]=s.Parameters["table"];
      size+=tablestr.size();
      }
    m.Parameters[it->first]=it->second;
    size+=tmpstr.size();
    }
  if(size>tablestr.size())
    //writeconsoleln("TransmitMessageLarge");
    TransmitList.push_back(m);
  return;
  
}


void CMessages::prune() {
  const int size = 100;
  const int newsize=60;
  if(TransmitList.size()>size) while (TransmitList.size() > newsize) TransmitList.pop_front();
  if(TransmittedList.size()>size) while (TransmittedList.size() > newsize) TransmittedList.pop_front();
  if(ReceivedList.size()>size) while (ReceivedList.size() > newsize) ReceivedList.pop_front();
  if(MessagesList.size()>size) while (MessagesList.size() > newsize) MessagesList.pop_front();
  if(DataList.size()>size) while (DataList.size() > newsize) DataList.pop_front();
}

void CMessages::displayList(int option=0){
    writeconsoleln();
    writeconsoleln("---------------------------------------------TransmitListLog Begin");
   // for (std::list<CMsg>::iterator it = TransmitList.begin(); it != TransmitList.end(); ++it) {
   //   for (auto it = TransmitList.begin(); it != TransmitList.end(); ++it) {
   //    CMsg m = *it;
    for (auto m:TransmitList){
       writeconsoleln(m.Data());
    }  
    writeconsoleln("---------------------------------------------TransmitListLog End");
    writeconsoleln();
}
