#include "fhmotor.h"


#define MOTOR_X_FG PJ_11
#define MOTOR_Y_FG PI_7
#define MOTOR_Z_FG PJ_10

volatile long _countx=0;
volatile long _county=0;
volatile long _countz=0;



//void PWMCounter::increment()
void incrementx()
{ 
  _countx++;      
}

void incrementy()
{
  _county++;   
}

void incrementz()
{
  _countz++;   
}


PWMCounter::PWMCounter(){}

void PWMCounter::config(PinName pin)       // create the InterruptIn on the pin specified to Counter
{
  _interrupt=pin;
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
    //_interrupt.rise(callback(this, &PWMCounter::increment)); // attach increment function of this counter instance  //SPI BUG
    pinMode(_interrupt, INPUT_PULLUP);

    if(_interrupt==MOTOR_Z_FG){     
      axis(_ZAXIS);
      attachInterrupt(digitalPinToInterrupt(_interrupt), incrementz, FALLING);    
    }
    if(_interrupt==MOTOR_Y_FG){     
      axis(_YAXIS);
      attachInterrupt(digitalPinToInterrupt(_interrupt), incrementy, FALLING);    
    }
    if(_interrupt==MOTOR_X_FG){     
      axis(_XAXIS);
      attachInterrupt(digitalPinToInterrupt(_interrupt), incrementx, FALLING);    
    }
    
  #else
     attachInterrupt(pin, incrementx, FALLING);      
  #endif  
}


long PWMCounter::read()   { return getCount();    }


long PWMCounter::getCount(){  
  long val=0;
  if(_axis==_ZAXIS ){    
    val= _countz;
  }
  if(_axis==_YAXIS ){    
    val= _county;    
  }
  if(_axis==_XAXIS ){    
    val= _countx;    
  }

 
_lastCount=_Count;
_lastTime=_Time;

_Time=getTime();
_Count=val;

long diff=_Count-_lastCount;

//writeconsole(_Count); writeconsole(" ");writeconsole(_lastCount);     writeconsole(" ");writeconsoleln(diff);     

return (diff);
}


double PWMCounter::getRPS(){      
  long curcount=getCount();

  if(curcount>0){
    _rps=(1000.0*curcount)/(_Time-_lastTime);    
    _rps/=6.0;  //Six pings a rev 
  }
  else{
    _rps=0;
  }


  return _rps;   
};

double PWMCounter::getRPM(){
  return 60.0*getRPS();   
}

  
CMotorController::CMotorController():CBaseDrive(){  
  myPID.begin(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd);   //Key this here  Need to make sure the PID is setup before the first loop or it crashes
  _channel=channel;
  channel++;
  setInterval(50);
};


CMotorController::~CMotorController(){
}


void CMotorController::config(PinName sig, PinName fg,PinName dir){
  init();
  
  PIN_SIGNAL=sig;
  PIN_DIR=dir;
  PIN_FG=fg;  
 

  cPin.config(PIN_SIGNAL);
  pwmCounter.config(PIN_FG);    
}


/*

void CMotorController::config(CMsg &msg){
  std::string strSignal=msg.getParameter("PIN_SIGNAL");
  std::string strDir=msg.getParameter("PIN_DIR");
  std::string strFG=msg.getParameter("PIN_FG");
  
  config(Pins[strSignal],Pins[strDir],Pins[strFG]);  
}
*/


void CMotorController::init(){
  CBaseDrive::init();

  _Setpoint=0.0;
  _Input=0.0;
  _Output=0.0;
  _Setpoint_last=0.0;
  _Output_last=0.0;

  //setState(_PLAY);  
}
  
double CMotorController::getRPM(){
  double tmp=pwmCounter.getRPM(); 

  if(!getDir())
    tmp*=-1.0;
  return tmp;
}

double CMotorController::getRPS(){
  double tmp=pwmCounter.getRPS();  
  if(!getDir())
    tmp*=-1.0;
  return tmp;
}

long CMotorController::getCount(){    
  return pwmCounter.getCount();  
}


void CMotorController::off(){
  speed(0.0);
  setState(_DONE);
}


void CMotorController::setPoint(CMsg &msg){
  _Setpoint=msg.get(_VALUE,_Setpoint);
}



void CMotorController::newGains(CMsg &msg){
  _Kd=msg.get(_KD,_Kd);
  _Ki=msg.get(_KI,_Ki);
  _Kp=msg.get(_KP,_Kp);
}

void CMotorController::speed(float val){  //from 0 to 100%
//sendPWM(2000);


  sendPlotter(_mLog);
  
  
  if((val<0)&&(getDir())){
    setDir(!getDir());            
    digitalWrite(PIN_DIR,LOW);  
    writeconsoleln("-------------------------------------DIR: LOW");
  }

  else if((val>=0)&&(!getDir())){  
    setDir(!getDir());    
    digitalWrite(PIN_DIR,HIGH);
    writeconsoleln("----------------------------------------------------DIR: HIGH");
  }
  
  cPin.pct(val);
}





void CMotorController::loop(){  
    CMsg m;
    runOnce(m);  
  }


void CMotorController::setup(){

  writeconsole("------------Setup    Mode:------------");writeconsoleln(Mode());
  if(Mode()==_LOCK)   //Stop Spinning
    setupLock();
  else if(Mode()==_SPEED)
    setupSpeed();
  else if((Mode()==_SIMPLE)||(Mode()==_MANUAL))
    setupSpeedSimple();
  else if(Mode()==_POSITION)  
    setupPosition();  
   
}


 void CMotorController::callCustomFunctions(CMsg &msg){
   writeconsoleln("CMotorController CallCustomFunctions :  ");
  
  std::string act=msg.get(_ACT); 
  float val=msg.get(_VALUE,0.0); 

  
  if(act==_GETRPM)  transmitResult(act,tostring(getRPM()));
  if(act==_GETRPS)  transmitResult(act,tostring(getRPS()));
  if(act==_GETCOUNT)  transmitResult(act,tostring(getCount()));
  
  mapcustommsg(newGains)
  mapcustommsg(setPoint)
  //if(act=="NEWGAINS") newGains(msg); 


  CBaseDrive::callCustomFunctions(msg);
 }