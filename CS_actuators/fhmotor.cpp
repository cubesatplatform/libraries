#include "fhmotor.h"


#define MOTOR_X_FG PJ_11
#define MOTOR_Y_FG PI_7
#define MOTOR_Z_FG PJ_10

volatile unsigned long _countx=0;
volatile unsigned long _county=0;
volatile unsigned long _countz=0;



//void PWMCounter::increment()
void incrementx()
{
  //writeconsole("_x");
    _countx++;      

}

void incrementy()
{
 // writeconsole("_y");
    _county++;   
}

void incrementz()
{
  //writeconsole("_z");
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
      axis('Z');
      attachInterrupt(digitalPinToInterrupt(_interrupt), incrementz, FALLING);    
    }
    if(_interrupt==MOTOR_Y_FG){     
      axis('Y');
      attachInterrupt(digitalPinToInterrupt(_interrupt), incrementy, FALLING);    
    }
    if(_interrupt==MOTOR_X_FG){     
      axis('X');
      attachInterrupt(digitalPinToInterrupt(_interrupt), incrementx, FALLING);    
    }
    
  #else
     attachInterrupt(pin, incrementx, FALLING);      
  #endif  
}


unsigned long PWMCounter::read()   { return getCount();    }


unsigned long PWMCounter::getCount(){
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  if(_axis=='Z' ){    
    return _countz;
  }
  if(_axis=='Y' ){    
    return _county;
  }
  if(_axis=='X' ){    
    return _countx;
  }
#endif  
return _countx;

}


double PWMCounter::RPS(){
  lastT=millis();   // may want micros
  
  unsigned long diffsec=(lastT-prevT);
  if(diffsec>30){
    unsigned long curcount=getCount();
    unsigned long diffcount=curcount-lastCount;  //diff counts
    /////// Update for next cycle
    prevT=lastT;
    lastCount=curcount;


    _rps=1000.0*(double)diffcount/(double)diffsec;  
    _rps=_rps/6.0;  //Six pings a rev 
  }

  
  return _rps;   
};

double PWMCounter::RPM(){
  return 60.0*RPS();   
}

  
CMotorController::CMotorController():CBaseDrive(){  
  _channel=channel;
  channel++;
  setInterval(25);
};


CMotorController::~CMotorController(){
}


void CMotorController::config(PinName sig, PinName fg,PinName dir){
  init();
  
  PIN_SIGNAL=sig;
  PIN_DIR=dir;
  PIN_FG=fg;  
 
  #if !(defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7))
    analogWriteResolution(sig, PIN_RESOLUTION);
  #else
    analogWriteResolution(PIN_RESOLUTION);
  #endif
  pinMode(PIN_DIR, OUTPUT);
  pwmCounter.config(PIN_FG);  
  setState("PLAY");
}



void CMotorController::config(CMsg &msg){
  std::string strSignal=msg.getParameter("PIN_SIGNAL");
  std::string strDir=msg.getParameter("PIN_DIR");
  std::string strFG=msg.getParameter("PIN_FG");
  
  config(Pins[strSignal],Pins[strDir],Pins[strFG]);
  setState("PLAY");
}



void CMotorController::init(){
  CBaseDrive::init();

  _Setpoint=0.0;
  _Input=0.0;
  _Output=0.0;
  _Setpoint_last=0.0;
  _Output_last=0.0;

  setState("PLAY");
   activateDrive((int)_Output);  
}
  
double CMotorController::RPM(){
  double tmp=pwmCounter.RPM(); 

  if(!getDir())
    tmp*=-1.0;
  return tmp;
}

double CMotorController::RPS(){
  double tmp=pwmCounter.RPS();  
  if(!getDir())
    tmp*=-1.0;
  return tmp;
}

unsigned long CMotorController::getCount(){    
  return pwmCounter.getCount();  
}


void CMotorController::off(){
  activateDrive(0);
  setState("DONE");
}


void CMotorController::echoData(CMsg &msg){
  CMsg m=getDataMap("GYRO");

  m.setParameter("Name",Name());
  m.setParameter("Mode",Mode());
  m.setParameter("Dir",getDir());
  m.setParameter(" SP",_Setpoint);
  m.setParameter(" SP Last",_Setpoint_last);
  m.setParameter(" Dur",_Output_duration);
  m.setParameter(" In",_Input);
  m.setParameter(" Out",_Output);
  m.setParameter(" Kp",_Kp);
  m.setParameter(" Ki",_Ki);
  m.setParameter(" Kd",_Kd);
  
  addTransmitList(m);
};



void CMotorController::newGains(CMsg &msg){
  _Kd=msg.getParameter("KD",_Kd);
  _Ki=msg.getParameter("KI",_Ki);
  _Kp=msg.getParameter("KP",_Kp);
}

void CMotorController::activateDrive(int val){

  if(!setMSpeed(val)) {  //Check if same speed as last time
    writeStats();
    return;
  }
  writeStats();
  int nVal;
  
  if((val<0)&&(getDir())){
    setDir(0);            
    digitalWrite(PIN_DIR,LOW);  
  }

  if((val>=0)&&(!getDir())){  
    setDir(1);    
    digitalWrite(PIN_DIR,HIGH);
  }

  nVal=abs(val); 

  if(nVal<0 ) nVal=0;
  if(nVal>PWM_MAX) nVal=PWM_MAX;

  writeconsole("      nVal: ");
  writeconsoleln(nVal);
          
  sendPWM(nVal);
}

void CMotorController::writeStats(){
  if(getTime()<_lastStats+1000)
  return;
  _lastStats=getTime();
 
  CMsg m;

  m.setParameter("Name",Name());
  m.setParameter("Mode",Mode());
  m.setParameter("Direction",getDir());
  m.setParameter(" Setpoint",_Setpoint);
  m.setParameter(" Setpoint Last",_Setpoint_last);
  m.setParameter(" Duration",_Output_duration);
  m.setParameter(" Input",_Input);
  m.setParameter(" Output",_Output);
  m.writetoconsole(); 
  
}



void CMotorController::loop(){  
    CMsg m;
    runOnce(m);  
  }


void CMotorController::setup(){

  writeconsole("Setup    Mode:");writeconsoleln(Mode());
  if(Mode()=="LOCK")
    setupLock();
  else if(Mode()=="SPEED")
    setupSpeed();
  else if(Mode()=="SIMPLE")
    setupSpeedSimple();
  else if(Mode()=="ROTATION")  
    setupRotation();  
  else if(Mode()=="RAMP")  
    setupRamp();  
  else if(Mode()=="PWM")  
    setupPWM();  
 setState("PLAY");
}


 void CMotorController::callCustomFunctions(CMsg &msg){
  std::string act=msg.getACT(); 
  
  writeconsoleln("CMotorController CallCustomFunctions :  ");
  if(act=="NEWGAINS") {newGains(msg); return; }
 }