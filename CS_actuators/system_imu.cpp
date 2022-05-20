#include "system_imu.h"

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
  std::string mode=msg.getParameter("DATAMODE","GYROROT");
  int period=msg.getParameter("PERIOD",2000);
  _dataMode=mode;
  setState("");
  Run(period);
    
  CMsg m=getDataMap(mode);
  m.writetoconsole();
  addTransmitList(m);    
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
  //return false;
}

void CIMU::GetData(){
    //Look for reports from the IMU    
  //  writeconsoleln(" --------------------------------------------------------------------------------- GET IMU DATA ---------------------------------------------------------------------------");

  _dataUpdatedOn=getTime();
  PRY.archiveData();
  Quat.archiveData();
  Lin.archiveData();
  Gyro.archiveData();
  Mag.archiveData();
  
  *PRY.pPitch=(myIMU.getPitch()) * 180.0 / PI; // Convert pitch to degrees
  *PRY.pRoll=(myIMU.getRoll()) * 180.0 / PI; // Convert roll to degrees
  *PRY.pYaw=(myIMU.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees


  float quatRadianAccuracy;

  

  Mag._changedOn=getTime();
  Quat._changedOn=getTime();
  Lin._changedOn=getTime();
  PRY._changedOn=getTime();  //Pitch, Roll Yaw
  Gyro._changedOn=getTime();

  CMsg mGyro,mLin,mMag,mPRY, mQuat;


  myIMU.getGyro(Gyro._fX , Gyro._fY, Gyro._fZ, Gyro.Acc);
  mGyro=Gyro.makeMessage("GYRO");
  addDataMap(std::string("GYRO"),mGyro);


  myIMU.getLinAccel(Lin._fX , Lin._fY, Lin._fZ, Lin.Acc);
  mLin=Lin.makeMessage("LIN");
  addDataMap(std::string("LIN"),mLin);

  myIMU.getMag(Mag._fX , Mag._fY, Mag._fZ, Mag.Acc);
  mMag=Mag.makeMessage("MAG");
  addDataMap(std::string("MAG"),mMag);

  myIMU.getQuat(Quat._fX, Quat._fY, Quat._fZ, Quat._fR, quatRadianAccuracy, Quat.Acc);
  mQuat=Quat.makeMessage("QUAT");
  addDataMap(std::string("QUAT"),mQuat);

  mPRY=PRY.makeMessage("PRY");
  addDataMap(std::string("PRY"),mPRY);

      
}    

  
void CIMU::init(){  
  setErrorThreshold(5);
  setInterval(5);
  setForever();
}


void CIMU::setup(){
  _dataUpdatedOn=getTime();
  if(Name()=="IMUSPI") setupSPI();
  if(Name()=="IMUI2C") setupI2C();
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

void CIMU::dataMode(std::string option, int period){              //IMPORTANT
  /*
  myIMU.enableLinearAccelerometer(50);  // m/s^2 no gravity
  myIMU.enableRotationVector(50);  // quat
  myIMU.enableGyro(50);  // rad/s
  //myIMU.enableMagnetometer(50);  // cannot be enabled at the same time as RotationVector (will not produce data)
  */

  writeconsoleln(option);
  _dataMode=option;
  if(option==std::string("QUAT")) myIMU.enableRotationVector(period); //Quaternion Send data update every 50ms
  if(option==std::string("GYRO"))  myIMU.enableGyro(period); // rad/s  
  if(option==std::string("GYROROT")) myIMU.enableGyroIntegratedRotationVector(period); //Quaternion Send data update every 50ms
  if(option==std::string("MAG")) myIMU.enableMagnetometer(period);
  if(option==std::string("ACCEL")) myIMU.enableAccelerometer(period);
  if(option==std::string("LINACCEL")) myIMU.enableLinearAccelerometer(period);
}

void CIMU::setupSPI(){
#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
   delay(100);
   writeconsoleln("BNO080 SPI Setup");
 
for(int retries=0;retries<5;retries++){
    if(myIMU.beginSPI(IMU_OBC_NSS, IMU_OBC_WAKE, IMU_OBC_INT, IMU_OBC_RST) == false)
    {
      if(incErrorCount()){
       sendError();
      }     
    }
    else{ 
     dataMode(_dataMode);
      
     setState("PLAY");
     writeconsole("IMU Ready");
     return;
    }
  }
#endif  
}


void CIMU::setupI2C(){
  delay(100);
  if(_pWire==NULL)
    config(_address);
  _pWire->flush();

  for(int retries=0;retries<5;retries++){
    
    if (myIMU.begin(_address) == false) {   
      unsigned int counter=10000;
      while(_pWire->available()&&counter) {  _pWire->read();counter--;}   //Flushes wire.  Then try again    Wire.flush() should do the same thing
        
      if(incErrorCount())
        sendError();        
      }
    else{ 
      dataMode(_dataMode);
        
      setState("PLAY");
      writeconsole("IMU Ready");
      return;
      }
    }
  }
