#include "fhmotor.h"
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
  #ifdef PORTENTA
    
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


unsigned long PWMCounter::read()   { return count();    }


unsigned long PWMCounter::count(){
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
    unsigned long curcount=count();
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
    Name("Motor");
    _channel=channel;
    channel++;
    };


  CMotorController::~CMotorController(){

  }


void CMotorController::config(const char  *str,PinName sig, PinName fg,PinName dir){
  #ifdef PORTENTA
  analogWriteResolution(12);
  #endif
  Name(str);
  PIN_SIGNAL=sig;
  PIN_DIR=dir;
  PIN_FG=fg;  

  pwmCounter.init(PIN_FG);
  pinMode(PIN_DIR, OUTPUT);

//Kp=2, Ki=5, Kd=1;
  myPID.init(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd, DIRECT);
   myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(50);
 // SetOutputLimits(0,100);


  Init();
}



  void CMotorController::Init(){
    #ifdef TTGO
      ledcSetup(_channel, freq, resolution);   // configure LED PWM functionalitites
      ledcAttachPin(PIN_SIGNAL, _channel);  // attach the channel to the GPIO to be controlled
    #endif

     setState("READY");
  }
  
  float CMotorController::RPM(){
    
    return pwmCounter.RPM();
    
  }

  float CMotorController::RPS(){
    return pwmCounter.RPS();
    
  }

  unsigned long CMotorController::Count(){
    
    return pwmCounter.count();
    
  }

void CMotorController::activateDrive(float val, bool dir, int motor){
  float fval;
  
  #ifdef PORTENTA    
    writeconsoleln("Portenta: CMotorController  ---   ACTIVATE DRIVE !!!!!!!!!!");
    if(val<0)
        _mdir=0;              
    else
      _mdir=1;

    if(_mdir) digitalWrite(PIN_DIR,HIGH);
    if(!_mdir) digitalWrite(PIN_DIR,LOW);
    
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
      _mdir=0;              
    else
      _mdir=1;
    
    
    _mspeed=val;
    _PWMSpeed=fval;

    if(_mdir) digitalWrite(PIN_DIR,HIGH);
    if(!_mdir) digitalWrite(PIN_DIR,LOW);

      short int sVal=abs(fval);
      ledcWrite(_channel, sVal); 
    #endif
  }


void CMotorController::sendPWM(int nVal){
   #ifdef PORTENTA   
  analogWrite(PIN_SIGNAL,nVal);
  #else
    ledcWrite(_channel, nVal);  
  #endif
}

void CMotorController::loop(){
    _Input =RPS();
    myPID.Compute();
   sendPWM(_Output);
  }



