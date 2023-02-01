#include "phone.h"


#define MAXSTREAMSIZE 200  //200
#define BUFFER_LENGTH 1000    //50

#define TIMEOUT 1*1000

#define PHONE_BAUD_RATE 115200

#define LATLON "LATLON"

////////// WARNING  WARNING WARNING     -------- NEED POWER AND GROUND TO CHIP TO WORK                 /////////////////


/*
Commands
//For Cubesat Dont need the C
//For Phone stuff, need the 'C' infront to designate 'Command'
//Compression parameters dont seem to be working   Need to check Android code
"CTIME"
"CGPS"
Front/Back, Quality, ISO, Shutter speed, White Balance
CPHOTO(B,50%,100,250,CLOUDY)
CPHOTO(F,50%,100,100,AUTO)
CPHOTO(B,50%,100,250,CLOUDY)
CPHOTO(F,50%,100,100,AUTO)
CBURST(B,50%,100,10,CLOUDY)
CBURST(F,50%,100,500,CLOUDY)
//CSTREAM(108.png,0)
//CSTREAM(108.png,117)
//CSTREAM(3858.jpg,0+)  this changed

stream(0_3.jpg,0)


for burst, you can submit the number of images in commend. If you don't it will use 5 as default

so in message it will be following

CBURST(B,50%,100,10,CLOUDY) > 5 pictures
CBURST(B,50%,100,10,CLOUDY,7) > 7 pictures
CBURST(B,50%,100,10,CLOUDY,7,5) > 7 pictures with 5 s delay


f: it presented it will take from front
q: quality of images
iso: iso of image
shutter: shutter of image
wp is the mode of image I will write down what is there
count and delay are optional, count number of images and delay

for wp there is : auto, cloudy, day, fluorescent, incandescent, shade, twilight, warm_fluorescent


(f,q,iso,shutter,wp,count,delay)
POWER
BATTERY
DELETE  //Delete all files


TO:BS~SYS:PHONE~ACT:SENDSERIAL~V:BATTERY
TO:BS~SYS:PHONE~ACT:SENDSERIAL~V:CSTREAM(11_330.jpg,0)
TO:BS314~SYS:PHONE~ACT:SENDSERIAL~V:PHOTO(B,10%,100,250,CLOUDY)



// DOn't send stuff directly to SendSerial.  Add it to queue instead
TO:BS314~SYS:PHONE~ACT:CTIME
TO:BS314~SYS:PHONE~ACT:CPHOTO(B,70%,100,250,CLOUDY)
TO:BS~SYS:PHONE~ACT:GETBLOCKS~N:11~START:5~STOP:16



*/


#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
#else
  
  #define PHONE_RX 13
  #define PHONE_TX 14

  /*

   TO:SENDSERIAL~PHONE:xxx

  #include <HardwareSerial.h>


//Definitely works TBeam 443
//May not need power, but definitely needs a GROUND connected to the Serial module

#define RXD2 13
#define TXD2 14

HardwareSerial ESerial2(2);


void setup() {
    Serial.begin(115200);
    ESerial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    while(!ESerial2)
      Serial.println("Serial2.................");
    
    Serial.println("starting.................");


}

void loop()
{
  
  if (Serial.available() > 0) {
    char a=Serial.read();
 //  Serial.print(a); Serial.print(".");
    ESerial2.write(a);
  }
  
  if (ESerial2.available() > 0) {
    
    Serial.write(ESerial2.read());
  }
}
  */
#endif


#define PHONE_DELAY_START 1
#define PHONE_DELAY 100000
#define ANDROID_POWERUP_DELAY 1000

////////// WARNING  WARNING WARNING     -------- NEED POWER AND GROUND TO CHIP TO WORK                 /////////////////


//TO:BS~SYS:PHONE~ACT:GETBLOCKS~N:11~START:5~STOP:16
void CPhone::getBlocks(CMsg &msg) {  //Send message to phone to get stuff 
  //stream(0_3.jpg,0)
	std::string key=msg.get(_NAME);
  int start=msg.get(_START,0);
  int stop=msg.get(_STOP,start+50);

  for(int count=start;count<stop;count++) {
    std::string str="CSTREAM("+key+"_";
    str+=tostring(count);
    str+=".jpg,0)";
    CMsg m;
    m.set(_ACT,str);
    m.writetoconsoleln();
    
    commandQueue.push(m);
  }
}

void  CPhone::clearQueue() {  //Send message to phone to get stuff 
  while(!commandQueue.empty()) commandQueue.pop();
}

void  CPhone::callCustomFunctions(CMsg &msg){  
  
  std::string act=msg.get(_ACT);

  mapcustommsg(getBlocks)
  mapcustom(clearQueue)
    
	
  if((act==_SENDSERIAL)) {std::string str=msg.get(_VALUE); sendSerial(str.c_str());  return;}
                    
  if(act.size()>1) { commandQueue.push(msg); return;}
  CSystemObject::callCustomFunctions(msg);  
}



CPhone::CPhone(){
  //Name ( _PHONE);   //Never name in constructor  Crashes!!@#!@#!@
  setForever(true);    
  setInterval(20);
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  #else
  _TX=PHONE_TX;
  _RX=PHONE_RX;
  #endif
  }


void CPhone::init(){

  _id = 0;
  transmitted = false;
  transmitting = false;
  received = false;

  _waitingForAck=0;
  _waitForAck=false;
  _lastSerial = getTime();
  _lastTx = getTime();
  _lastDebug = getTime();
  _lastPacketTOA = 0;
  _bnew=false;

  _lastTransmit = getTime();
  _nextTransmit = getTime();
  
}


void CPhone::setup() {   
  init();
 

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  Serial1.begin(PHONE_BAUD_RATE);
#else  
   Serial1.begin(PHONE_BAUD_RATE,SERIAL_8N1, PHONE_RX, PHONE_TX);
#endif  
  setState(_PLAY);
  sendSerial("INIT"); //to synchronise    
}


void CPhone::config(CMsg &msg){
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  #else
  int nTX=msg.get(_PHONETX,11);
  int nRX=msg.get(_PHONERX,12);  
  
  _TX=nTX;
  _RX=nRX;
  #endif
  setup();
}


void CPhone::ready(){  
  ReceivedPacket();  //Should hit onInit whch should set state to PLAY
}


bool CPhone::waitForBytes(int count) {   //Waits for data from serial port up to TIMEOUT secs   If it got the number of bytes, return true.   
  long start = getTime();
  while (Serial1.available() < count) {    //Get the number of bytes (characters) available for reading from the serial port. This is data that’s already arrived and stored in the serial receive buffer
    if (getTime() - start > TIMEOUT) {
      return false;
    }
  }
  return true;
}

bool CPhone::readUntil(char terminator, unsigned char* buffer) {    //Read from Phone
  for (int i = 0; i < BUFFER_LENGTH; i++) {
    bool ok = waitForBytes(1);
    if (!ok) return false;

    unsigned char b = Serial1.read();
    if (b == terminator) {
      buffer[i] = 0;
      return true;
    }
    else {
      if (i == BUFFER_LENGTH - 1) {
        return false;
      }
      else {
        buffer[i] = b;
      }
    }
  }
  return false;
}



//TO:ADR1~SYS:PHONE~ACT:SENDSERIAL~V:TIME

void CPhone::sendSerial(CMsg &msg){  
  
  writeconsoleln("send serial --> ");
  std::string s=msg.get(_VALUE);
  sendSerial(s.c_str());
}

void CPhone::sendSerial(const char* cmd) {    //Send to Phone
  if(state()!=_PLAY){   //Means u probably called it directly  Need to setup
    run(50);
    return;
  }
  writeconsoleln("void CPhone::sendSerial(const char* cmd)  ");
  _m.set(_MODE,_MODERX);
  _lastTransmit=getTime();
  _nextTransmit=_lastTransmit + 2000;
  Serial1.print(_id);
  Serial1.flush();
  Serial1.print(",");
  Serial1.flush();
  Serial1.println(cmd);
  Serial1.flush();
  delayMicroseconds(500); 
  _id++;


    CMsg m;
  m.set("NOW",getTime());
  m.set("LAST",_lastTransmit);
  m.set("NEXT",_nextTransmit);

  m.writetoconsoleln();

  writeconsoleln();  writeconsole("Sending to phone => ");  writeconsoleln(cmd);
}
    

 /*   

void push_stream(long id, byte* fileName, int block, int len, bool hasMore, byte* data) {
  message m;
  m.data[0] = id >> 8;                           //writing the parameters passed to the message structure
  m.data[1] = id;                                
  m.data[2] = 'S';
  m.data[3] = block >> 8;
  m.data[4] = block;
  m.data[5] = len;
  m.data[6] = hasMore;
  strcpy(m.data+7, (char*) fileName);
  int fnLen = strlen((char*) fileName);
  m.data[7 + fnLen] = 0;
  memcpy(m.data + fnLen + 8, data, len);
  
  m.length = len + fnLen + 8;
  
  if (queue.size() < 400) {
  //  queue.push(m);
  }
  else {
    writeconsoleln("Queue full!!!");
  }
}
*/



   
void CPhone::push_stream(long id, unsigned char* fileName, int block, int len, bool hasMore, unsigned char* data) {  //Gets Filename and offset sent to comms,  id is id of the phone
// char type = 'S';
std::string str;
//MAYBE USE CID AS ID
//X as first character denotes stream
//Capital B means no more
//std::string str = std::string("fileid:")+std::to_string(id)+std::string("~filename:")+std::string((char *)fileName)+std::string("~len:")+ std::to_string(int(len))+std::string("~more:")+ std::to_string(int(hasMore));
//if(hasMore)  str= std::string("cid:")+std::to_string(id)+std::string("~b:")+std::to_string(block);
//else str = std::string("X")+CID()+std::string("B")+std::to_string(block);
if(hasMore)  str= std::string("X")+getCID()+std::string("b")+tostring(block);                    
else str = std::string("X")+getCID()+std::string("B")+tostring(block);

while (str.length()<STREAMHEADERLEN) str=str+" ";
 writeconsoleln(); writeconsole("Stream Length:"); writeconsole(len+STREAMHEADERLEN); writeconsole("  "); writeconsoleln(str.c_str());

unsigned char buffer[2560];
int bufcount=0,count=0;
for (count=0;count<str.length();count++){
  buffer[bufcount]=str[count];
  bufcount++;
}
if(bufcount>STREAMHEADERLEN){
  writeconsole("ERROR   STREAM HEADER OVERFLOW ");    writeconsoleln(bufcount);
}
while(bufcount<STREAMHEADERLEN){
  buffer[bufcount]=' ';
  bufcount++;
}

for (count=0;count<len;count++){
  buffer[bufcount]=data[count];
  bufcount++;
}

//memcpy(buffer, str.c_str(), 10);


CMsg m(str);
std::string strfn((const char *)fileName);
m.set(_FILENAME,strfn);
m.set(_BLOCK,tostring(block));
m.initArray(buffer,bufcount);   ///// FIX THIS
std::string key=strfn;
key+="_";
key+=tostring(block);
m.set(_NAME,key);

addDataMap(key,m); 
addTransmitList(m);
writeconsoleln("Adding Picture Data to DataList");
m.writetoconsole();
}

void CPhone::onInitAvailable(int id) {   //Read from Phone add to queue
  std::string status="OK" ;
  writeconsoleln("On Init Available ");
  unsigned char buff[BUFFER_LENGTH];
  bool ok = readUntil('\n', buff);
  if (!ok)    return;

  CMsg m;
  m.set(_PHONE_INIT,status);
  addTransmitList(m);

  _m.set(_MODE,_BLANK);
 m.writetoconsole();
 setState(_PLAY);
 
}

void CPhone::onDirectoryAvailable(int id) {   //Read from Phone  add to queue
  writeconsoleln("On Directory Available");
  unsigned char batStr[BUFFER_LENGTH];
  bool ok = readUntil('\n', batStr);
  if (!ok) return;

  std::string str1=(char *)batStr;
  CMsg m;
  m.set(_PHONE_BATTERY,str1);   
  _m.set(_MODE,_BLANK);
   m.writetoconsole();
  addTransmitList(m); 
}




void CPhone::onBatteryAvailable(int id) {   //Read from Phone  add to queue
  writeconsoleln("On Battery Available");
  unsigned char batStr[BUFFER_LENGTH];
  bool ok = readUntil('\n', batStr);
  if (!ok) return;

  std::string str1=(char *)batStr;
  CMsg m;
  m.set(_PHONE_BATTERY,str1);   
  _m.set(_MODE,_BLANK);
   m.writetoconsole();
  addTransmitList(m); 
}


void CPhone::onTimeAvailable(int id) {   //Read from Phone  add to queue
  writeconsoleln("On Time Available");
  unsigned char timeStr[BUFFER_LENGTH];
  bool ok = readUntil('\n', timeStr);
  if (!ok) return;

  std::string str1=(char *)timeStr;
  CMsg m;
  m.set(_PHONE_TIME,str1);     
  _m.set(_MODE,_BLANK);
  m.writetoconsole();
  addTransmitList(m); 
}

void CPhone::onGpsAvailable(int id) {   //Read from Phone  add to queue
  unsigned char gpsStr[BUFFER_LENGTH];
  bool ok = readUntil('\n', gpsStr);
  if (!ok) return;

  std::string str;
  str=String((char *)gpsStr).c_str();
  int x=str.find(',');
  if (x+4<=str.size()){
    CMsg m;
    m.set(_LATLON,str.substr(0,x)+std::string(",")+str.substr(x+1,20));        
    _m.set(_MODE,_BLANK);
    m.writetoconsole();
    addTransmitList(m);
  }
}

void CPhone::onPhotoAvailable(int id) {    //Read from Phone  add to filename queue to go to comms
  unsigned char fileName[BUFFER_LENGTH];
  unsigned char fileSizeStr[BUFFER_LENGTH];
  long fileSize;
  bool ok = readUntil(',', fileName);
  if (!ok) return;
  ok = readUntil('\n', fileSizeStr);
  if (!ok) return;
  sscanf((char*)fileSizeStr, "%ld", &fileSize);

  //char msg[BUFFER_LENGTH];
  //sprintf(msg, "%s,%d", fileName, fileSize);
  std::string strfn((const char *)fileName);
  std::string strfs((const char *)fileSizeStr);
  CMsg m;
  m.set(_NAME,_PHONE_PHOTO);
  m.set(_FILENAME,strfn);
  m.set(_FILESIZE,strfs);  
  _m.set(_MODE,_BLANK);
  m.writetoconsole();
  addTransmitList(m);
}
 



void CPhone::onStreamAvailable(int id) {   //Read from Phone add to streamqueue
  std::string strfilename;
  unsigned char fileName[BUFFER_LENGTH];
  bool ok = readUntil(',', fileName);
  if (!ok) return;
  ok = waitForBytes(4);
  if (!ok) return;
  long block = (Serial1.read() << 8) + Serial1.read();
  char len = Serial1.read();
  unsigned char hasMore = Serial1.read();
  
  std::string strfn((const char *)fileName);
  strfn=strfn.substr(0,strfn.size()-4);
  strfn="img"+strfn;
 
 
  std::string datastr;
  int i=0, bufferlen=0;
  char c='a';
  int errcount=0;
  
  CMsg m;

  for(int count=0;count<BUFFER_LENGTH;count++){
    if(Serial1.available()>0){
      unsigned char a=Serial1.read();    
      i++;
      writeconsole(a);
      datastr+=a;
    }
    else{
      delay(5);
      errcount++;
      if(errcount>100) break;
    }
  }

  writeconsoleln(" ");  writeconsole("Bytes Read Data: ");  writeconsoleln(i);  writeconsoleln((long)datastr.size());

  
  strfilename=strfn;  
  strfilename+="_.jpg";
  m.set(_MSGTYPE,_STREAM);
  m.set(_API,_INSERTMULTI);
  m.set(strfilename,datastr);
  
  addTransmitList(m);

  bufferlen=i;
  

  for(int count=0;count<bufferlen;count+=MAXSTREAMSIZE){
    std::string str;
    m.clear();
    str=datastr.substr(count,MAXSTREAMSIZE);
      
    strfilename=strfn;  
    strfilename+=c;
    strfilename+=".jpg";
    m.set(_MSGTYPE,_STREAM);
    m.set(_API,_INSERTMULTI);
    m.set(strfilename,str);
    
    c++;
    addTransmitList(m);
  }

  /*
  for(int count=0;count<10;count++){
  while(Serial1.available()>0){
    unsigned char a=Serial1.read();    
    i++;
    writeconsole(a);
    datastr+=a;

    if(i>=MAXSTREAMSIZE) {
      
      std::string strfn((const char *)fileName);
      strfn=strfn.substr(0,strfn.size()-4);
      strfn="img"+strfn;
      strfn+=c;
      strfn+=".jpg";
      m.set(_MSGTYPE,_STREAM);
      m.set(_API,_INSERTMULTI);
      m.set(strfn,datastr);
      
      addTransmitList(m);
      writeconsoleln(" ");
      writeconsole("Bytes Read Data: ");
      writeconsoleln(i);
      writeconsoleln((long)datastr.size());

      std::string sample=m.get(strfn);
      
      writeconsole("Bytes Sample Read Data: ");
      writeconsoleln(i);
      writeconsoleln((long)sample.size());

      c++;
      datastr="";
      i=0;
      m.clear();
    }
  }
  delay(10);
  }

  if(datastr.size()>0){    
    std::string strfn((const char *)fileName);
    strfn=strfn.substr(0,strfn.size()-4);
    strfn="img"+strfn;
    strfn+=c;
    strfn+=".jpg";
    m.set(_MSGTYPE,_STREAM);
    m.set(_API,_INSERTMULTI);
    m.set(strfn,datastr);
    addTransmitList(m);
    writeconsoleln(" ");
    writeconsole("Bytes Read Data: ");
    writeconsoleln(i);
    writeconsoleln((long)datastr.size());
    
  }
*/
_m.set(_MODE,_BLANK);
return; 


//  push_stream(id, fileName, block, len, hasMore, data);         //This is the output



  
  /*
  if (hasMore == 3) {         //This is the queue to get more data from the phone
    char cmd[50];
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "STREAM(%s,%ld+)", fileName, block+1);  //Seems to be stream command

    CMsg m(cmd);
    streamQueue.push(m);  
  }

  if (streamQueue.size() > 0) {   //Sends next request to phone
    sendSerial(streamQueue.front().Data().c_str());
    streamQueue.pop();
  }
  */
}



void CPhone::ProcessCommandQueue(){     //This is where the New Command is procressed and sent to the phone
if(Mode()!=_BLANK) return;
if(_nextTransmit>getTime()) return;

if (commandQueue.size() > 0) {
  writeconsoleln("Sending Command to Phone ");  
  CMsg cmsg =commandQueue.front();
  commandQueue.pop();

  std::string bytes=cmsg.get(_ACT);    //Temporaty not using any params just seeing if CTIME works
  writeconsoleln(cmsg.Data().c_str());
  writeconsoleln(bytes.c_str());
  received = false;
  
  if (bytes[0] == 'C') { //command
    std::string tmpstr=bytes.substr(1,200);  //Sends it to the Phone   Needs to start with 'C'
    sendSerial((char*)tmpstr.c_str());   //Sends it to the Phone  
  }
  else if (bytes[0] == 'A') { //acknowledgment from receiver
    _waitingForAck = 0;
    /*
    char filename[16];
    for (int i = 0; i < 16; i++) {
      filename[i] = bytes[i+1];
      }
    bool sent = false;
    for (int i = 0; i < 64; i++) {
      for (int j = 0; j < 8; j++) {
        if ((bytes[i+1+16] & (1 << (7-j))) == 0) {
          char cmd[BUFFER_LENGTH];
          memset(cmd, 0, sizeof(cmd));
          sprintf(cmd, "STREAM(%s,%ld)", filename, i*8 + j);
          if (!sent) {
            sendSerial(cmd);        //Sends it to the Phone
            sent = true;
          }
          else {
            std::string s=cmd;
            CMsg m(s);
            streamQueue.push(m);
          }
        }
      }
    }
    */
  } 
}

setState(_PLAY);

}


 
void CPhone::ReceivedPacket() {  //Any Comments to Serial Port go right back to the PHONE!!!!!  Screws it up
  if(Mode()!=_MODERX) return;

  if(getTime()>  _lastTransmit+PHONEDATADELAY){
    _m.set(_MODE,_BLANK);
    return;
  }

 int counter=0;
  if (Serial1.available() > 0) {   //Reads data from phone.  Takes ID and MessageType and uses that to call the appropriate function to read from phone and if result is good adds it to the "queue"
    counter++;
    if (counter>30000) {
      return;
    }

    writeconsoleln("Reading Data from Phone");
    unsigned char msgIdStr[BUFFER_LENGTH];
    int msgId;
    char msgType;
    bool ok = readUntil(',', msgIdStr);
    if (!ok) return;
    sscanf((char*)msgIdStr, "%d", &msgId);
    ok = waitForBytes(1);
    if (!ok) return;
    msgType = Serial1.read();

    //writeconsole(" --msgType"); writeconsoleln(msgType);
  
    switch (msgType) {
      case 'B':      
        onBatteryAvailable(msgId);
        break;
      case 'F':      
        onDirectoryAvailable(msgId);
        break;
      case 'I':        
        onInitAvailable(msgId);
        break;
      case 'T':      
        onTimeAvailable(msgId);  
        break;
      case 'G':      
        onGpsAvailable(msgId);
        break;
      case 'P':        
        onPhotoAvailable(msgId);
        break;
      case 'S':        
        onStreamAvailable(msgId);
        break;
    }
  }

}


void CPhone::SetPhoneReceive(){}

void CPhone::TransmitPacket(CMsg &m){
  commandQueue.push(m);
}   

void CPhone::loop() {    
  ReceivedPacket();
    
  ProcessCommandQueue();      
}
