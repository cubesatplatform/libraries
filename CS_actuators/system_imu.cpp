#include "system_imu.h"


#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  #include "pinDefinitions.h"
#endif

#define IMU_WAIT_TIME 10000


void CIData::init(){  
 _lastTime=0;
 _changedOn=0;

 _lastX=0.0;
 _lastY=0.0;
 _lastZ=0.0;
 _lastR=0.0;
 _lastAcc=0.0;
  
 _fX=0.0;
 _fY=0.0;
 _fZ=0.0;
 _fR=0.0;
 Acc=0.0;
}


void CIData::remap(float * pnewX, float *pnewY, float *pnewZ){
  pX=pnewX;
  pY=pnewY;
  pZ=pnewZ;
}


void CIMU::loop(){  
    CMsg m;
    runOnce(m);  
  }


void CIMU::test(CMsg &msg){ 

  myIMU.enableRotationVector(50); //Send data update every 50ms

  Serial.println(F("Rotation vector enabled"));
  Serial.println(F("Output in form i, j, k, real, accuracy"));

  //Look for reports from the IMU
  long t=getTime();

  while(getTime()<t+1500){
    if (myIMU.dataAvailable() == true)
    {    
      float roll = (myIMU.getRoll()) * 180.0 / PI; // Convert roll to degrees
      float pitch = (myIMU.getPitch()) * 180.0 / PI; // Convert pitch to degrees
      float yaw = (myIMU.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees

      Serial.print(roll, 1);
      Serial.print(F(","));
      Serial.print(pitch, 1);
      Serial.print(F(","));
      Serial.print(yaw, 1);

      Serial.println();
    }    
  }
}
  



void CIMU::runOnce(CMsg &m){
  if (myIMU.dataAvailable() == true) {    
    GetData();          
    }
  else {
    if(getTime()>(_dataUpdatedOn+IMU_WAIT_TIME)){  //Check if nothing for 100s        
        writeconsole("Wait Time  NO Data  Resetting State");
        _dataUpdatedOn=getTime();
        setState(_BLANK);
    }
  }
}


void CIMU::GetData(){
  
  _dataUpdatedOn=getTime();
  bool newData=false;

  float quatRadianAccuracy;  
 
  CMsg m;

  _Data.archiveData();
  _Data._changedOn=getTime();
  newData=true;

  std::string key=name();
  key+=Mode();

  

  if((Mode()==_GYRO)||(Mode()==_BLANK)){    
    myIMU.getGyro(_Data._fX , _Data._fY, _Data._fZ, _Data.Acc);    
    m=_Data.makeMessage(key);
    
    
  }

  if(Mode()==_LIN){
    myIMU.getLinAccel(_Data._fX , _Data._fY, _Data._fZ, _Data.Acc);    
    m=_Data.makeMessage(key);
    
  }

  if(Mode()==_ACCEL){    
    myIMU.getAccel(_Data._fX , _Data._fY, _Data._fZ, _Data.Acc);
    m=_Data.makeMessage(key);
    
  }

  if(Mode()==_MAG){
    myIMU.getMag(_Data._fX , _Data._fY, _Data._fZ, _Data.Acc);  
    m=_Data.makeMessage(key);
    
  }


  if(Mode()==_QUAT){    
    myIMU.getQuat(_Data._fX, _Data._fY, _Data._fZ, _Data._fR, quatRadianAccuracy, _Data.Acc);
    m=_Data.makeMessage(key);

  }

  if(Mode()==_PRY){
    *_Data.pPitch=(myIMU.getPitch()) * 180.0 / PI; // Convert pitch to degrees
    *_Data.pRoll=(myIMU.getRoll()) * 180.0 / PI; // Convert roll to degrees
    *_Data.pYaw=(myIMU.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees
    m=_Data.makeMessage(key);
  }

  m.writetoconsole();
  addDataMap(m);


  if(newData){
    _newDataCounter++;
    if(_newDataCounter%150==0)  {  writeconsoleln("New IMU Data");  _newDataCounter=0;}
   
  }
}    

  
void CIMU::init(){    

  setForever();
}


void CIMU::setup(){
  _dataUpdatedOn=getTime();
  
  if(name()==_IMUS) setupSPI();  
  else setupI2C();
  
  if(state()==_PLAY){  
    setPeriod();
  
  }
}


void CIMU::config(char addr, TwoWire *twowire){
  _address=addr;
  _pWire=twowire;  
  setForever();
  setInterval(10);  
  setErrorThreshold(100);
  setModifiedTime(getTime());
  }


void CIMU::config(CMsg &msg){
  std::string straddress=msg.get(_ADDRESS);
  std::string strwire=msg.get(_WIRE);

  TwoWire *pWire;

  if(strwire==_WIRE) pWire=&Wire;  
  if(strwire==_WIRE1) pWire=&Wire1;  
  if(strwire==_WIRE2) pWire=&Wire2;  

  if(straddress.size()>0){
    config(straddress[0],pWire);
  }
}


void CIMU::goMag(){
  _m.set(_MODE,_MAG);
  setPeriod(50);
  run();
}



void CIMU::goGyro(){
  _m.set(_MODE,_GYRO);
  setPeriod(30);
  run();
}


void CIMU::goPRY(){
  _m.set(_MODE,_PRY);
  setPeriod(30);
  run();
}


void CIMU::goQuat(){
_m.set(_MODE,_QUAT);
  setPeriod(30);
  run();
}

void CIMU::goAccel(){
  _m.set(_MODE,_ACCEL);
  setPeriod(30);
  run();
}


void CIMU::newMode(CMsg &msg){
  msg.writetoconsole();
  std::string mode=msg.get(_VALUE);
  _m.set(_MODE,mode);  
  setPeriod(50);
}

void CIMU::setPeriod(int period){              //IMPORTANT
  /*
  myIMU.enableLinearAccelerometer(30);  // m/s^2 no gravity
  myIMU.enableRotationVector(30);  // quat
  myIMU.enableGyro(30);  // rad/s  
  */  

  if (state()!=_PLAY){
    run(200);
    if (state()!=_PLAY) return;
  }
  
  if(Mode()==_PRY) {
    myIMU.enableRotationVector(period); //Euler & Quaternion Send data update every 50ms
    writeconsoleln("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww          Set Period  "+Mode());
  }
  if(Mode()==_QUAT) myIMU.enableRotationVector(period); //Euler & Quaternion Send data update every X ms
  if(Mode()==_GYRO)  myIMU.enableGyro(period); // rad/s  
  if(Mode()==_GYROROT) myIMU.enableGyroIntegratedRotationVector(period); //Quaternion Send data update every X ms
  if(Mode()==_MAG) myIMU.enableMagnetometer(period);   // cannot be enabled at the same time as RotationVector (will not produce data)
  if(Mode()==_ACCEL) myIMU.enableAccelerometer(period);
  if(Mode()==_LIN) myIMU.enableLinearAccelerometer(period);
}

void CIMU::setupSPI(){
#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
   delay(100);
   writeconsoleln("BNO080 SPI Setup");
  setState(_ERROR);
  for(int retries=0;retries<5;retries++){
      if (myIMU.beginSPI(PinNameToIndex(PJ_8), PinNameToIndex(PJ_9), PinNameToIndex(PI_15), PinNameToIndex(PI_14))  == false)  //This worked in demo program
      {
        incErrorCount();             
      }
      else{       
        setState(_PLAY);
        writeconsole("IMU Ready");
        return;
      }
    }
#endif 
sendError(); 
}


void CIMU::setupI2C(){
  //enableMBLogic();
  delay(100);
  if(_pWire==NULL)
    config(_address);
  _pWire->flush();

  writeconsoleln("BNO080 I2C Setup");
  setState(_ERROR);
  for(int retries=0;retries<5;retries++){
    
    if (myIMU.begin(_address) == false) {   
      unsigned int counter=10000;
      while(_pWire->available()&&counter) {  _pWire->read();counter--;}   //Flushes wire.  Then try again    Wire.flush() should do the same thing        
      incErrorCount();    
    }  
    else{             
      setState(_PLAY);
      writeconsoleln("IMU Ready   !!!");
      return;
      }
    }
    sendError(); 
  }




void CIMU::callCustomFunctions(CMsg &msg){
  std::string act=msg.get(_ACT);
  int val=msg.get(_VALUE,30);
  writeconsole("CIMU::callCustomFunctions newMsg receieved by ");writeconsole(name()); writeconsole("  ");writeconsoleln(act);
  if(act==_SETPERIOD) {setPeriod(val);writeconsoleln("Period Set");return;}


  mapcustom(goMag)
  mapcustom(goGyro)
  mapcustom(goPRY)
  mapcustom(goAccel)
  mapcustom(goQuat)

  mapcustom(setupI2C)
  mapcustom(setupSPI)

  CSystemObject::callCustomFunctions(msg);
}