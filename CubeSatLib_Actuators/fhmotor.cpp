#include "fhmotor.h"
#include <consoleio.h>
#include <Arduino.h>

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


float PWMCounter::RPS(){
  lastT=millis();   // may want micros
  unsigned long curcount=getCount();
  unsigned long diffcount=curcount-lastCount;  //diff counts
  unsigned long diffsec=(lastT-prevT);

  /////// Update for next cycle
  prevT=lastT;
  lastCount=curcount;

  //float fcount=(float) diffcount;

  // writeconsoleln(lcount);
  //fcount/=6.0;  //6 pings per revolution
  //writeconsoleln(fcount);
  //float fsec=(float)lsec;
  //fsec/=1000.0;

  float ftmp=1000.0*(float)diffcount/(float)diffsec;
  // writeconsole(ftmp);
  ftmp=ftmp/6.0;  //Six pings a rev 
  //writeconsoleln(ftmp);
  return ftmp;   
};

float PWMCounter::RPM(){
  return 60.0*RPS();   
}

  
CMotorController::CMotorController():CBaseDrive(){
  Name("MOTOR");
  _channel=channel;
  channel++;
};


CMotorController::~CMotorController(){
}


void CMotorController::config(PinName sig, PinName fg,PinName dir){
  init();
  #ifdef PORTENTA
    analogWriteResolution(12);
  #endif
  
  PIN_SIGNAL=sig;
  PIN_DIR=dir;
  PIN_FG=fg;  
  
  pinMode(PIN_DIR, OUTPUT);
  pwmCounter.config(PIN_FG);  
}

void CMotorController::configSpeed(){  
  setMode("SPEED");
  _Kp=2, _Ki=5, _Kd=1;
  myPID.init(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd, DIRECT);
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(50);
  myPID.SetOutputLimits(100, 1255);
}

void CMotorController::configRotation(CIMU *pIMU,char axis){
  setMode("ROTATION");
  _pIMU=pIMU;
  _axis=axis;
  
  //Kp=2, Ki=5, Kd=1;
  _pIMU->myIMU.enableGyro(50);
  _Setpoint=0.0;
  _Kp=5.0; _Ki=45.0; _Kd=5.0;
  myPID.init(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd, DIRECT);
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(50);
  myPID.SetOutputLimits(-1255, 1255);
}



void CMotorController::init(){
  CBaseDrive::init();

  _Setpoint=0.0;
  _Input=0.0;
  _Output=0.0;
  _Output_last=0.0;

  setState("PLAY");
}
  
float CMotorController::RPM(){
  return pwmCounter.RPM(); 
}

float CMotorController::RPS(){
  return pwmCounter.RPS();  
}

unsigned long CMotorController::getCount(){    
  return pwmCounter.getCount();  
}

void CMotorController::activateDrive(float val){
  setMSpeed(val);
  int nVal;
  
  if((val<0)&&(getDir())){
    setDir(0);              
    digitalWrite(PIN_DIR,LOW);  
  }

  if((val>=0)&&(!getDir())){  
    setDir(1);
    digitalWrite(PIN_DIR,HIGH);
  }

      
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7) 
    writeconsole("Portenta: CMotorController  ---   ACTIVATE DRIVE !!!!!!!!!!   ");    writeconsoleln(val);
    nVal=abs(val*1000.0);  
  #else      
    writeconsole("TTGO :CMotorController  ---   ACTIVATE DRIVE !!!!!!!!!!");     writeconsoleln(val);
    nVal=abs(val*250.0);  
  #endif
        

  sendPWM(nVal);

}




void CMotorController::loopSpeed(){
  
  _Input =RPS();
  myPID.Compute();  
  writeconsole("loopSpeed  "); writeconsoleln((float)_Output);  
  activateDrive(_Output);
  
}

void CMotorController::loopRotation(){
  CMsg m;
  if(_pIMU==NULL) {writeconsoleln("NO IMU!!!!!!!!!!!!!!!!!!!!!");return;}
  _pIMU->runOnce(m);
  if(_axis=='X')    _Input=_pIMU->Gyro.X;
  if(_axis=='Y')    _Input=_pIMU->Gyro.Y;    
  if(_axis=='Z')    _Input=_pIMU->Gyro.Z;
  if(_axis=='R')    _Input=_pIMU->Gyro.R;
  //_Input =RPS();
  _Output_last=_Output;
  myPID.Compute();
  writeconsole("loopRotation  "); writeconsoleln((float)_Output);  
  if(_Output_last ==_Output)
    return;

  writeconsole(_axis) ;writeconsole("    Input: ");writeconsole((float)_Input);writeconsole("    Output: ");writeconsoleln((float)_Output);
  if(_Output>=0.0){
    
    if(getDir()) {
      digitalWrite(PIN_DIR,HIGH);
      setDir(true);
      delay(20);
      }    
    activateDrive(_Output);
    }

  if(_Output<0.0){    
    if(getDir()) {
      digitalWrite(PIN_DIR,LOW);
      setDir(false);
      delay(20);
      }    
    activateDrive(_Output*(-1.0));
  } 
} 

void CMotorController::runOnce(CMsg &m){
  /*
  if(Mode()=="SPEED")
    loopSpeed();

  if(Mode()=="ROTATION")    
    loopRotation();
    */
}


void CMotorController::test() {
float sspeed=0.0;  

configSpeed();

Speed(.1,3000);

for(int count=0; count<100; count++){
  loop();
  delay(100);
  writeconsole(".");
  
}

}
