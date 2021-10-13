#pragma once
#include "defs.h"
#include "basedrive.h"
#include "SCMDW.h"
#include "SCMDW_config.h" //Contains #defines for common SCMD register names and values


#include <Wire.h>
  //  It uses .setDrive( motorNum <0,1>, direction<1:forward,0:backward>, level<0...255> ) to drive the motors.

//classSCMDrive:public CSystemObject{
  class SCMDrive:public CBaseDrive{
 // char _address=0x5D;
  char _address=0x60;
//  int _motor=0;
 // int _mspeed=0;
 // int _mdir=1;
 //  unsigned long _duration=0;
 //  unsigned long _changedOn;
  TwoWire *pWire=&Wire;


  SCMDW myMotorDriver; //This creates the main object of one motor driver and connected slaves.
public:
 SCMDrive();
 SCMDrive(const char * str,char addr, int m=0);
  ~SCMDrive();  
  void config(const char * str,char addr=0x60, TwoWire &twowire=Wire,int m=0);
  void Address(char c);
  bool init();
  void activateDrive(float val, bool dir=true, int motor=0);
 
 // void Forward(float s=255,unsigned long dur=0);
 // void Backward(float s=255,unsigned long dur=0);
 // void Reverse();
 // bool isForward();
 // void Speed(float s,unsigned long dur=0);
 // int Speed(){return _mspeed;};
  //int Motor(){return _motor;};
  //unsigned long Duration(){return _duration;};
  //void Duration(unsigned long tmp){_duration=tmp;}
 

  //void Stop();

 
 
  //void loop();
  //void runOnce();
  //void TestMotor();
  /*

  void CallFunction(CMsg &msg){   //Calls a specific function directly
    std::string act=msg.getParameter("ACT");
    std::string callback=msg.getParameter("CALLBACK");
    float speed=msg.getParameter("SPEED",(float)1.0);
    unsigned long duration=msg.getParameter("DURATION",50);

    if(callback.size()) _cmsg.Parameters["CALLBACK"]=callback;
    if (act=="Forward") Forward(speed);
    if (act=="Back") Backward(speed);
    if (act=="Reverse")Reverse();
    if (act=="Stop") Stop();
    if (act=="Speed") Speed(speed);
  }
  */
};
