#include "pwmpin.h"

int CPWMController::channel=0;


  

CPWMController::CPWMController(const char *str, PinName sig){
  _channel=channel;channel++;
  config(str, sig);

  }

  CPWMController::~CPWMController(){
    #ifdef PORTENTA    
    if (pMotor!=NULL) delete pMotor;
    #endif
  }


void CPWMController::config(const char *str, PinName sig){
  _INTERVAL=50;
    Name(str);
    PIN_SIGNAL=sig;
    Init();
}



  void CPWMController::Init(){
    
    #ifdef PORTENTA
      pMotor=new mbed::PwmOut(PIN_SIGNAL);
      pMotor->period_ms(1);   //PWM signal must have a fixed frequency between 500Hz and 18Khz.   1==1000Hz   2=500Hz
      pMotor->write(0.0);      //Percent from 0 to 1
    
    #endif

      
  #ifdef TTGO
    ledcSetup(_channel, freq, resolution);   // configure LED PWM functionalitites
    ledcAttachPin(PIN_SIGNAL, _channel);  // attach the channel to the GPIO to be controlled
  #endif

 
  }


void CPWMController::setup(){bOn=false;}
void CPWMController::activateDrive(float val, bool dir, int motor){ 
  writeconsoleln("PWMController ---   ACTIVATE DRIVE !!!!!!!!!!");
  #ifdef PORTENTA     
    pMotor->write(val);
  #else
      float fval=255.0*val;
      //fval=255.0;
     
      ledcWrite(_channel, (int) fval); 
 
  #endif

}

/*
void CPWMController::Speed(float x){
  if(_startTime==0)
    _startTime=getTime();
  x=abs(x);

  if(x>1.0) x=1.0;
  if(x<.03) x=0.0;
  _currentSpeed=x;
  _modifiedTime=getTime();
  #ifdef PORTENTA     
    pMotor->write(fspeed);
  #else
      float fval=255.0*x;
      //fval=255.0;
      writeconsole("speedspeedspeed");
      writeconsole(fval);
      ledcWrite(_channel, (int) fval); 
      if(fval>0.0) bOn=true;
      else bOn=false;
  #endif
  }


void  CPWMController::Log(){
  writeconsole("speed: ");
  writeconsole(_setSpeed);
  writeconsole("  channel:");
  writeconsole(_channel);
 
}

int CPWMController::Speed(){

  return _setSpeed;
  
}



  void CPWMController::Test() {
  float ss=1.0;

while(ss>0.0){
  
  //MotorX.Forward();
  Speed(1.0*ss/3.0);
  writeconsoleln("Forward...");
  delay(1000);
  
  ss-=.03;
}
  }



void CPWMController::Log(const char *str){
writeconsoleln("");
writeconsoleln(str);

writeconsole("currentspeed: ");
writeconsoleln(_currentSpeed);

writeconsole("setspeed: ");
writeconsoleln(_setSpeed);

writeconsole("maxRuntime: ");
writeconsoleln(_maxRunTime);

writeconsole("startTime: ");
writeconsoleln(_startTime);

writeconsole("modifiedTime: ");
writeconsoleln(_modifiedTime);

writeconsole("bOn: ");
writeconsoleln((int) bOn);
}


void CPWMController::loop(){

Log("Before");



writeconsoleln("E");
if(bOn&&_currentSpeed<0.01){
  writeconsoleln("F");
  Speed(_setSpeed);
  writeconsoleln("G");
  Log("E-G");
 
}

writeconsoleln("H");
if(!bOn&&_currentSpeed>0.01){
  writeconsoleln("I");
  Speed(0.0);
  writeconsoleln("J");
  Log("H-J");  
}


  writeconsoleln("A");
  unsigned long ct=getTime();
  writeconsoleln("B");
  if((_RunTime>0)&&(ct>(_maxRunTime+_startTime))){
    writeconsoleln("C");
    Speed(0.0);
    writeconsoleln("D");
    Log("A-D");    
  }


if (_interval){
  if(ct>(_modifiedTime+_interval)){
    _modifiedTime=ct;
    if(bOn){
      Speed(0.0);      
      return true;
    }
    else{
      Speed(_setSpeed);
      return true;
    }      
  }
}

}


void CPWMController::callNewFunction(CMsg &msg){
  std::string act=msg.getACT();  

  if(act=="TURNON") start();
  if(act=="TURNOFF") stop();  
}
*/