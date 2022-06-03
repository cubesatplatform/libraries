#include "system_imu.h"
#include <powerup.h>

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
#include "pinDefinitions.h"

//#define IMU_OBC_NSS PinNameToIndex(PB_4)
//#define IMU_OBC_RST PinNameToIndex(PI_14)
//#define IMU_OBC_INT PinNameToIndex(PI_15)
//#define IMU_OBC_WAKE PinNameToIndex(PB_3)

#define IMU_OBC_NSS PinNameToIndex(PJ_8)
#define IMU_OBC_WAKE PinNameToIndex(PJ_9)
#define IMU_OBC_INT PinNameToIndex(PI_15)
#define IMU_OBC_RST PinNameToIndex(PI_14)
#endif

#define IMU_WAIT_TIME 100000


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
  std::string mode=msg.getParameter("MODE","ROT");  
  int period=msg.getParameter("PERIOD",3000);
  setMode(mode);    
  Run(period);  
  
  if (State()!="ERROR"){  
    if(Mode()=="ROT")
      mode="PRY";
    CMsg m=getDataMap(mode);
    m.writetoconsole();
    addTransmitList(m);   
  } 
  disableMBLogic();
}


void CIMU::runOnce(CMsg &m){
  if (myIMU.dataAvailable() == true) {
    GetData();      
    writeconsoleln("New IMU Data");
    }
  else {
    if(getTime()>(_dataUpdatedOn+IMU_WAIT_TIME)){  //Check if nothing for 100s
        incErrorCount();  //Reset IMU        
    }
  }
}

void CIMU::GetData(){
  _dataUpdatedOn=getTime();

  float quatRadianAccuracy;  
 
  CMsg mGyro,mLin,mMag,mPRY, mQuat,mAccel;

  if(Mode()=="GYRO"){
    Gyro.archiveData();
    myIMU.getGyro(Gyro._fX , Gyro._fY, Gyro._fZ, Gyro.Acc);
    Gyro._changedOn=getTime();
    mGyro=Gyro.makeMessage("GYRO");
    addDataMap(std::string("GYRO"),mGyro);
  }

  if(Mode()=="LIN"){
    Lin.archiveData();
    myIMU.getLinAccel(Lin._fX , Lin._fY, Lin._fZ, Lin.Acc);
    Lin._changedOn=getTime();
    mLin=Lin.makeMessage("LIN");
    addDataMap(std::string("LIN"),mLin);
  }

  if(Mode()=="ACCEL"){
    myIMU.getAccel(Lin._fX , Lin._fY, Lin._fZ, Lin.Acc);
    Accel._changedOn=getTime();
    mAccel=Accel.makeMessage("ACCEL");
    addDataMap(std::string("ACCEL"),mAccel);
  }

  if(Mode()=="MAG"){
    Mag.archiveData();
    myIMU.getMag(Mag._fX , Mag._fY, Mag._fZ, Mag.Acc);
    Mag._changedOn=getTime();
    mMag=Mag.makeMessage("MAG");
    addDataMap(std::string("MAG"),mMag);
  }


  if(Mode()=="ROT"){
    PRY.archiveData();
    Quat.archiveData();
    PRY._changedOn=getTime();  //Pitch, Roll Yaw
    Quat._changedOn=getTime();
    myIMU.getQuat(Quat._fX, Quat._fY, Quat._fZ, Quat._fR, quatRadianAccuracy, Quat.Acc);
    mQuat=Quat.makeMessage("QUAT");
    addDataMap(std::string("QUAT"),mQuat);


    *PRY.pPitch=(myIMU.getPitch()) * 180.0 / PI; // Convert pitch to degrees
    *PRY.pRoll=(myIMU.getRoll()) * 180.0 / PI; // Convert roll to degrees
    *PRY.pYaw=(myIMU.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees
    mPRY=PRY.makeMessage("PRY");
    addDataMap(std::string("PRY"),mPRY);      
  }
}    

  
void CIMU::init(){    
  setErrorThreshold(4);
  setInterval(5);
  setForever();
}


void CIMU::setup(){
  _dataUpdatedOn=getTime();
  
  if(Name()=="IMUSPI") setupSPI();
  
  if(Name()=="IMUI2C") setupI2C();
  
  if(State()=="PLAY"){  
    initMode();
  
  }
}


void CIMU::config(char addr, TwoWire *twowire){
  _address=addr;
  _pWire=twowire;  
  setForever();
  setInterval(5);  
  setModifiedTime(getTime());
  }


void CIMU::config(CMsg &msg){
  std::string straddress=msg.getParameter("ADDRESS");
  std::string strwire=msg.getParameter("WIRE");

  TwoWire *pWire;

  if(strwire=="Wire") pWire=&Wire;  
  if(strwire=="Wire1") pWire=&Wire1;  
  if(strwire=="Wire2") pWire=&Wire2;  

  if(straddress.size()>0){
    config(straddress[0],pWire);
  }
}

void CIMU::initMode(int period){              //IMPORTANT
  /*
  myIMU.enableLinearAccelerometer(50);  // m/s^2 no gravity
  myIMU.enableRotationVector(50);  // quat
  myIMU.enableGyro(50);  // rad/s  
  */  
  
  if(Mode()==std::string("ROT")) myIMU.enableRotationVector(period); //Euler & Quaternion Send data update every 50ms
  if(Mode()==std::string("GYRO"))  myIMU.enableGyro(period); // rad/s  
  if(Mode()==std::string("GYROROT")) myIMU.enableGyroIntegratedRotationVector(period); //Quaternion Send data update every 50ms
  if(Mode()==std::string("MAG")) myIMU.enableMagnetometer(period);   // cannot be enabled at the same time as RotationVector (will not produce data)
  if(Mode()==std::string("ACCEL")) myIMU.enableAccelerometer(period);
  if(Mode()==std::string("LIN")) myIMU.enableLinearAccelerometer(period);
}

void CIMU::setupSPI(){
#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
   delay(100);
   writeconsoleln("BNO080 SPI Setup");
 
for(int retries=0;retries<5;retries++){
    if(myIMU.beginSPI(IMU_OBC_NSS, IMU_OBC_WAKE, IMU_OBC_INT, IMU_OBC_RST) == false)
    {
      incErrorCount();             
    }
    else{       
      setState("PLAY");
      writeconsole("IMU Ready");
      return;
    }
  }
#endif 
sendError(); 
}


void CIMU::setupI2C(){
  enableMBLogic();
  delay(100);
  if(_pWire==NULL)
    config(_address);
  _pWire->flush();

  writeconsoleln("BNO080 I2C Setup");
  for(int retries=0;retries<5;retries++){
    
    if (myIMU.begin(_address) == false) {   
      unsigned int counter=10000;
      while(_pWire->available()&&counter) {  _pWire->read();counter--;}   //Flushes wire.  Then try again    Wire.flush() should do the same thing        
      incErrorCount();    
    }  
    else{             
      setState("PLAY");
      writeconsole("IMU Ready");
      return;
      }
    }
    sendError(); 
  }
