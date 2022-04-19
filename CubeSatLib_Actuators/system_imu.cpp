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
  if(subscribers(Name())){
    CMsg m;
    runOnce(m);
    }
  }


void CIMU::test(CMsg &msg){ 
  CMsg m;

  unsigned long tt=getTime()+5000;
  
  while(getTime()<tt){    
    runOnce(m);    
    m=Gyro.makeMessage("GYRO");
  
    m.writetoconsole();      
    delay(100);    
  }
  
}


void CIMU::runOnce(CMsg &m){
     if((State()!="READY")&&(State()!="PLAY") )setup();
    if((State()!="READY")&&(State()!="PLAY") ) return;
    if (myIMU.dataAvailable() == true) {
      GetData();      
      writeconsoleln("New IMU Data");
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
      
      *PRY.pPitch=(myIMU.getPitch()) * 180.0 / PI; // Convert pitch to degrees
      *PRY.pRoll=(myIMU.getRoll()) * 180.0 / PI; // Convert roll to degrees
      *PRY.pYaw=(myIMU.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees

      //Quat.I=myIMU.getQuatI();       
      //Quat.J = myIMU.getQuatJ();
      //Quat.K = myIMU.getQuatK();
      //Quat.R = myIMU.getQuatReal();
      //Quat.Acc = myIMU.getQuatRadianAccuracy();

      float quatRadianAccuracy;

       

      //Lin.X = myIMU.getLinAccelX();
      //Lin.Y = myIMU.getLinAccelY();
      //Lin.Z = myIMU.getLinAccelZ();
      //Lin.Acc = myIMU.getLinAccelAccuracy();

      myIMU.getLinAccel(Lin._fX , Lin._fY, Lin._fZ, Lin.Acc);
      myIMU.getGyro(Gyro._fX , Gyro._fY, Gyro._fZ, Gyro.Acc);
      myIMU.getMag(Mag._fX , Mag._fY, Mag._fZ, Mag.Acc);
      myIMU.getQuat(Quat._fX, Quat._fY, Quat._fZ, Quat._fR, quatRadianAccuracy, Quat.Acc);

  //  gyrox = myIMU.getGyroX();
  //  gyroy = myIMU.getGyroY();
  //  Gyro.z = myIMU.getGyroZ();
    //  Gyro.X = myIMU.getFastGyroX();
    //  Gyro.Y = myIMU.getFastGyroY();
    //  Gyro.Z = myIMU.getFastGyroZ();

    //  Mag.X = myIMU.getMagX();
    //  Mag.Y = myIMU.getMagY();
    //  Mag.Z = myIMU.getMagZ();
    //  Mag.Acc = myIMU.getMagAccuracy();

      Mag._changedOn=getTime();
      Quat._changedOn=getTime();
      Lin._changedOn=getTime();
      PRY._changedOn=getTime();
      Gyro._changedOn=getTime();

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

void CIMU::config(const char * option, int period){              //IMPORTANT
  /*
  myIMU.enableLinearAccelerometer(50);  // m/s^2 no gravity
  myIMU.enableRotationVector(50);  // quat
  myIMU.enableGyro(50);  // rad/s
  //myIMU.enableMagnetometer(50);  // cannot be enabled at the same time as RotationVector (will not produce data)
  */

  if(option=="ROT") myIMU.enableRotationVector(period); //Quaternion Send data update every 50ms
  if(option=="GYRO") myIMU.enableGyro(period); // rad/s
  if(option=="GYROROT") myIMU.enableGyroIntegratedRotationVector(period); //Quaternion Send data update every 50ms
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
     config("GYRO");
      
     setState("PLAY");
     writeconsole("IMU Ready");
     return;
    }
  }
#endif  
}


void CIMU::setupI2C(char imuaddress){
  Name("IMU");
  delay(100);
  Wire.flush();

for(int retries=0;retries<5;retries++){
  
  if (myIMU.begin(imuaddress) == false) {   //IMUADDRESS2
     unsigned int counter=10000;
     while(Wire.available()&&counter) {  Wire.read();counter--;}   //Flushes wire.  Then try again    Wire.flush() should do the same thing
     delay(100);              
     setState("ERROR");
    writeconsoleln("Setup IMU Error...............................");
    }
  else{  
    Wire.setClock(400000);
    //  myIMU.enableRotationVector(50); //Send data update every 50ms
    
    config("GYRO");
    myIMU.enableGyro(50); //Send data update every 50ms
    setState("PLAY");
    writeconsoleln("IMU Ready...............................");
    
   return ;
  }
}
}
