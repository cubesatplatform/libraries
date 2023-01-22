#include "mdrive.h"
#include <consoleio.h>


#define DEADBAND 10

  //  It uses .setDrive( motorNum <0,1>, direction<1:forward,0:backward>, level<0...255> ) to drive the motors.

CMDrive::CMDrive(){  
  //myPID.begin(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd);   //Key this here  Need to make sure the PID is setup before the first loop or it crashes
  
  setState(_BLANK);
  setInterval(10);
  Mode(_MANUAL);
  }


CMDrive::~CMDrive(){
  //Stop();
  }


void CMDrive::loop(){    
    CMsg m;
    runOnce(m);  
    sendPlotter();
  }  

void CMDrive::runOnce(CMsg &m){  
  if(Mode()==_BLANK)  {}
  if(Mode()==_DETUMBLE)  {detumble();}
  if(Mode()==_MANUAL)  {manual();}
}


void CMDrive::activate(){  
    _active=true;
    writeconsoleln("MDrive Activate");
    //setState(_PLAY);  
  }  


void CMDrive::deactivate(){  
    _active=false;
    writeconsoleln("MDrive DeActivate  Set speed 0");
    speed(0.0);
    
  }  




void CMDrive::detumble(){
  if(_active){
  writeconsoleln("Detumbling...");

  
  CMsg msg=getDataMap(_IMU+_GYRO);
  //CMsg msgs=getDataMap(_IMU+GYRO);

  std::string field;
  if (name()==_MAGX) field=_XAXIS;
  if (name()==_MAGY) field=_YAXIS;
  if (name()==_MAGZ) field=_ZAXIS;

  float x=msg.get(field,0.0);

  _Input=x;

  if (x>DEADBAND) {
    activateDrive(100.0);
    _Output=1.0;
    return;
  }

  if (x<-DEADBAND) {
    activateDrive(-100.0);
    _Output=-1.0;
    return;
  }
  //setState(_DONE);
  }
  else{
    _Output=0.0;
    writeconsoleln("Doing nothing for detumbling");
  }

}



  
void CMDrive::config(char addr, TwoWire *twowire, int m){
  _address=addr;
  _pWire=twowire;
  setMotor(m);   
  setForever();
  setInterval(5);  
  setModifiedTime(getTime());
  setDuration(200000);
  setErrorThreshold(5);

  }

  
void CMDrive::config(CMsg &msg){
  std::string straddress=msg.get(_ADDRESS);
  std::string strwire=msg.get(_WIRE);

  TwoWire *pWire;

  if(strwire==_WIRE) pWire=&Wire;  
  if(strwire==_WIRE1) pWire=&Wire1;  
  if(strwire==_WIRE2) pWire=&Wire2;  

  if(straddress.size()>0){
    config(straddress[0],pWire);
  }
}



void CMDrive::init(){      
    delay(100);
             
  for(int count=0;count<10;count++){  
    int x=!myMotorDriver.begin(_address, _pWire);  //0=success  1=fail

/*
        setState(_PLAY);                           ///This is a hack to get the motor to work
        myMotorDriver.run(RELEASE);
        return;
*/

    if( x ) {//Wait until a valid ID word is returned          
      writeconsole(name()); writeconsoleln(_address);
      incErrorCount();        

    _pWire->beginTransmission(_address);
    if (_pWire->endTransmission() == 0) {
        writeconsole(name()); writeconsole(" WORKED ");writeconsoleln(_address);
    }
    

      }
      else{
        writeconsoleln(name()); writeconsoleln("  Success");
        setState(_PLAY);
        myMotorDriver.run(RELEASE);
        return;
      }
      delay(100);
    } 
  sendError();
  return;
}




void CMDrive::address(char c){_address=c;}

 

void CMDrive::activateDrive(float val){   //0-1000
  
 
  writeconsole(name());writeconsole("  Speed:");
  writeconsole(val);
  writeconsoleln("   CMDrive ---   ACTIVATE DRIVE !!");

  if (val==0){
    
  }
  

  if((val>0.0)&&(!getDir())){
    setDir(true);
    myMotorDriver.run(FORWARD);
  }

  if((val<0.0)&&(getDir())){
    setDir(false);
    myMotorDriver.run(BACKWARD);
  }      

  myMotorDriver.setSpeed(convertToPWM(val));  
}


void CMDrive::test(CMsg &msg){
  _pWire->begin();
  delay(200);
  Mode(_MANUAL);
  run(550);

  if(state()!=_PLAY){
    setState(_BLANK);
    writeconsoleln("State wrong  Leaving");
    return;
  }

  
   
  writeconsoleln("BACK");
  myMotorDriver.run(RELEASE);
  myMotorDriver.run(BACKWARD);  


  myMotorDriver.setSpeed(254.0);   //Set Speed  0-255 Max  Its their driver
  delay(3000);

  writeconsoleln("FORWARD");
  myMotorDriver.run(RELEASE);
  myMotorDriver.run(FORWARD);  
  myMotorDriver.setSpeed(254.0);   //Set Speed  0-255 Max  Its their driver
  delay(3000);

   
  writeconsoleln("BACK");
  myMotorDriver.run(RELEASE);
  myMotorDriver.run(BACKWARD);  
  myMotorDriver.setSpeed(254.0);   //Set Speed  0-255 Max  Its their driver
  delay(3000);


  writeconsoleln("FORWARD");
  myMotorDriver.run(RELEASE);      
  myMotorDriver.run(FORWARD);  
  myMotorDriver.setSpeed(254.0);   //Set Speed  0-255 Max  Its their driver
  delay(3000);



  activateDrive(100.0);
  
  delay(2000);
  
  activateDrive(-100.0);
  
  delay(2000);
  
  activateDrive(0.0);
  speed(0.0);
}


void CMDrive::callCustomFunctions(CMsg &msg){   //Calls a specific function directly  
  std::string act=msg.get(_ACT);  

  msg.writetoconsole();

  mapcustom(activate)
  mapcustom(deactivate)

  CSystemObject::callCustomFunctions(msg);
}


void CMDrive::sendPlotter(){  
  int send=_m.get(_SEND,1);  


  if(send>0){

    long now=getTime();
    long interval=_m.get(_INTERVAL,500);
    long last=_m.get(_LAST,0);


    if((now-last)<interval)
      return;

    _m.set(_LAST,now);
    CMsg msg;
    msg.set(_TO,"PLOTTER");
    msg.set(_SYS,"PLOTTER");
    msg.set("INPUT",_Input);
    msg.set("OUTPUT",_Output);
    msg.set(_SETPOINT,_Setpoint);
        
    addTransmitList(msg);    
  }
}
