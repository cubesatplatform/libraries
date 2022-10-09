#include "cloud.h"

CCloud::CCloud(){
  Name("CLOUD");
  MSG= getMessages();

  URLS["REGISTER"] = host + std::string("/register");
  URLS["INSERTLOCATION"] = host + std::string("/insertlocation?bsid=$bsid");
  URLS["GETCMD"] = host + std::string("/getcmd?bsid=$bsid");
  URLS["UPDATECMD"] = host + std::string("/updatecmd?cid=$cid&bsid=$bsid");
  URLS["INSERTCMD"] = host + std::string("/insertcmd?sent=1&cid=$cid&bsid=$bsid&sendon=$sendon&data=$data");
  URLS["UPDATEACK"] = host + std::string("/updateack?ack=1&cid=$cid&bsid=$bsid");
  URLS["INSERT"] =  host + std::string("insert?bid=1");
  URLS["QUERY"] =  host + std::string("query?bid=1");
  URLS["LOG"] =host + std::string("/v1/errorlog?bid=1&data=");

  
}
 
void CCloud::connectWifi() {
  
  wifiMulti.addAP("Truffle1", "Alexander1");
//  wifiMulti.addAP("Cabana", "alexander");
  long count=0;
  while ((wifiMulti.run() != WL_CONNECTED)&&(count<10)) {
    delay(delayWaitingForWifiConnection);
    writeconsole(".");
    count++;
  }
  writeconsoleln("");
  writeconsole("WiFi connected to ");
  writeconsole(WiFi.SSID());
  writeconsole(" with IP ");
  writeconsoleln(String(WiFi.localIP()));
}
  

CMsg CCloud::receiveFile() {
  writeconsoleln("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv CMsg receiveFile()vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
  CMsg m;
  int maxloops = 0;
  unsigned char buffer[500]={NULL};

  //wait for the server's reply to become available
  while (!client.available() && maxloops < 1500)
  {
    maxloops++;
    delay(1); //delay 1 msec
  }  
  // if there are incoming bytes available
  // from the server, read them and print them:
  char c;
  if(client.available()){
    while (client.available()) {  //Clears out headers
      String line = client.readStringUntil('\r');
      //writeconsole(line);      writeconsole("   =>");      writeconsole(line.length());
      if (line.length()<=1){
        c = client.read();      
        break;  
      }
    }

    int count=0;
    
    while (client.available()&&(count<250)) {
      c = client.read();      
      //Serial.write(c);
      buffer[count]=c;
      count++;
    }

    if(count>20){  
      writeconsoleln("  InitArray");
      m.initArray(buffer, count);
    }

    client.stop();
  }

  delay(100);
  writeconsoleln();writeconsoleln("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ CMsg receiveFile()^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
  return m;
 
}


std::string CCloud::getPage(std::string URL){
  String payload;
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      writeconsole("Get URL =========>  ");
      writeconsoleln(URL.c_str());
      
      // Your Domain name with URL path or IP address with path
      http.begin(URL.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {        
        writeconsole("HTTP Response code: ");
        writeconsoleln(httpResponseCode);
        payload = http.getString();
        writeconsoleln(payload);        
      }
      else {
        writeconsole("Error code: ");
        writeconsoleln(httpResponseCode);
        http.end();
        return(std::string(payload.c_str()));
      }
      // Free resources
      http.end();
    }
    else {
      writeconsoleln("WiFi Disconnected");
      setState("");
    }
    payload=payload.substring(1,payload.length()-2);  //Gets rid of []   This JSon doesnt like it
    return(std::string(payload.c_str()));
   }





CMsg CCloud::getJSONDoc(std::string strJSON, jsonDoc &doc){
  CMsg msg;
  if (doc==NULL) return msg;

 DeserializationError error = deserializeJson(doc, strJSON);
 
 if (error)  {
   writeconsole(F("deserializeJson() failed: "));
   //writeconsoleln(error.f_str());
   return msg;
  }  
  else {  
    
    JsonObject root = doc.as<JsonObject>();
    
    // using C++11 syntax (preferred):
    for (JsonPair kv : root) {          
      msg.setParameter(std::string(kv.key().c_str()), std::string(kv.value().as<char*>()));
    }

    addTransmitList(msg);  
    return msg;
    }      
  return msg;
  }




  
void CCloud::updateAllReceivedCommands(){
  if (!MSG->ReceivedList.size()) return;
  int count=0;
  writeconsoleln("updateAllReceivedCommands()  {");
  while(MSG->ReceivedList.size()){
      CMsg msg=MSG->ReceivedList.front();
      if (msg.isAck()) {
        msg.setParameter("API","UPDATEACK");
        callAPI(msg);
        }
        else{ 
          if(msg.Data().length()>1) {
            msg.setParameter("API","INSERT");
            callAPI(msg);
          } 
          else {  //Its a stream  Don't know how to add yet         
          }
      }      
    //  radio.ReceivedList.pop_front();   //???????????????????????????????????
      count++;
      if(count>10) break;
    }
   writeconsoleln("}END    updateAllReceivedCommands()");  
  }


void CCloud::updateAllTransmittedCommands(){
  int count=0;
  
  while(MSG->TransmittedList.size()){
    
      CMsg msg=MSG->TransmittedList.front();
      msg.writetoconsole();
  
      std::string cid=msg.getParameter("CID","");
      if (cid.length()<1) {      
        writeconsoleln("updateAllTransmittedCommands()  NO CID ---------------------------------------------------------");
        return;
      } 
      else{
        writeconsoleln("updateAllTransmittedCommands()  YES CID --------------------------------------------------------");
        msg.setParameter("API","UPDATECMD");
        callAPI(msg);
      }

      MSG->TransmittedList.pop_front();
      count++;
      if(count>10) break;
    }  
  }


void CCloud::loop(){

  if(millis() - _lastCmd >= CMD_INTERVAL )  {
    _lastCmd=millis();

    CMsg m;
    m.setParameter("API","GETCMD");
    callAPI(m);   //Restarts when connecting to service has some problem        
    updateAllTransmittedCommands(); 
    updateAllReceivedCommands();    
  }  
}


void CCloud::callCustomFunctions(CMsg &msg)  {  //Used to be NewMsg
  CSystemObject::callCustomFunctions(msg);  
  std::string sys=msg.getSYS();
  std::string act=msg.getACT();

  msg.writetoconsole();

  if(act=="CALLAPI") callAPI(msg);
  if(act=="UPDATEALLTRANSMITTEDCOMMANDS")   updateAllTransmittedCommands();
  if(act=="UPDATEALLRECEIVEDCOMMANDS") updateAllReceivedCommands();    
  if(act=="CONNECTWIFI") connectWifi();    
  if(act=="GETPAGE") getPage(msg.getParameter("URL"));
  
}



std::string CCloud::fillurl(std::string original,CMsg &msg){
  std::string cid=msg.get("CID");
  std::string bsid=msg.get("BSID");
  std::string datastr=msg.get("DATA");
  std::string sendon=msg.get("SENDON");
  std::string newurl=original;

  newurl = strReplace(newurl,"$cid",cid);
  newurl = strReplace(newurl,"$bsid",bsid);
  newurl = strReplace(newurl,"$data",datastr);
  newurl = strReplace(newurl,"$sendon",sendon);

  newurl = strReplace(newurl,"$bsid",std::string("all"));

  writeconsole("Fill URL   :==>");
  writeconsole(original); writeconsole("   :    ");
  writeconsoleln(newurl);
  return newurl;
 
}



CMsg CCloud::callAPI(CMsg &msg) {
  std::string api=msg.getParameter("API","");
  std::string url=msg.getParameter("URL",URLS[api]);
  std::string newurl=fillurl(URLS[api],msg);
  std::string line =getPage(newurl);   

  if(line.length()<3) {
    CMsg m;
    return m;
  }
   
  //line=line.substring(1,line.length()-2);  //Gets rid of []   This JSon doesnt like it
  writeconsoleln(line.c_str());

  jsonDoc doc;
  
  CMsg m=getJSONDoc(line, doc);

  writeconsoleln(line);
  m.writetoconsole();

  if(m.Parameters.size())  {
    std::string act=m.getACT();
    //addTransmitList(m);    
    addMessageList(m);    
    }
  return m;
  }

