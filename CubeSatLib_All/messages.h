#pragma once
#include <vector>
#include <list>
#include "msg.h"
#include "systemobject.h"


class CMessages {
public:  
  //std::chrono::time_point<std::chrono::system_clock> starttime;
  std::list<CMsg> ReceivedList;     //Received via Radio.  Put inbound received msgs here  ---Need to add acks for these as well
  std::list<CMsg> MessagesList;     //Moved from Radio to here.  These are pumped out
  std::list<CMsg> DataList;       //This is where the one demand data is put to be sent per request.  Most things put data here
  std::list<CMsg> TransmitList;     //Add output to transmit here (health, beacons and non on demand stuff go here)    Radio reads from here
  std::list<CMsg> TransmittedList;    //Sent  Heep for a bit if then need a resend    Radio writes to here when transmitted

   


  CMessages() {};
  ~CMessages() {};


  CMsg findinDataList(std::string filename,std::string block);
  void newMessage(const char* s);
  void newTransmitMessage(const char* s);
  void newTransmitMessage(CMsg &s);
  void newTransmitMessageLarge(CMsg &s) ;
  void addReceivedList(CMsg &s,std::string nameSat="");
  void newMessage(CMsg &s);
  void moveReceived();
  void prune();
  void movetoTransmitList(CMsg &msg);                                //Need to make a subsystem that moved data to transmitlist data based on requests
  void displayList(int option);
  
};

CMessages *getMessages();
