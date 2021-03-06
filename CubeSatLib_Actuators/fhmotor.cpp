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
  //Name("MOTOR");
  _channel=channel;
  channel++;
};


CMotorController::~CMotorController(){
}


void CMotorController::config(PinName sig, PinName fg,PinName dir){
  init();
  
  
  
  PIN_SIGNAL=sig;
  PIN_DIR=dir;
  PIN_FG=fg;  
 
  #ifdef TTGO || TTGO1
    analogWriteResolution(sig, PIN_RESOLUTION);
  #else
    analogWriteResolution(PIN_RESOLUTION);
  #endif
  pinMode(PIN_DIR, OUTPUT);
  pwmCounter.config(PIN_FG);  
}





void CMotorController::init(){
  CBaseDrive::init();

  _Setpoint=100.0;
  _Input=0.0;
  _Output=0.0;
  _Output_last=0.0;

  setState("PLAY");
  setMode("");
  Speed(0,10000);
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
  if(nVal>4000) nVal=4000;

  writeconsole("      nVal: ");
  writeconsoleln(nVal);
    
      
  sendPWM(nVal);

}

void CMotorController::writeStats(){
  writeconsole(PIN_SIGNAL);
  writeconsole(Name());  writeconsole(" ");
  writeconsole(Mode());  writeconsole(" ");
  writeconsole("   Setpoint: ");
  writeconsole((float)_Setpoint);  
  writeconsole("Input:  ");      
  writeconsole(_Input);  
  writeconsole("      Output: ");
  writeconsoleln((float)_Output);

  
  
}

void CMotorController::loopPWM(){  
  RPS();
  activateDrive((int)_Setpoint);    
}



void CMotorController::loopSpeed(){

  _Input =RPS();
  myPID.compute();
  if(_Output<20 ) _Output=20;
  if(_Output>4000) _Output=4000;
    

  if((_Output<10)&&(_Output>0))
    _Output=5;
  
  activateDrive((int)_Output);    
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


 activateDrive((int)_Output);
  
}

void CMotorController::loopRotation(){

  if(_pIMU==NULL) {writeconsoleln("NO IMU!!!!!!!!!!!!!!!!!!!!!");return;}
  CMsg m;

  _pIMU->runOnce(m);
  if(_axis=='X')    _Input=*_pIMU->Gyro.pZ;
  if(_axis=='Y')    _Input=*_pIMU->Gyro.pY;    
  if(_axis=='Z')    _Input=*_pIMU->Gyro.pX;
  if(_axis=='R')    _Input=*_pIMU->Gyro.pR;
  
  _Output_last=_Output;
  myPID.compute();
 
  activateDrive((int)_Output);  
} 


void CMotorController::loopRamp(){
  if(_Setpoint>4000) _Setpoint=0;
  _Setpoint+=20;
  activateDrive(_Setpoint);  
    
  

/*
writeconsoleln(1500);
activateDrive(1500);
delay(2000);
writeconsoleln(500);
activateDrive(500);
delay(2000);
writeconsoleln(1);
activateDrive(1);
delay(1000);

//digitalWrite(PIN_DIR,HIGH);
activateDrive(4000);
delay(2000);
int val=2000;
activateDrive(val);
float ftmp;
while(getTime()<tt){
 //loop(); 
  ftmp=RPS();
  flist.push_back(ftmp);
  delay(10);
  }

writeconsole("Value: ");writeconsoleln(val);
 for(auto x:flist) {
   writeconsoleln(x);
 }
 */
}

void CMotorController::runOnce(CMsg &m){
  writeconsoleln(Mode());
  if(Mode()=="SPEED")
    loopSpeed();
  if(Mode()=="SIMPLE")
    loopSpeedSimple();
  if(Mode()=="ROTATION")  
    loopRotation();  
  if(Mode()=="RAMP")  
    loopRamp();  
  if(Mode()=="PWM")  
    loopPWM();  
}


void CMotorController::configRotation(CIMU *pIMU){  
  setIMU(pIMU);
  std::string str=Name();
  if(str.size()){
    char c=str[str.size()-1];
    _axis=c;
  }
  else _axis='Z';
  
  _Kp=3332.0, _Ki=10.10, _Kd=0.0;
  myPID.begin(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd);

  // myController.reverse()               // Uncomment if controller output is "reversed"
  // myController.setSampleTime(10);      // OPTIONAL - will ensure at least 10ms have past between successful compute() calls
  
  myPID.setBias(4000.0 / 2.0);  // was 2.0
  myPID.setWindUpLimits(-10000000000, 1000000000); // Groth bounds for the integral term to prevent integral wind-up
  
  //myPID.SetSampleTime(50); //commented out in code
  
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)     
    myPID.setOutputLimits(10, 4000);   
  #else          
    myPID.setOutputLimits(10, 4000);
  #endif
  myPID.start();
  Speed(0,10000);


}

void CMotorController::configSpeed(){      
  _Kp=32.0, _Ki=0.10, _Kd=0.0;
  myPID.begin(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd);

  // myController.reverse()               // Uncomment if controller output is "reversed"
  // myController.setSampleTime(10);      // OPTIONAL - will ensure at least 10ms have past between successful compute() calls
  
  myPID.setBias(4000.0 / 2.0);  // was 2.0
  myPID.setWindUpLimits(-10000000000, 1000000000); // Groth bounds for the integral term to prevent integral wind-up
  
  //myPID.SetSampleTime(50); //commented out in code
  
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)     
    myPID.setOutputLimits(10, 4000);   
  #else          
    myPID.setOutputLimits(10, 4000);
  #endif
  myPID.start();
  Speed(0,10000);

  // myController.reset();               // Used for resetting the I and D terms - only use this if you know what you're doing
  // myController.stop();                // Turn off the PID controller (compute() will not do anything until start() is called)
  
}


void CMotorController::test(CMsg &msg) {
std::list<float> flist; 
int sspeed=0;  

std::string testMode=msg.getParameter("MODE","PWM");
setMode(testMode);  

float setpt=msg.getParameter("SETPOINT",1787.0);
int duration=msg.getParameter("DURATION",10000);

if(testMode=="ROTATION"){
  CIMU *pIMU=(CIMU *)getSystem("IMUSPI");
  if(pIMU!=NULL) {    
   // pIMU->setup();
    pIMU->test(msg);
  }

  configRotation(pIMU);
  
  setPoint(setpt,duration);
}
else{
  configSpeed();
  setPoint(setpt,duration);
}


unsigned long tt=getTime()+duration;

while(getTime()<tt){
  loop();   
  delay(30);
  }


Speed(0,10000);

}
