#pragma once

#include "framework_defs.h"
#include <vector>
#include <list>
#include <map>
#include "msg.h"
#include "systemobject.h"

#define MAXLISTSIZE 50
#define MAXDATALEN 170

class CMessageList{
private:
  std::list<CMsg *> MList; 
  int _maxsize=MAXLISTSIZE;
public:   
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
  CMsg find(std::string str);
  CMsg findwRemove(std::string str);
  void clear();
  void prune();

};

class CMessages {
  void _addTransmit(CMsg &m);

  unsigned long _lastReceived=0;
  unsigned long _lastMessage=0;
  unsigned long _lastData=0;
  unsigned long _lastTransmit=0;


public:  
  CMessageList ReceivedList;     //Received via Radio.  Put inbound received msgs here  ---Need to add acks for these as well
  CMessageList MessageList;     //Moved from Radio to here.  These are pumped out
  CMessageList DataList;       //This is where the one demand data is put to be sent per request.  Most things put data here
  CMessageList TransmitList;     //Add output to transmit here (health, beacons and non on demand stuff go here)    Radio reads from here
  CMessageList TransmittedList;    //Sent  Heep for a bit if then need a resend    Radio writes to here when transmitted
  
  std::map<std::string, int> Subscribers;  //Subscribers to a specific system

  //std::chrono::time_point<std::chrono::system_clock> starttime;
  

  CMessages(){TransmittedList.maxSize(5);};
  ~CMessages(){};



  CMsg findinDataList(std::string filename,std::string block);

  void moveReceived();
  void movetoTransmitList(CMsg &msg);                             //Need to make a subsystem that moved data to transmitlist data based on requests
  void sendData(CMsg &msg);            //Similar to movetoTransmitList
  void moveDataToTransmit();

  void subscribe(std::string str);
  void unsubscribe(std::string str);
  int subscribers(std::string str);

  
  void addTransmitList(CMsg &m );
  void addDataList(CMsg &m); 
  void addMessageList(CMsg &m );
  void addReceivedList(CMsg &m );

  std::list<CMsg> splitMsg(CMsg &m);
  std::list<CMsg> splitMsgData(CMsg &m);  

  void setReceivedTimestamp(){_lastReceived=getTime();}
  unsigned long getLastReceived(){return _lastReceived;}
  unsigned long getLastData(){return _lastData;}
  unsigned long getLastMessage(){return _lastMessage;}
  unsigned long getLastTransmit(){return _lastTransmit;}
  
  void prune();
  void displayList(int option);  
};

CMessages *getMessages();
