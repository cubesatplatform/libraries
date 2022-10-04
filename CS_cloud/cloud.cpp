#include "cloud.h"

CCloud::CCloud(){
  Name("CLOUD");
  MSG= getMessages();
  
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


void CCloud::getLastCommand() {
   std::string line =getPage(getcmdurl);   

   if(line.length()<3) return;
   
  //line=line.substring(1,line.length()-2);  //Gets rid of []   This JSon doesnt like it
  writeconsoleln(line.c_str());

  jsonDoc doc;
  
  CMsg msg=getJSONDoc(line, doc);

  writeconsoleln(line);
  msg.writetoconsole();

  if(msg.Parameters.size())  addTransmitList(msg);    
  }

void CCloud::insertSatData(CMsg &msg){
  std::string newurl=fillurl(inserturl,msg);
  getPage(newurl);
}


void CCloud::updateTransmittedCmd(CMsg &msg){
  std::string newurl=fillurl(updatecmdurl,msg);
  writeconsole(".........................................updateTransmittedCmd------");
  writeconsoleln(newurl);
  getPage(newurl);
}


void CCloud::updateAckCmd(CMsg &msg){
  std::string newurl=fillurl(updateackurl,msg);
  getPage(newurl);
}



void CCloud::getEchoData(std::string  URL){
   writeconsoleln(getPage(std::string(URL)).c_str());
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
        updateAckCmd(msg);
        }
        else{ 
          if(msg.Data().length()>1)  insertSatData(msg);
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
        updateTransmittedCmd(msg);
      }

      MSG->TransmittedList.pop_front();
      count++;
      if(count>10) break;
    }  
  }


void CCloud::loop(){

  if(millis() - _lastCmd >= CMD_INTERVAL )  {
    _lastCmd=millis();


    getLastCommand();   //Restarts when connecting to service has some problem        
    updateAllTransmittedCommands(); 
    updateAllReceivedCommands();    
  }

  
  //processMSG();  
}


void CCloud::callCustomFunctions(CMsg &msg)  {  //Used to be NewMsg
  
  std::string sys=msg.getSYS();
  std::string act=msg.getACT();

  msg.writetoconsole();

  if(act=="UPDATEALLTRANSMITTEDCOMMANDS")   updateAllTransmittedCommands();
  if(act=="UPDATEALLRECEIVEDCOMMANDS") updateAllReceivedCommands();    
  if(act=="CONNECTWIFI") connectWifi();  
  if(act=="GETLASTCOMMAND") getLastCommand();
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
