#include "fhmotor.h"
#include <consoleio.h>

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
  
  unsigned long diffsec=(lastT-prevT);
  if(diffsec>30){
    unsigned long curcount=getCount();
    unsigned long diffcount=curcount-lastCount;  //diff counts
    /////// Update for next cycle
    prevT=lastT;
    lastCount=curcount;


    _rps=1000.0*(float)diffcount/(float)diffsec;  
    _rps=_rps/6.0;  //Six pings a rev 
  }
  return _rps;   
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
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)  
    analogWriteResolution(12);
  #else  
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
  _Kp=150.0, _Ki=14.0, _Kd=3.0;
  myPID.begin(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd);
  myPID.setBias(4000.0 / 2.0);
  myPID.setWindUpLimits(-10, 10); // Groth bounds for the integral term to prevent integral wind-up
  
  //myPID.SetSampleTime(50); //commented out in code
  
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)     
    myPID.setOutputLimits(150, 4000);   
  #else          
    myPID.setOutputLimits(150, 4000);
  #endif
  myPID.start();
  
}


void CMotorController::configRotation(CIMU *pIMU,char axis){  
  setMode("ROTATION");
  _pIMU=pIMU;
  _axis=axis;
  
  //Kp=2, Ki=5, Kd=1;
  _pIMU->myIMU.enableGyro(50);
  _Setpoint=0.0;
  _Kp=5.0; _Ki=45.0; _Kd=5.0;
   _Kp=150.0, _Ki=14.0, _Kd=3.0;
  myPID.begin(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd);
  myPID.setBias(4000.0 / 2.0);
  myPID.setWindUpLimits(-10, 10); // Groth bounds for the integral term to prevent integral wind-up

  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)     
    myPID.setOutputLimits(150, 4000);   
  #else          
    myPID.setOutputLimits(150, 4000);
  #endif
  myPID.start();
  

}



void CMotorController::init(){
  CBaseDrive::init();

  _Setpoint=100.0;
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

void CMotorController::activateDrive(int val){

  setMSpeed(val);
  int nVal;
  
  if((val<0)&&(getDir())){
    setDir(0);              
    //digitalWrite(PIN_DIR,LOW);  
  }

  if((val>=0)&&(!getDir())){  
    setDir(1);
  //  digitalWrite(PIN_DIR,HIGH);
  }

  nVal=abs(val); 
      
        
  //writeconsole("           PWM :");     writeconsoleln(val);
  sendPWM(nVal);

}




void CMotorController::loopSpeed(){

   _Input =RPS();
   myPID.compute();
   
    writeconsole("Setpoint: ");
    writeconsole((float)_Setpoint);  
    writeconsole("Input:  ");      
    writeconsole(_Input);  
    writeconsole("      Output PWM: ");
    writeconsole((float)_Output);  
    float ftmp=(float)_Output;

    // ftmp=(ftmp/120.0)*4000.0;
    // ftmp+=getMSpeed();
    writeconsoleln("------ End  ");
    //activateDrive((int)_Output);
    activateDrive((int)ftmp);

}




void CMotorController::loopSpeedSimple(){
    
  _Input =RPS();
  float diff=_Setpoint-_Input;

  if(diff>40) 
    _Output+=400;    

  else if(diff>20) 
    _Output+=200;    
    
  else if(diff>10) 
    _Output+=100;  
  
  else if(diff>5) 
    _Output+=50;      

  else if(diff>3) 
    _Output+=10;          

  else if(diff>1) 
    _Output+=3;          

  else if(diff>.1) 
    _Output+=1;      

    else if(diff<-.1) 
    _Output-=1;      

else if(diff<-1) 
    _Output-=3;          

else if(diff<-3) 
    _Output-=10;          

else if(diff<-5) 
    _Output-=50;      

else if(diff<-10) 
    _Output-=100;  
  
else if(diff<-20) 
    _Output-=200;    
    
else if(diff<-40) 
    _Output-=400;    


if(_Output<100 ) _Output=100;
if(_Output>4000 ) _Output=4000;
    
  writeconsole("Simple Setpoint: ");
  writeconsole((float)_Setpoint);  
  writeconsole("    Input: ");
  writeconsole((float)_Input);
  writeconsole("    Onput: ");  
  writeconsole((float)_Output);  
  writeconsoleln("------ End  ");
  activateDrive((int)_Output);
  
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
  myPID.compute();
  writeconsole("loopRotation  "); writeconsoleln((float)_Output);  
  if(_Output_last ==_Output)
    return;

  writeconsole(_axis) ;writeconsole("    Input: ");writeconsole(_Input);writeconsole("    Output: ");writeconsoleln(_Output);
  if(_Output>=0.0){
    
    if(getDir()) {
      digitalWrite(PIN_DIR,HIGH);
      setDir(true);
      delay(20);
      }    
    activateDrive((int)_Output);
    }

  if(_Output<0.0){    
    if(getDir()) {
      digitalWrite(PIN_DIR,LOW);
      setDir(false);
      delay(20);
      }    
    activateDrive((int)_Output*(-1));
  } 
} 

void CMotorController::runOnce(CMsg &m){
  if(Mode()=="SPEED")
    loopSpeed();
  if(Mode()=="SPEEDSIMPLE")
    loopSpeedSimple();

  if(Mode()=="ROTATION")  
    loopRotation();
  
}


void CMotorController::test(CMsg &msg) {
int sspeed=0;  

configSpeed();

setMode("SPEED");

float f=msg.getParameter("SETPOINT",101.0);
setPoint(f);

unsigned long tt=getTime()+50000;
/*

for (int count=0;count<4000;count++){
  activateDrive(count);
  delay(200);
  RPM();
  delay(100);
  writeconsole(count);writeconsole(",");writeconsoleln(RPM());
}
writeconsoleln(1500);
activateDrive(1500);
delay(2000);
writeconsoleln(500);
activateDrive(500);
delay(2000);
writeconsoleln(1);
activateDrive(1);
delay(1000);
*/
//digitalWrite(PIN_DIR,HIGH);

while(getTime()<tt){
 loop(); 
  delay(30);
  }


Speed(10,10000);

}
