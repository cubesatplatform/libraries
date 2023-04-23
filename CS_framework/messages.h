#pragma once

#include <vector>
#include <list>
#include <map>
#include "msg.h"




class CMessageList {
private:
   
  int _maxsize=MAXLISTSIZE;
  unsigned int _mid;   //Used to make sure we don't get a duplicate db entry and to combine entries when u need to split fields
public:   
  std::list<CMsg > MList;
  CMessageList();
  ~CMessageList();

  int size();
  void maxSize(int tmp=MAXLISTSIZE){_maxsize=tmp;}
  CMsg front();  
  CMsg back();  
  void pop_front();
  void pop_back();
  void push_back(CMsg &m);
  void push_front(CMsg &m);
  CMsg find(std::string str,std::string field=_SYS);
  CMsg findwRemove(std::string str,std::string field=_SYS);
  void clear();
  void prune();
  //void serializeFile(const char * path){ writeMsgList(path,&MList );}
  //void deserializeFile(const char * path){readMsgList(path,&MList );}

};




class CMessages {
private:
  void _addTransmit(CMsg &m);

  long _lastReceived=0;
  long _lastMessage=0;
  long _lastData=0;
  long _lastTransmit=0;
  
  

public:  
  CMessageList ReceivedList;     //Received via Radio.  Put inbound received msgs here  ---Need to add acks for these as well
  CMessageList MessageList;     //Moved from Radio to here.  These are pumped out  
  CMessageList TransmitList;     //Add output to transmit here (health, beacons and non on demand stuff go here)    Radio reads from here
  CMessageList TransmittedList;    //Sent  Heep for a bit if then need a resend    Radio writes to here when transmitted
  CMessageList CloudList;    //Add here if want to save to cloud

  std::map<std::string, CMsg> DataMap;     //This is where the one demand data is put to be sent per request.  Most things put data here
  
  std::map<std::string, int> Subscribers;  //Subscribers to a specific system

  //std::chrono::time_point<std::chrono::system_clock> starttime;
  

  CMessages(){};
  ~CMessages(){};
  
  void moveReceived();                        //Need to make a subsystem that moved data to transmitlist data based on requests
  
  void sendData(CMsg &msg);            //Similar to movetoTransmitList

  void addDMap(std::string key,CMsg &m);   
  void addDMap(CMsg &m); 
  void sendDataMap(CMsg &m); 
  void addTList(CMsg &m );  
  void addMList(CMsg &m );
  void addRList(CMsg &m,std::string strIAM );

  std::list<CMsg> splitMsg(CMsg &m);
  std::list<CMsg> splitMsgData(CMsg &m);  

  void setReceivedTimestamp(){_lastReceived=getTime();}
  long getLastReceived(){return _lastReceived;}
  long getLastData(){return _lastData;}
  long getLastMessage(){return _lastMessage;}
  long getLastTransmit(){return _lastTransmit;}

  void addMessageList(CMsg &m );
  void addReceivedList(CMsg &m );
  void addTransmitList(CMsg &m );  
  void addCloudList(CMsg &m );  
  void addDataMap(std::string key, CMsg &m); 
  void addDataMap(CMsg &m); 
  void setDataMap(CMsg &m){addDataMap(m);}
  void setDataMap(std::string key, CMsg &m){addDataMap(key,m);}; 
  CMsg getDataMap(std::string key); 



  long displayFreeHeap();
  
  void prune();
  void displayList(int option);  
  
  CMsg rawStats(CMsg &msg);
  void clearDataMap();
  
};


