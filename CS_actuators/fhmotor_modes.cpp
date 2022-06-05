#include "fhmotor.h"


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



void CMotorController::loopRotation(CMsg &msg){
  
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



void CMotorController::configLock(CMsg &msg){ 
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








/////////////////

void CMotorController::test(CMsg &msg) {
init();

std::list<double> flist; 
int sspeed=0;  

std::string testMode=msg.getParameter("MODE","SIMPLE");
testMode="LOCK";
setMode(testMode);  
initMode(msg);

  
CIMU *pIMU=(CIMU *)getIMU();
if(pIMU==NULL)
  return;

  
setPoint(0.0 );

unsigned long ct=getTime();

while(getTime()<ct+20000){
  pIMU->Run();
  Run();  
}

//off();  
}