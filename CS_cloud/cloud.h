#pragma once

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#include "messages.h"


#include <systemobject.h>
#include <ArduinoJson.h>

#define CMD_INTERVAL 5000
typedef StaticJsonDocument<1200> jsonDoc;

class CCloud:public CSystemObject {
private:
  
  unsigned long _lastCmd = 0;
  
  //const char* host = "9nxvzfnhrd.execute-api.us-east-1.amazonaws.com";
  std::string host = "http://192.168.86.148";
  

  //const int httpsPort = 443;
  const int httpsPort = 80;
  const int serialBaudRate = 115200;
  const int delayWaitingForWifiConnection = 500;
   
public:

  std::map<std::string, std::string> URLS;
  
  CMessages *MSG;
  WiFiClientSecure client;
  WiFiMulti wifiMulti;
  
  //ConsoleEcho SerialB;
  
  CCloud();
  void setup(){connectWifi();  setState("PLAY");setInterval(10);}
  void loop();

  void connectWifi();  

  std::string fillurl(std::string original,CMsg &msg);
  
  unsigned char h2int(char c);
  String urldecode(String str);
  String urlencode(String str);
  std::string convertStringToCommand(std::string str);
  void updateAllTransmittedCommands();
  void updateAllReceivedCommands();

  CMsg callAPI(CMsg &msg);    
  
  std::string getPage(std::string URL="https://example.org/");
  CMsg getJSONDoc(std::string strJSON, jsonDoc &doc);
  
  void callCustomFunctions(CMsg &msg); 

  CMsg receiveFile();
};
