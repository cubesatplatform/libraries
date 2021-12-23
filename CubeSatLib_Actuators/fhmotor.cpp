#include "fhmotor.h"
#include <consoleio.h>
#include <arduino.h>

volatile unsigned long _countx=0;
volatile unsigned long _county=0;
volatile unsigned long _countz=0;
int CMotorController::channel=0;


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

void PWMCounter::init(PinName pin)       // create the InterruptIn on the pin specified to Counter
{
      _interrupt=pin;
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
    
    pinMode(_interrupt, INPUT_PULLUP);

    if(_interrupt==PI_6)
      attachInterrupt(digitalPinToInterrupt(_interrupt), incrementz, FALLING);    
    else if(_interrupt==PI_7)
      attachInterrupt(digitalPinToInterrupt(_interrupt), incrementy, FALLING);    
    else
      attachInterrupt(digitalPinToInterrupt(_interrupt), incrementx, FALLING);    
    //_interrupt.rise(callback(this, &PWMCounter::increment)); // attach increment function of this counter instance  //SPI BUG
  #else
     attachInterrupt(pin, incrementx, FALLING);      
  #endif
}


unsigned long PWMCounter::read()   { return getCount();    }


unsigned long PWMCounter::getCount(){
  #ifdef PORTENTA
  if(_interrupt==PI_6)
    return _countz;
  if(_interrupt==PI_7)
    return _county;
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
////////
    //float fcount=(float) diffcount;

   // Serial.println(lcount);
    //fcount/=6.0;  //6 pings per revolution
    //Serial.println(fcount);
    //float fsec=(float)lsec;
    //fsec/=1000.0;

    float ftmp=1000.0*(float)diffcount/(float)diffsec;
   // Serial.print(ftmp);
    ftmp=ftmp/6.0;  //Six pings a rev 
    //Serial.println(ftmp);
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


void CMotorController::configSpeed(PinName sig, PinName fg,PinName dir){
  #ifdef PORTENTA
  analogWriteResolution(12);
  #endif

  setMode("SPEED");
  PIN_SIGNAL=sig;
  PIN_DIR=dir;
  PIN_FG=fg;  

  pwmCounter.init(PIN_FG);
  pinMode(PIN_DIR, OUTPUT);

//Kp=2, Ki=5, Kd=1;
  myPID.init(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd, DIRECT);
   myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(50);
  myPID.SetOutputLimits(100, 1255);


  init();
}

void CMotorController::configRotation(PinName sig, PinName fg,PinName dir,CIMU *pIMU,char axis){
  init();
  #ifdef PORTENTA
  analogWriteResolution(12);
  #endif

  setMode("ROTATION");
  _pIMU=pIMU;
  _axis=axis;
  PIN_SIGNAL=sig;
  PIN_DIR=dir;
  PIN_FG=fg;  

  pwmCounter.init(PIN_FG);
  pinMode(PIN_DIR, OUTPUT);

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
  
    #ifdef TTGO
      ledcSetup(_channel, freq, resolution);   // configure LED PWM functionalitites
      ledcAttachPin(PIN_SIGNAL, _channel);  // attach the channel to the GPIO to be controlled
    #endif

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
  float fval;
  
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7) 
    writeconsoleln("Portenta: CMotorController  ---   ACTIVATE DRIVE !!!!!!!!!!");
    if(val<0)
      setDir(0);              
    else
      setDir(1);

    if(getDir()) digitalWrite(PIN_DIR,HIGH);
    if(!getDir()) digitalWrite(PIN_DIR,LOW);
    
      fval=val*4000.0;

       int nVal=abs(fval);

      Serial.println(nVal);
      analogWrite(PIN_SIGNAL,nVal);
    #else
      
      writeconsoleln("TTGO :CMotorController  ---   ACTIVATE DRIVE !!!!!!!!!!");
      
      writeconsole(".............................. Val :    ");
      writeconsole(val);      
      fval=val*255.0;
      writeconsole(".............................. Value :    ");
      writeconsoleln(fval);


    if(val<0)
      setDir(0);              
    else
      setDir(1);
    
    
    setMSpeed(val);
    setPWMSpeed(fval);

    if(getDir()) digitalWrite(PIN_DIR,HIGH);
    if(!getDir()) digitalWrite(PIN_DIR,LOW);

      short int sVal=abs(fval);
      ledcWrite(_channel, sVal); 
    #endif
  }


void CMotorController::sendPWM(int nVal){
   #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  analogWrite(PIN_SIGNAL,nVal);
  #else
    ledcWrite(_channel, nVal);  
  #endif
}

void CMotorController::loopSpeed(){
    _Input =RPS();
    myPID.Compute();
    
   sendPWM(_Output);
  }

void CMotorController::loopRotation(){
  CMsg m;
  _pIMU->runOnce(m);
  if(_axis=='X')
    _Input=_pIMU->Gyro.X;
  if(_axis=='Y')
    _Input=_pIMU->Gyro.Y;    
  if(_axis=='Z')
    _Input=_pIMU->Gyro.Z;
  if(_axis=='R')
    _Input=_pIMU->Gyro.R;
    //_Input =RPS();
    _Output_last=_Output;
    myPID.Compute();
    if(_Output_last ==_Output)
      return;

writeconsole(_axis) ;writeconsole("    Input: ");writeconsole((float)_Input);writeconsole("    Output: ");writeconsoleln((float)_Output);
  if(_Output>=0.0){
    
    if(getDir()) {
      digitalWrite(PIN_DIR,HIGH);
      setDir(true);
      delay(20);
      }    
    sendPWM(_Output);

    }

  if(_Output<0.0){    
    if(getDir()) {
      digitalWrite(PIN_DIR,LOW);
      setDir(false);
      delay(20);
      }    
    sendPWM(_Output*(-1.0));

    }

   
  }

void CMotorController::loop(){
  if(Mode()=="SPEED")
    loopSpeed();

  if(Mode()=="ROTATION")    
    loopRotation();
    
  }



