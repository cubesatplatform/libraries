#include <systemdefs.h>
#include "fhmotor.h"


// Z axis is flat


void CMotorController::newMode(CMsg &msg){
  writeconsoleln("NEW MODE");

  std::string mode=msg.get(_VALUE);
  _m.set(_MODE,mode);  
  setup();
  
  _Output_duration=msg.get(_DURATION,0);
  if(_Output_duration>0){
    _Setpoint_last=_Setpoint;
    _Output_duration+=getTime();
  }
  
  _Setpoint=msg.get(_SETPOINT,_Setpoint);
  msg.writetoconsole();
  
}




void CMotorController::runOnce(CMsg &msg){  
  
  if(Mode()==_LOCK)
    loopLock();
  else if(Mode()==_SPEED)
    loopSpeed();
  else if(Mode()==_SIMPLE)
    loopSpeedSimple();
  else if(Mode()==_POSITION)  
    loopPosition();  
  else if(Mode()==_POSITION)  
    manual();  
 
  else
    loopSpeedSimple();
  
}


void CMotorController::sendPlotter(CMsg &msg){  
  int send=_m.get(_SEND,1);  
  

  if(send>0){

    long now=getTime();
    long interval=_m.get(_INTERVAL,100);
    long last=_m.get(_LAST,0);


    if((now-last)<interval)
      return;

    _m.set(_LAST,now);
    
    msg.set(_TO,"PLOTTER");
    msg.set(_SYS,"PLOTTER");
   
    addMessageList(msg);
  }
}





void CMotorController::loopSpeed(){
  _Input =getRPS();
  myPID.compute(&Serial);

   myPID.debug(&Serial, name().c_str(), PRINT_INPUT    | // Can include or comment out any of these terms to print
                                        PRINT_OUTPUT   | // in the Serial plotter
                                        PRINT_SETPOINT |
                                        PRINT_BIAS     |
                                        PRINT_P        |
                                        PRINT_I        |
                                        PRINT_D);

  
  speed(_Output);    
}



void CMotorController::loopSpeedSimple(){    
  _Input =getRPS();

  writeconsole("Input: "); writeconsole((int)_Input); writeconsole("              Output: "); writeconsoleln(getPWMSpeed());   
  
}


void CMotorController::loopLock(){
  
  writeconsole("Lock  ");
  _Input=getIMUValue(_GYRO,_axis);

  _x=getIMUValue(_GYRO,_XAXIS);
  _y=getIMUValue(_GYRO,_YAXIS);
  _z=getIMUValue(_GYRO,_ZAXIS);
  _t=getIMUValue(_GYRO,_TIME);
  
  _Output_last=_Output;
  _mLog=myPID.compute(&Serial);
  
  myPID.debug(&Serial, name().c_str(), PRINT_INPUT    | // Can include or comment out any of these terms to print
                                        PRINT_OUTPUT   | // in the Serial plotter
                                        PRINT_SETPOINT |
                                        PRINT_BIAS     |
                                        PRINT_P        |
                                        PRINT_I        |
                                        PRINT_D);

  speed(_Output);  

}





void CMotorController::loopPosition(){  
  _Input=getIMUValue(_PRY,_axis);
  

  _Input_last=_Input;

  _x=getIMUValue(_PRY,_XAXIS);
  _y=getIMUValue(_PRY,_YAXIS);
  _z=getIMUValue(_PRY,_ZAXIS);
  _t=getIMUValue(_PRY,_TIME);

  _Output_last=_Output;
  _mLog=myPID.compute(&Serial);
  /*
  myPID.debug(&Serial, name().c_str(), PRINT_INPUT    | // Can include or comment out any of these terms to print
                                        PRINT_OUTPUT   | // in the Serial plotter
                                        PRINT_SETPOINT |
                                        PRINT_BIAS     |
                                        PRINT_P        |
                                        PRINT_I        |
                                        PRINT_D);
  */
 echoOn();
 _mLog.writetoconsoleln();
 echoOff();

 // sendPlotter(m);

  speed(_Output);  //Stay below 50
  
} 




void CMotorController::setupPosition(){  
  
  _Kp=0.25, _Ki=0.02, _Kd=0.0;
  //setPoint(90.0);
  myPID.begin(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd);

  // myController.reverse()               // Uncomment if controller output is "reversed"
  // myController.setSampleTime(10);      // OPTIONAL - will ensure at least 10ms have past between successful compute() calls
  

  //Bias is the PID output when the error is zero, and is filtered by the reset time whose best setting is reduced to be about the deadtime.
  //myPID.setBias(PWM_MIN / 2.0);  // was 2.0
  myPID.setBias(0.0);  // was 2.0

  //Integral windup is the process of accumulating the integral component beyond the saturation limits of final control element. 
  myPID.setWindUpLimits(-350, 350); // Growth bounds for the integral term to prevent integral wind-up
        
  myPID.setOutputLimits(-50.0, 50.0);

  speed(0.0);
  setState(_PLAY);

}

void CMotorController::setupSpeed(){      
  _Kp=0.001, _Ki=0.00001, _Kd=0.0;
  myPID.begin(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd);


  myPID.setBias(0.0);  // was 2.0
  myPID.setWindUpLimits(-150, 150); // Growth bounds for the integral term to prevent integral wind-up
  
        
  myPID.setOutputLimits(-100.0, 100.0);

  
  speed(0.0);

  setState(_PLAY);
}



void CMotorController::setupLock(){ 
  _Kp=5.0, _Ki=1.00, _Kd=0.0;
  myPID.begin(&_Input, &_Output, &_Setpoint, _Kp, _Ki, _Kd);


  myPID.setBias(0.0);  // was 2.0
  myPID.setWindUpLimits(-150, 150); // Growth bounds for the integral term to prevent integral wind-up
    
    
  myPID.setOutputLimits(-100.0, 100.0);
  
  setPoint(0.0);
  
  setState(_PLAY);
  }

void CMotorController::setupSpeedSimple(){
  setPoint(-1630.0);
  _Output=500.0;
  setState(_PLAY);
  }


