#pragma once
#include <vector>
#include <list>
#include "msg.h"
#include "systemobject.h"

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
  void push_back(CMsg &m);
  void clear();
  bool pop_front(CMsg * pM);

};

class CMessages {
public:  
  CMessageList ReceivedList;     //Received via Radio.  Put inbound received msgs here  ---Need to add acks for these as well
  CMessageList MessageList;     //Moved from Radio to here.  These are pumped out
  CMessageList DataList;       //This is where the one demand data is put to be sent per request.  Most things put data here
  CMessageList TransmitList;     //Add output to transmit here (health, beacons and non on demand stuff go here)    Radio reads from here
  CMessageList TransmittedList;    //Sent  Heep for a bit if then need a resend    Radio writes to here when transmitted

  //std::chrono::time_point<std::chrono::system_clock> starttime;
  

  CMessages(){TransmittedList.maxSize(5);};
  ~CMessages(){};

  CMsg findinDataList(std::string filename,std::string block);

  void moveReceived();
  void movetoTransmitList(CMsg &msg);                                //Need to make a subsystem that moved data to transmitlist data based on requests
  void moveDataToTransmit();
  
  void prune();
  void displayList(int option);  
};

CMessages *getMessages();
