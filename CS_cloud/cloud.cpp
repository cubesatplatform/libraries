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

//satadata    this is the table name that has everything   all the rest are just views of this table
//vsatadata   this is the view of satadata that is ordered by time Ts desc
//vgps       this is the view of satadata that is ordered by time Ts desc and has only the gps data


//https://difpyeehbpvjttcvjudw.supabase.co/rest/v1/satdata?select=*,Key,Value,Ts&limit=5&order=Ts.asc&Key=eq.CLUSTER&apikey=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImRpZnB5ZWVoYnB2anR0Y3ZqdWR3Iiwicm9sZSI6InNlcnZpY2Vfcm9sZSIsImlhdCI6MTY3ODQzMzY1NiwiZXhwIjoxOTk0MDA5NjU2fQ.mS7qHs79PuVDR7fqFgd3PkseiwKGuKfXNrCnNbxEL1o
// gt.   greater than
// lt.  less than
// eq.  equal to
// neq. not equal to
// gte. greater than or equal to
// lte. less than or equal to
// like.  like
// ilike.  case insensitive like
// is.  is null
// in.  in
// cs.  contains
// cd.  contains (case insensitive)
// sl.  starts with
// sd.  starts with (case insensitive)
// nl.  ends with
// nd.  ends with (case insensitive)
// ov.  overlaps
// ft.  full text search
//

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
  
}

void CCloud::setup(){
    connectWifi();  
    client.setInsecure();
 
    setInterval(100);//pMessages=getMessages();
  }

 
void CCloud::connectWifi() {
  
  wifiMulti.addAP("Truffle1", "Alexander1");
  wifiMulti.addAP("jpad_connect", "Starbucks1660");
  wifiMulti.addAP("jpad_connect", "starbucks1660");
  wifiMulti.addAP("Cabana", "alexander");
  wifiMulti.addAP("Cabana", "alexander1");
  long count=0;
  while ((wifiMulti.run() != WL_CONNECTED)&&(count<10)) {
    delay(delayWaitingForWifiConnection);
    writeconsole(".");
    count++;
  }
  if(wifiMulti.run() == WL_CONNECTED){
    setState(_PLAY);
    writeconsoleln("");  writeconsole("WiFi connected to ");  writeconsole(WiFi.SSID());  writeconsole(" with IP ");  writeconsoleln(String(WiFi.localIP()));
    }
  else{
    setState(_PAUSE);
    writeconsoleln("No WiFi connection");
  }  
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
    std::string path=msg.get(_URL);
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




std::string CCloud::getPageMulti(CMsg &msg){
  
  std::string payload;
  if(WiFi.status()== WL_CONNECTED){         
    std::string api=msg.get(_API);
    std::string path=msg.get(_URL);
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
  
  mapcustom(connectWifi)
  mapcustommsg(save)
  
  CSystemObject::callCustomFunctions(msg);  
}




void CCloud::registerBS(){
  writeconsoleln("RegisterBS");
  CMsg m;
  m.set("Key","RegisterBS");
  std::string data=m.payload();
  std::string payload=supabase.INSERT("satdata",data);
  m.clear();
  m=getJSONDoc(payload);

  std::string bsid=m.get("Id");
  if(bsid.size()>3) {
    bsid=bsid.substr(0,3);
  }
  setIAM(bsid);
  _bRegistered=true;

  writeconsole("RegisterBS ");writeconsoleln(bsid);

  }

void CCloud::getCommand(){
  CMsg m,mpayload;
  
  std::string payload=supabase.SELECT("vnextcmd",m,m,m,1);

  mpayload=getJSONDoc(payload);              
  mpayload.writetoconsole();

  std::string data=mpayload.get("Value");


  if(data.size()>5)  {
   // String s=data.c_str();
   // s.replace(";",":");
   // s.replace("^","~");
   // std::string data2=s.c_str();
    
    CMsg mm(data);
    mm.set(_FROM,getIAM());    
    mm.writetoconsole();
    addTransmitList(mm);  


    CMsg mupdate;
    mupdate.set("Id",mpayload.get("Id"));
    mupdate.set("Key",std::string("SENT")+getIAM());
    mupdate.set("T",getTime());


    std::string dataupdate=mupdate.payload();
    std::string payload=supabase.UPDATE("satdata",dataupdate);
    }
}

void CCloud::save(CMsg &msg)  {
  if(state()!=_PLAY)
    return;
  writeconsoleln("Save To Cloud: ");
  msg.set(_PWD,_BLANK);    //Postgres cant handle bytes in text fields  so this blanks out the pwd check
  msg.set(_MID,msg.getStringID());
  msg.set("Bsid",getIAM());
  
    
  std::string payload=supabase.INSERT("satdata",msg);
  
}




void CCloud::save(){
  if(  MMM.CloudList.MList.size()>0)  {
    CMsg m = MMM.CloudList.MList.front();
    MMM.CloudList.MList.pop_front();
    save(m);
  }

}


void CCloud::loop(){
  if(millis() - _lastCmd >= CMD_INTERVAL )  {
    _lastCmd=millis();

    if(!_bRegistered){
      registerBS();
    }
    else{
      getCommand();              
    }
  }
  save();
  
}

