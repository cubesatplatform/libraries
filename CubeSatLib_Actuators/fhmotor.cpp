#include "fhmotor.h"

int CMotorController::channel=0;

PWMCounter::PWMCounter(PinName pin) : _interrupt(pin)          // create the InterruptIn on the pin specified to Counter
{
  #ifdef PORTENTA
   // _interrupt.rise(callback(this, &PWMCounter::increment)); // attach increment function of this counter instance  //SPI BUG
  #endif
}

void PWMCounter::increment()
{
    _count++;
   
}

int PWMCounter::read()   { return _count;    }


long PWMCounter::count(){return _count;}


float PWMCounter::RPM(){
    lastT=millis();   // may want micros
    long lcount=_count-lastCount;  //diff counts
    long lsec=(lastT-prevT);
    float fcount=(float) lcount;
    fcount/=6.0;  //6 pings per revolution

    float fsec=(float)lsec;
    fsec/=1000.0;

   


   

    prevT=lastT;
    lastCount=_count;

    return 60.0*fcount/fsec;   
};


  
  CMotorController::CMotorController():CBaseDrive(){
    Name("Motor");
    _channel=channel;
    channel++;
    };


  CMotorController::~CMotorController(){
    #ifdef PORTENTA
    if (pCounter!=NULL) delete pCounter;
    if (pMotor!=NULL) delete pMotor;
    #endif
  }


void CMotorController::config(const char  *str,PinName sig, PinName fg,PinName dir){
  Name(str);
  PIN_SIGNAL=sig;
  PIN_DIR=dir;
  PIN_FG=fg;  

  pCounter=new PWMCounter(PIN_FG);
  pinMode(PIN_DIR, OUTPUT);


  Init();
}



  void CMotorController::Init(){
   
    #ifdef PORTENTA      
      pMotor=new mbed::PwmOut(PIN_SIGNAL);
      pMotor->period_ms(1);   //PWM signal must have a fixed frequency between 500Hz and 18Khz.   1==1000Hz   2=500Hz
      pMotor->write(0.0);      //Percent from 0 to 1
    #endif

    #ifdef TTGO
      ledcSetup(_channel, freq, resolution);   // configure LED PWM functionalitites
      ledcAttachPin(PIN_SIGNAL, _channel);  // attach the channel to the GPIO to be controlled
    #endif

     setState("READY");
  }
  
  float CMotorController::RPM(){
    #ifdef PORTENTA
    return pCounter->RPM();
    #else
    return 0;
    #endif
  }

  long CMotorController::Count(){
    #ifdef PORTENTA
    return pCounter->count();
    #else
    return 0;
    #endif
  }

void CMotorController::activateDrive(float val, bool dir, int motor){
  writeconsoleln("CMotorController ---   ACTIVATE DRIVE !!!!!!!!!!");
  #ifdef PORTENTA    
       
      pMotor->write(val);
    #else
      
    float fval;
      
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





/*                   
  void CMotorController::Speed(float s,unsigned long dur){
    if(_startTime==0) _startTime=getTime();
    _changedOn=getTime();
     if(s<0){
      s*=-1.0;
      _mdir=0;
    }
    else
      _mdir=1;

    _mspeed=s;

    #ifdef PORTENTA    
       
      pMotor->write(fspeed);
    #else
      int val=255.0*_mspeed;
      ledcWrite(_channel, val); 
    #endif
  }


void  CMotorController::Log(){
  writeconsole("speed: ");
  writeconsole(_mspeed);
  writeconsole("  channel:");
  writeconsole(_channel);
  writeconsole("  dir:");
  writeconsoleln(_mdir);
  
}

  int CMotorController::Speed(){
    #ifdef PORTENTA  
    return pCounter->RPM();    
    #else
    return 0;
    #endif
  }


  void CMotorController::Forward(float s,unsigned long dur){
    _changedOn=getTime();
    _mdir=true;    
    digitalWrite(PIN_DIR,HIGH);    
    Log();
  }

  void CMotorController::Backward(float s,unsigned long dur){
    _changedOn=getTime();
    _mdir=false;    
    digitalWrite(PIN_DIR,LOW);    
    Log();
  }
  
  void CMotorController::Reverse(){
    _changedOn=getTime();
    _mdir!=_mdir;    
    if(_mdir) digitalWrite(PIN_DIR,HIGH);
    if(!_mdir) digitalWrite(PIN_DIR,LOW);
    Log();
  }



  void CMotorController::Test() {
  float ss=1.0;

while(ss>0.0){
  
  //MotorX.Forward();
  Speed(1.0*ss/3.0);
  writeconsoleln("Forward...");
  delay(1000);
  //MotorX.Backward();  
  Speed(-1.0*ss/3.0);
  writeconsoleln("Backward...");
  
  delay(1000);

  ss-=.03;
}
  }


void CMotorController::loop(){
  runOnce();
}

void CMotorController::runOnce(){
      if(_duration<1){
    }
  
    unsigned long ct=getTime();
    if(ct>_changedOn+_duration){
      Stop();
      setState("IDLE");
    }

}

  void CMotorController::Stop(){
    Speed(0.0,0);
  }

  */
