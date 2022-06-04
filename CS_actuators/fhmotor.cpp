#include "fhmotor.h"
#include <consoleio.h>


#define MOTOR_X_FG PJ_11
#define MOTOR_Y_FG PI_7
#define MOTOR_Z_FG PJ_10

volatile unsigned long _countx=0;
volatile unsigned long _county=0;
volatile unsigned long _countz=0;

#define PWM_MAX 4000





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
  setInterval(500);
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

  _Setpoint=2200.0;
  _Input=0.0;
  _Output=0.0;
  _Output_last=0.0;

  setState("PLAY");
  Speed(0,10000);
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
 
  CMsg m;

  m.setParameter("Name",Name());
  m.setParameter("Mode",Mode());
  m.setParameter("Direction",getDir());
  m.setParameter(" Setpoint",_Setpoint);
  m.setParameter(" Input",_Input);
  m.setParameter(" Output",_Output);
  m.writetoconsole(); 
  
}

void CMotorController::loopPWM(CMsg &msg){  
  _Input =RPS();
  activateDrive((int)_Setpoint);    
}



void CMotorController::loopSpeed(CMsg &msg){

  _Input =RPS();
  myPID.compute();
  if(_Output<20 ) _Output=20;
  if(_Output>PWM_MAX) _Output=PWM_MAX;
    

  if((_Output<10)&&(_Output>0))
    _Output=5;
  
  activateDrive((int)_Output);    
}




void CMotorController::loopSpeedSimple(CMsg &msg){
    
  _Input =RPM();

  double diff=_Setpoint-_Input;

  _Output+=(diff/4.0);

  
  if(_Output>4000.0) _Output=4000.0;
  if(_Output<-4000.0) _Output=-4000.0;

 activateDrive((int)_Output);
  
}


void CMotorController::loopLock(CMsg &msg){
  if(_pIMU==NULL) {writeconsoleln("NO IMU!!!!!!!!!!!!!!!!!!!!!");return;}
  writeconsoleln("!!!!!!!!!!!!!!!!!!!!!  LOOOP LOCK   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  
  _pIMU->Run();
  std::string axis="GYRO_";
  axis+=_axis;

  writeconsole("Axis: "); writeconsoleln(axis);
  CMsg m=getDataMap("GYRO");
  m.writetoconsole();

  _Input=m.getParameter(axis,0.0);  

  double diff=_Setpoint-_Input;
  _Output=1000.0*diff;

 activateDrive((int)_Output);
  
}


void CMotorController::loop(){  
    CMsg m;
    runOnce(m);  
  }

void CMotorController::loopRotation(CMsg &msg){

  if(_pIMU==NULL) {writeconsoleln("NO IMU!!!!!!!!!!!!!!!!!!!!!");return;}
  
  _pIMU->Run();
  std::string axis="GYRO_";
  axis+=_axis;

  writeconsoleln(axis);
  CMsg m=getDataMap("GYRO");

  m.writetoconsole();

  _Input=m.getParameter(axis,0);  
  _Output_last=_Output;
  myPID.compute();
 
  activateDrive((int)_Output);  
} 


void CMotorController::loopRamp(CMsg &msg){
  if(_Setpoint>PWM_MAX) _Setpoint=0;
  _Setpoint+=20;
  activateDrive(_Setpoint);  
  
}



void CMotorController::configRotation(CMsg &msg){  
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
  
  myPID.setBias(PWM_MAX / 2.0);  // was 2.0
  myPID.setWindUpLimits(-10000000000, 1000000000); // Groth bounds for the integral term to prevent integral wind-up
  
  //myPID.SetSampleTime(50); //commented out in code
  
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)     
    myPID.setOutputLimits(10, PWM_MAX);   
  #else          
    myPID.setOutputLimits(10, PWM_MAX);
  #endif
  myPID.start();
  Speed(0,10000);
  setState("PLAY");

}

void CMotorController::configSpeed(CMsg &msg){      
  _Kp=32.0, _Ki=0.10, _Kd=0.0;
  myPID.begin(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd);

  // myController.reverse()               // Uncomment if controller output is "reversed"
  // myController.setSampleTime(10);      // OPTIONAL - will ensure at least 10ms have past between successful compute() calls
  
  myPID.setBias(PWM_MAX / 2.0);  // was 2.0
  myPID.setWindUpLimits(-10000000000, 1000000000); // Groth bounds for the integral term to prevent integral wind-up
  
  //myPID.SetSampleTime(50); //commented out in code
  
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)     
    myPID.setOutputLimits(10, PWM_MAX);   
  #else          
    myPID.setOutputLimits(10, PWM_MAX);
  #endif
  myPID.start();
  Speed(0,10000);

  // myController.reset();               // Used for resetting the I and D terms - only use this if you know what you're doing
  // myController.stop();                // Turn off the PID controller (compute() will not do anything until start() is called)
  setState("PLAY");
  
}


void CMotorController::test(CMsg &msg) {
init();

std::list<double> flist; 
int sspeed=0;  

std::string testMode=msg.getParameter("MODE","SIMPLE");
testMode="LOCK";
setMode(testMode);  
initMode(msg);
  
setPoint(0.0 );
Run(20000);  

//off();  
}



void CMotorController::runOnce(CMsg &msg){  
  if(Mode()=="LOCK")
    loopLock(msg);
  else if(Mode()=="SPEED")
    loopSpeed(msg);
  else if(Mode()=="SIMPLE")
    loopSpeedSimple(msg);
  else if(Mode()=="ROTATION")  
    loopRotation(msg);  
  else if(Mode()=="RAMP")  
    loopRamp(msg);  
  else if(Mode()=="PWM")  
    loopPWM(msg);  
  else
    loopSpeedSimple(msg);
}


void CMotorController::initMode(CMsg &msg){  
  writeconsole("Init Mode..");writeconsoleln(Mode());
  if(Mode()=="LOCK")
    configLock(msg);
  else if(Mode()=="SPEED")
    configSpeed(msg);
  else if(Mode()=="SIMPLE")
    configSpeedSimple(msg);
  else if(Mode()=="ROTATION")  
    configRotation(msg);  
  else if(Mode()=="RAMP")  
    configRamp(msg);  
  else if(Mode()=="PWM")  
    configPWM(msg);  
  else
    configSpeedSimple(msg);
}


void CMotorController::configLock(CMsg &msg){
  CIMU *pTmp=(CIMU *)getSystem("IMUI2C","IMUI2C");
  if(pTmp==NULL)
    return;
  setIMU(pTmp);
  pTmp->setMode("GYRO"); 
  setPoint(0.0);
  setState("PLAY");
  }

void CMotorController::configSpeedSimple(CMsg &msg){
  setPoint(-1630.0);
  _Output=500.0;
  setState("PLAY");
  }

void CMotorController::configRamp(CMsg &msg){setState("PLAY");}

void CMotorController::configPWM(CMsg &msg){setState("PLAY");}
