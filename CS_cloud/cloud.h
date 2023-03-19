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

  String API_URL = "https://difpyeehbpvjttcvjudw.supabase.co/";
  String API_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImRpZnB5ZWVoYnB2anR0Y3ZqdWR3Iiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTY3ODQzMzY1NiwiZXhwIjoxOTk0MDA5NjU2fQ.mS7qHs79PuVDR7fqFgd3PkseiwKGuKfXNrCnNbxEL1o";


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

  void setup();
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

  void callSupabaseAPI(CMsg &msg);
  CMsg callSupabaseAPI(std::string key,std::string value, std::string name, std::string mid, std::string bsid);
  
  CMsg getJSONDoc(std::string strJSON);
  
  void callCustomFunctions(CMsg &msg); 

  CMsg receiveFile();
};
