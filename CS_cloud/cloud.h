#pragma once

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#include "messages.h"


#include <systemobject.h>
#include <ArduinoJson.h>

#define CMD_INTERVAL 10000
typedef StaticJsonDocument<1200> jsonDoc;  

class CCloud:public CSystemObject {
private:
  
  long _lastCmd = 0;
  bool _bRegistered=false;
  
  //const char* host = "9nxvzfnhrd.execute-api.us-east-1.amazonaws.com";
  std::string host = "http://192.168.86.148";
  

  //const int httpsPort = 443;
  const int httpsPort = 80;
  const int serialBaudRate = 115200;
  const int delayWaitingForWifiConnection = 500;
   
public:

  std::map<std::string, std::string> URLS;
  
  //CMessages *pMessages=NULL;
  WiFiClientSecure client;
  WiFiMulti wifiMulti;
  
  //ConsoleEcho SerialB;
  
  CCloud();
  void setup(){
    connectWifi();  
    setState(_PLAY);
    setInterval(100);//pMessages=getMessages();
  }
  void loop();

  void connectWifi();  

  std::string fillurl(std::string original,CMsg &msg);
  
  unsigned char h2int(char c);
  String urldecode(String str);
  String urlencode(String str);
  std::string urlencode(std::string str);
  std::string urldecode(std::string str);
  std::string convertStringToCommand(std::string str);  
  void registerBS();

  CMsg callAPI(CMsg &msg);    
  void save(CMsg &msg);    
  void getCommand();
  std::string getPage(std::string url="https://example.org/");
  std::string getPage(CMsg &msg);
  std::string getPageMulti(CMsg &msg);
  
  CMsg getJSONDoc(std::string strJSON, jsonDoc &doc);
  
  void callCustomFunctions(CMsg &msg); 

  CMsg receiveFile();
};
