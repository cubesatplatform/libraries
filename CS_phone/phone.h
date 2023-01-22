#pragma once

#include <messages.h>
#include <systemobject.h>
#include <list>
#include <queue>
#include <funcs.h>





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
CSTREAM(108.png,0)
CSTREAM(108.png,117)
CSTREAM(3858.jpg,0+)

stream(12_54.jpg,0)                 //54 is the chunk number   0 is the part of 54 to get

TO:BS103~SYS:PHONE~ACT:SENDSERIAL~V:stream(12_54.jpg,0)

Command for streaming is stream(123.jpg,0+), if you dont have photos from last time, you can find commamds for taking photo in transmitter.ino, at the end of the file. 
As it's streaming you can see which chunks are received. At the end of streaming, there will be acknowledgement message, which on receiver will look like "<messageid> A <list of chunks not received>". 
That list will most likely be empty, you can maybe force lost packets by removing antenna from receiver 
(I'm not sure for that if comms are close to each other, i connected radios with jumpers and disconnected i think dio1 to test missing packets).

for individual chunk its stream(123.jpg,456) (here for chunk number 456). there is no single command for streaming all miising chuncks
*/


//How it works  Send one CMessage at a time  It fills it tells you its done   no more queues  data goes into cmessage  Pass as reference


#define STREAMHEADERLEN 15
#define PHONEDATADELAY 30000
#define _PHONETX "PHONETX"
#define _PHONERX "PHONERX"


#define _MODETX "MODETX"
#define _MODERX "MODERX"   

#define _LATLON "LATLON"



//  Serial1.begin(PHONE_BAUD_RATE, SERIAL_8N1, PHONE_TX, PHONE_RX);


class CPhone:public CSystemObject {
  private:
    
    long int _id = 0;
    volatile bool transmitted = false;
    volatile bool transmitting = false;
    volatile bool received = false;
   
    long _waitingForAck;
    bool _waitForAck;
    long _lastSerial = getTime(), _lastTx = getTime(), _lastDebug = getTime();
    long int _lastPacketTOA = 0;
    bool _bnew=false;
    PinName _TX;
    PinName _RX;
    long _lastTransmit=0;
    long _nextTransmit=0;
    
  public:
    std::queue<CMsg> commandQueue;   //This is the commands that should be sent to phone,filled from the Transmitter  
    std::queue<CMsg> streamQueue;   //This is output file segment data from the Phone to be sent to  Transmitter

    
    CPhone();
 
    void setup();
    void config(CMsg &msg);
    bool waitForBytes(int count);
    bool readUntil(char terminator, unsigned char* buffer);
    void sendSerial(const char* cmd);
    void sendSerial(CMsg &msg);
    void push_stream(long id, unsigned char* fileName, int block, int len, bool hasMore, unsigned char* data);
    void onInitAvailable(int id);
    void onTimeAvailable(int id);
    void onGpsAvailable(int id);
    void onPhotoAvailable(int id);
    void onBatteryAvailable(int id);
    void onStreamAvailable(int id);
    void onDirectoryAvailable(int id);
    void ProcessCommandQueue();
    void TransmitPacket(CMsg &m);
    void SetPhoneReceive();
    void ReceivedPacket();
    void loop();
    void init();
    void ready();    
	  void getBlocks(CMsg &msg);    
    void clearQueue();
    void callCustomFunctions(CMsg &msg) override;
};
