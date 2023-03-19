#include <list>
#include "cloud.h"
#include "myUDHttp.h"



/*
TO:BS8~SYS:PHONE~ACT:SENDSERIAL~V:BURST(F,50%,100,100,AUTO)
TO:BS8~SYS:PHONE~ACT:SENDSERIAL~V:PHOTO(F,50%,100,100,AUTO)
TO:BS8~SYS:PHONE~ACT:SENDSERIAL~V:stream(2_7.jpg,0)
*/


//these callbacks will be invoked to read and write data to sdcard
//and process response
//and showing progress 

int xx=5;
std::list<unsigned char> listBuffer; 

int responsef(uint8_t *buffer, int len){
  Serial.printf("%s\n", buffer);
  return 0;
}

int rdataf(uint8_t *buffer, int len){
  int count=0;
  while(len>0){
    if(listBuffer.size()>0){
      *buffer=listBuffer.front();
      listBuffer.pop_front();
      buffer++;      
      count++;
    }
    len--;
  }
  
  return count;
}


int wdataf(uint8_t *buffer, int len){
  //write downloaded data to file
  return len;
}


void progressf(int percent){
  //Serial.printf("..Progress %d\n", percent);
}

int fillListBuffer(std::string str){
  //writeconsoleln("fillListBuffer:    ");
  listBuffer.clear();
  int i=0;
  for(i=0;i<str.size();i++){
    listBuffer.push_back(str[i]);
    //writeconsole(str[i]);
  }
  //writeconsoleln(" ");
  return i;
}

CCloud::CCloud(){  
  URLS[_REGISTER] = host + std::string("/register");
  URLS[_GETCMD] = host + std::string("/getcmd");
  URLS[_GETIMAGE] = host + std::string("/getimage");
  URLS[_INSERTCMD] = host + std::string("/insertcmd");
  URLS[_UPDATEACK] = host + std::string("/updateack");
  URLS[_INSERT] =  host + std::string("/insert");
  URLS[_INSERTMULTI] =  host + std::string("/insertmulti");
//  URLS[INSERTIMAGE] =  host + std::string("/insertimage");
  URLS[_QUERY] =  host + std::string("/query");
  URLS[_LOG] =host + std::string("/v1/errorlog");
  
}

void CCloud::setup(){
    connectWifi();  
    client.setInsecure();
    setState(_PLAY);
    setInterval(100);//pMessages=getMessages();
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
  writeconsoleln("");  writeconsole("WiFi connected to ");  writeconsole(WiFi.SSID());  writeconsole(" with IP ");  writeconsoleln(String(WiFi.localIP()));
}
  

CMsg CCloud::receiveFile() {
  //writeconsoleln("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv CMsg receiveFile()vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
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
  writeconsoleln();writeconsoleln("CMsg receiveFile");
  return m;
 
}


std::string CCloud::getPage(std::string url){
  String payload;
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      writeconsole("Get URL>  ");
      writeconsoleln(url.c_str());

      /*
      
      // Your Domain name with URL path or IP address with path
      http.begin(url.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      */

      ////////////////////////////////////////
       // Your Domain name with url path or IP address with path



     // std::string url="/insertcmd?CID=$cid&BSID=$bsid&SENDON=$sendon&DATA=$data";
      std::string path, params;
      std::size_t pos=url.find("?");

      if(pos>0){
        path=url.substr(0,pos);
        params=url.substr(pos+1, url.size());
        writeconsole("path   ");        writeconsoleln(path);
        writeconsole("params   ");        writeconsoleln(params);
      }
      http.begin(path.c_str());
      
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
      
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      //http.addHeader("Content-Type", "multipart/form-data;boundary=""boundary""");
      // Data to send with HTTP POST
      //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";           
      String httpRequestData = params.c_str();
      // Send HTTP POST request
      //int httpResponseCode = http.POST(httpRequestData);
      int httpResponseCode = http.POST("DATA=squirrels&RSSI=cats");
      httpResponseCode=http.POST("RSSI=cats");

      /////////////////////////////////////
      
      if (httpResponseCode>0) {        
        writeconsole("  HTTP Response code: ");
        writeconsoleln(httpResponseCode);
        payload = http.getString();
        writeconsoleln(payload);        
      }
      else {
        writeconsole("Error code: ");
        writeconsoleln(httpResponseCode);
        
        return(std::string(payload.c_str()));
      }
      // Free resources
      http.end();
    }
    else {
      writeconsoleln("WiFi Disconnected");
      setState(_BLANK);
    }
    //payload=payload.substring(1,payload.length()-2);  //Gets rid of []   This JSon doesnt like it
    return(std::string(payload.c_str()));
   }


//https://microcontrollerslab.com/esp32-http-post-using-arduino-ide-thingspeak-and-ifttt/
std::string CCloud::getPage(CMsg &msg){
  String payload;
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;

    std::string api=msg.get(_API);
    std::string path=msg.get(_URL,URLS[api]);
    writeconsole("Get URL >  ");
    writeconsoleln(path.c_str());

    http.begin(path.c_str());
    
    // If you need Node-RED/server authentication, insert user and password below
    //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    //http.addHeader("Content-Type", "multipart/form-data;boundary=""boundary""");
    // Data to send with HTTP POST
    
    int httpResponseCode; 
    std::string str;

    for (auto x:msg.Parameters){
      if(x.second.size()){
        str+=x.first+"="+x.second+"&";          
        //writeconsole(x.first);writeconsole(" = ");writeconsoleln((long) x.second.size());    
      }
    }
    writeconsoleln("vv");
    httpResponseCode=http.POST(str.c_str());
    writeconsoleln(str);
    writeconsoleln("^^");

    

    if (httpResponseCode>0) {        
      writeconsole("    HTTP Response code: ");
      writeconsoleln(httpResponseCode);
      payload = http.getString();
      writeconsoleln(payload);        
    }
    else {
      writeconsole("Error code: ");
      writeconsoleln(httpResponseCode);
      
      //return(std::string(payload.c_str()));
    }  
          
    // Free resources
    http.end();
  }
  else {
    writeconsoleln("WiFi Disconnected");
    setState(_BLANK);
  }
  payload=payload.substring(1,payload.length()-2);  //Gets rid of []   This JSon doesnt like it
  return(std::string(payload.c_str()));
}


CMsg CCloud::callSupabaseAPI(std::string key,std::string value, std::string name, std::string mid, std::string bsid){
  CMsg mpayload;
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(API_URL+"/rest/v1/satdata");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Prefer", "return=representation");
    http.addHeader("apikey", API_KEY);
    http.addHeader("Authorization", "Bearer " + API_KEY);

    std::string str="{";
    str+="\"";
    str+="Key";
    str+="\":";
    str+="\"";
    str+=key;          
    str+="\"";
    str+=",";
    str+="\"";
    str+="Value";
    str+="\":";
    str+="\"";
    str+=value;          
    str+="\"";

    str+=",";
    str+="\"";
    str+="Name";
    str+="\":";
    str+="\"";
    str+=name;          
    str+="\"";

    str+=",";
    str+="\"";
    str+="Bsid";
    str+="\":";
    str+="\"";
    str+=bsid;          
    str+="\"";

    str+="}";
    int httpCode = http.POST(str.c_str());
    String payload = http.getString(); 
    writeconsole("Code: "); writeconsoleln(httpCode);   //Print HTTP return code
    writeconsole("Payload: "); writeconsoleln(payload.c_str());    //Print request response payload

    mpayload=getJSONDoc(std::string(payload.c_str()));              

    mpayload.writetoconsole();
    http.end();
  }else{
    writeconsoleln("Error in WiFi connection");
  }  
  return mpayload;
}



//https://postgrest.org/en/stable/api.html#insertions
void CCloud::callSupabaseAPI(CMsg &msg){
  std::string name=msg.get(_NAME);
  std::string bsid=msg.get(_BSID);
  std::string mid=msg.get(_MID);

  for (auto x:msg.Parameters){
    if(x.second.size()){
      //writeconsole(x.first);writeconsole(" = ");writeconsoleln((long) x.second.size());    
      CMsg smsg=callSupabaseAPI(x.first,x.second, name, mid, bsid);
    }
  }
  
  return;
}



std::string CCloud::getPageMulti(CMsg &msg){
  
  std::string payload;
  if(WiFi.status()== WL_CONNECTED){         
    std::string api=msg.get(_API);
    std::string path=msg.get(_URL,URLS[api]);
    writeconsole("Get URL >  ");
    writeconsoleln(path.c_str());
    
    msg.writetoconsole();

    for (auto x:msg.Parameters){
      std::string str1,str2;
      str1=x.first;
      str2=x.second;
      //writeconsole(str1);writeconsole(" = ");writeconsoleln(str2);    
      if((str1.size()>2)&&(str2.size()>0)){
        UDHttp udh;        
        if(((str1[0]=='I')&&(str1[1]=='R'))||  ((str1[0]=='i')&&(str1[1]=='m')&&(str1[2]=='g'))){              
          writeconsoleln("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv Saving Multi Form vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv");
          fillListBuffer(str2);        
          udh.upload((char *)path.c_str(), (char *)str1.c_str(), listBuffer.size(), rdataf, progressf, responsef);
          writeconsoleln("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ End Saving Multi Form ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
        }
         
      }
    }
    
  }
  else {
    writeconsoleln("WiFi Disconnected");
    setState(_BLANK);
  }
  
  
  
  return(payload);
}



CMsg CCloud::getJSONDoc(std::string strJSON){
  CMsg msg;
  jsonDoc doc;

  String str=strJSON.c_str();
  str.trim();
  if(str.length()==0) return msg;

  if(str[0]=='[') str=str.substring(1,str.length()-1);  //Gets rid of [   This JSon doesnt like it

  if(str[str.length()-1]==']') str=str.substring(0,str.length()-1);  //Gets rid of [   This JSon doesnt like it

  strJSON=str.c_str();

 DeserializationError error = deserializeJson(doc, strJSON);
 
 if (error)  {
   writeconsoleln(F("deserializeJson() failed: "));   
  }  
  else {      
    JsonObject root = doc.as<JsonObject>();
    
    for (JsonPair kv : root) {         
       msg.set(std::string(kv.key().c_str()), std::string(kv.value().as<char*>()));      
      }
    }      
  return msg;
  }



void CCloud::callCustomFunctions(CMsg &msg)  {  //Used to be NewMsg
  
  std::string sys=msg.get(_SYS);
  std::string act=msg.get(_ACT);

  //msg.writetoconsole();
  if(act==_GETPAGE) {getPage(msg.get(_URL)); return;}
  mapcustommsg(callAPI)
  mapcustom(connectWifi)
  mapcustommsg(save)

  
  CSystemObject::callCustomFunctions(msg);  
}



std::string CCloud::fillurl(std::string original,CMsg &msg){
  std::string cid=msg.get(_MCID);
  std::string bsid=msg.get(_BSID,getIAM());
  std::string datastr=msg.get(_DATA);
  std::string sendon=msg.get(_SENDON);
  std::string newurl=original;

  datastr = urlencode(datastr);

  newurl = strReplace(newurl,"$cid",cid);
  newurl = strReplace(newurl,"$bsid",bsid);
  newurl = strReplace(newurl,"$data",datastr);
  newurl = strReplace(newurl,"$sendon",sendon);

  newurl = strReplace(newurl,"$bsid",std::string("all"));
  
  return newurl; 
}




CMsg CCloud::callAPI(CMsg &msg) {
  std::string api=msg.get(_API);
  std::string line;
  if(api==_INSERTMULTI){
    writeconsoleln("Insert Multi....  getPageMulti");
    line =getPageMulti(msg);  
  }
  else
    line =getPage(msg);   

  if(line.length()<3) {
    CMsg m;
    return m;
  }
   
  //writeconsoleln(line.c_str());

  jsonDoc doc;
  
  CMsg m=getJSONDoc(line);
  
  return m;
  }



void CCloud::registerBS(){
  
  CMsg m=callSupabaseAPI("","", "BS_REGISTER", "", "");
  std::string bsid=m.get("Id");
  if(bsid.size()>3) {
    bsid=bsid.substr(0,3);
  }
  setIAM(bsid);
  _bRegistered=true;

  writeconsole("RegisterBS ");writeconsoleln(bsid);

  }

void CCloud::getCommand(){
  CMsg m;
  m.set(_API,_GETCMD);
  m.writetoconsole();
  CMsg msg=callAPI(m);   //Restarts when connecting to service has some problem      

  std::string data=msg.get(_DATA);
  std::string to=msg.get(_TO,_ADR1);

  if(data.size())  {
    String s=data.c_str();
    s.replace(";",":");
    s.replace("^","~");
    std::string data2=s.c_str();
    
    CMsg mm(data2);
    mm.set(_FROM,getIAM());
    mm.set(_TO,to);
    mm.writetoconsole();
    addTransmitList(mm);      
    }
}

void CCloud::save(CMsg &msg)  {
  
  msg.set(_PWD,_BLANK);    //Postgres cant handle bytes in text fields  so this blanks out the pwd check
  msg.set(_BASE,getIAM());
  msg.set(_MID,msg.getStringID());
  //std::string data=msg.serialize();
  //data = strReplace(data,":",";");
  //data = strReplace(data,"~","^");

  //m.set(DATA,data);
  if (msg.get(_API).size()==0) msg.set(_API,_INSERT);
  

  writeconsoleln("Save: ");
  
  CMsg mm=callAPI(msg);   //Restarts when connecting to service has some problem    
}

void CCloud::loop(){
  if(millis() - _lastCmd >= 2*CMD_INTERVAL )  {
    _lastCmd=millis();

    if(!_bRegistered){
      registerBS();
    }
    else{
      //getCommand();                    
      CMsg msg;
      msg.set("First","MAMAMAMAlexander");
      msg.set("Last","MAMAMARusich");
      callSupabaseAPI(msg);
    }
  }
}

