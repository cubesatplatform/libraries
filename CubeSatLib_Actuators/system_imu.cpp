#include "system_imu.h"



#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
#include <pinDefinitions.h>

//#define IMU_OBC_NSS PinNameToIndex(PB_4)
//#define IMU_OBC_RST PinNameToIndex(PI_14)
//#define IMU_OBC_INT PinNameToIndex(PI_15)
//#define IMU_OBC_WAKE PinNameToIndex(PB_3)

#define IMU_OBC_NSS PinNameToIndex(PJ_8)
#define IMU_OBC_RST PinNameToIndex(PI_14)
#define IMU_OBC_INT PinNameToIndex(PI_15)
#define IMU_OBC_WAKE PinNameToIndex(PJ_9)
#endif


void CIData::init(){
 prevTime=0;
 changedOn=0;

 prevX=0.0;
 prevY=0.0;
 prevZ=0.0;
 prevR=0.0;
 prevAcc=0.0;
  
 X=0.0;
 Y=0.0;
 Z=0.0;
 R=0.0;
 Acc=0.0;

  }

void CIMU::loop(){
  if(subscribers(Name())){
  CMsg m;
  runOnce(m);
    }
  }


void CIMU::test(CMsg &msg){ 
  CMsg m;
  runOnce(m);
  Output(m);
 
}


void CIMU::runOnce(CMsg &m){
     if((State()!="READY")&&(State()!="PLAY") )setup();
    if((State()!="READY")&&(State()!="PLAY") ) return;
    if (myIMU.dataAvailable() == true) {
      GetData();      
      //return true;
      }
    else {
      if(getTime()>(_dataUpdatedOn+IMU_WAIT_TIME)){  //Check if nothing for 100s
        setState("");  //Reset IMU
        writeconsoleln("==========================  IMU RESET NOT HEARD IN 100 SECS ==============================");
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
      
      PRY.Pitch=(myIMU.getPitch()) * 180.0 / PI; // Convert pitch to degrees
      PRY.Roll=(myIMU.getRoll()) * 180.0 / PI; // Convert roll to degrees
      PRY.Yaw=(myIMU.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees

      Quat.I=myIMU.getQuatI();
       
      Quat.J = myIMU.getQuatJ();
      Quat.K = myIMU.getQuatK();
      Quat.R = myIMU.getQuatReal();
      Quat.Acc = myIMU.getQuatRadianAccuracy();

      Lin.X = myIMU.getLinAccelX();
      Lin.Y = myIMU.getLinAccelY();
      Lin.Z = myIMU.getLinAccelZ();
      Lin.Acc = myIMU.getLinAccelAccuracy();



  //  gyrox = myIMU.getGyroX();
  //  gyroy = myIMU.getGyroY();
  //  Gyro.z = myIMU.getGyroZ();
      Gyro.X = myIMU.getFastGyroX();
      Gyro.Y = myIMU.getFastGyroY();
      Gyro.Z = myIMU.getFastGyroZ();

      Mag.X = myIMU.getMagX();
      Mag.Y = myIMU.getMagY();
      Mag.Z = myIMU.getMagZ();
      Mag.Acc = myIMU.getMagAccuracy();

      Mag.changedOn=getTime();
      Quat.changedOn=getTime();
      Lin.changedOn=getTime();
      PRY.changedOn=getTime();
      Gyro.changedOn=getTime();

      CMsg mGyro,mLin,mMag,mPRY;

      if (Gyro.anythingNew()){
        mGyro=Gyro.makeMessage("GYRO");
        addDataList(mGyro);
      }

      
      if (Lin.anythingNew()){
        mLin=Lin.makeMessage("LIN");
        addDataList(mLin);
      }

      if (Mag.anythingNew()){
        mMag=Mag.makeMessage("Mag");
        addDataList(mMag);
      }

      
      if (PRY.anythingNew()){
        mPRY=PRY.makeMessage("PRY");
        addDataList(mPRY);
      }

      //mGyro.appendParams(mLin.Parameters);   
      //mGyro.appendParams(mMag.Parameters);
      //mGyro.appendParams(mPRY.Parameters);
      

      //if(mGyro.Parameters.size())
        //respondCallBack(mGyro);
      
}    

 void CIMU::Output(CMsg &msg){  
  CMsg mGyro;
  mGyro=Gyro.makeMessage("GYRO");
  addTransmitList(mGyro);

}

  
void CIMU::init(){  
  setInterval(5);
  setForever();
}


void CIMU::setup(){
  if(Name()=="IMUSPI") setupSPI();
  if(Name()=="IMUI2C") setupI2C();
}

void CIMU::config(std::string option, int period){
  if(option=="ROT") myIMU.enableRotationVector(period); //Send data update every 50ms
  if(option=="GYRO") myIMU.enableGyro(period); //Send data update every 50ms
  if(option=="GYROROT") myIMU.enableGyroIntegratedRotationVector(period); //Send data update every 50ms
  if(option=="MAG") myIMU.enableMagnetometer(period);
  if(option=="ACCEL") myIMU.enableAccelerometer(period);
  if(option=="LINACCEL") myIMU.enableLinearAccelerometer(period);
}

void CIMU::setupSPI(){
#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
//   myIMU.enableDebugging(Serial); //Pipe debug messages to Serial port
   delay(100);

   
for(int retries=0;retries<5;retries++){
    if(myIMU.beginSPI(IMU_OBC_NSS, IMU_OBC_WAKE, IMU_OBC_INT, IMU_OBC_RST) == false)
    {
      writeconsoleln("BNO080 over SPI not detected. Are you sure you have all 6 connections? Freezing...");
      delay(100); 
      setState("ERROR");
     
    }
    else{ 
     //  myIMU.enableRotationVector(50); //Send data update every 50ms
     //   myIMU.enableGyro(50); //Send data update every 50ms
     myIMU.enableGyroIntegratedRotationVector(50); //Send data update every 50ms
  
  
     setState("PLAY");
     writeconsole("IMU Ready");
     return;
    }
  }
#endif  
}


void CIMU::setupI2C(){
  Name("IMU");
  delay(100);
  Wire.flush();

for(int retries=0;retries<5;retries++){
  
  if (myIMU.begin(IMUADDRESS1) == false) {   //IMUADDRESS2
     unsigned int counter=10000;
     while(Wire.available()&&counter) {  Wire.read();counter--;}   //Flushes wire.  Then try again    Wire.flush() should do the same thing
     delay(100);              
     setState("ERROR");
    writeconsoleln("Setup IMU Error...............................");
    }
  else{  
    Wire.setClock(400000);
    //  myIMU.enableRotationVector(50); //Send data update every 50ms
    //  myIMU.enableGyro(50); //Send data update every 50ms
    myIMU.enableGyroIntegratedRotationVector(50); //Send data update every 50ms
    
    
    setState("PLAY");
    writeconsoleln("IMU Ready...............................");
    
   return ;
  }
}
}
