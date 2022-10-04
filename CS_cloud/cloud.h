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
  const std::string getcmdurl = host + std::string("/getcmd?bsid=$bsid");
  const std::string updatecmdurl = host + std::string("/updatecmd?cid=$cid&bsid=$bsid");
  const std::string insertcmdurl = host + std::string("/insertcmd?sent=1&cid=$cid&bsid=$bsid&sendon=$sendon&data=$data");
  const std::string updateackurl = host + std::string("/updateack?ack=1&cid=$cid&bsid=$bsid");
  const std::string inserturl =  host + std::string("insert?bid=1");
  const std::string queryurl =  host + std::string("query?bid=1");
  const std::string consolelogpath =host + std::string("/v1/errorlog?bid=1&data=");
  //const int httpsPort = 443;
  const int httpsPort = 80;
  const int serialBaudRate = 115200;
  const int delayWaitingForWifiConnection = 500;
   
public:
  CMessages *MSG;
  WiFiClientSecure client;
  WiFiMulti wifiMulti;
  
  //ConsoleEcho SerialB;
  
  CCloud();
  void setup(){connectWifi();  setState("PLAY");setInterval(10);}
  void loop();

  std::string fillurl(std::string original,CMsg &msg);
  
  unsigned char h2int(char c);
  String urldecode(String str);
  String urlencode(String str);
  std::string convertStringToCommand(std::string str);
  void updateAllTransmittedCommands();
  void updateAllReceivedCommands();
  
  void updateTransmittedCmd(CMsg &msg);
  void updateAckCmd(CMsg &msg);    
  void insertSatData(CMsg &msg);
  void getEchoData(std::string  URL);  
  void getLastCommand();
  std::string getPage(std::string URL="https://example.org/");
  CMsg getJSONDoc(std::string strJSON, jsonDoc &doc);
  void connectWifi();  
  void callCustomFunctions(CMsg &msg); 

  
  CMsg receiveFile();
};
