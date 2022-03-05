#include "system_mgr.h"
#include "messages.h"
#include "defs.h"
#include "powerup.h"
#include "system_imu.h"

void CSystemMgr::showTests() {
  initPins();  
  CMsg m;
  
  std::string tmpstr;
  int n;
  for (auto it : Pins) {    
    m.setParameter(it.first,it.second);    
  }
  
m.setParameter("RADIO","RADIO");
m.setParameter("MAG","MAG");
m.setParameter("MAGX","MAGX");
m.setParameter("MAGY","MAGY");
m.setParameter("MAGZ","MAGZ");
m.setParameter("PINSON","PINSON");
m.setParameter("PINSOFF","PINSOFF");
m.setParameter("PHONEON","PHONEON");
m.setParameter("PHONEOFF","PHONEOFF");
m.setParameter("BURN","BURN");
m.setParameter("IMUSPI","IMUSPI");
m.setParameter("IMU0","IMU0");
m.setParameter("IMU1","IMU1");
m.setParameter("IMU2","IMU2");
m.setParameter("I2C0","I2C0");
m.setParameter("I2C1","I2C1");
m.setParameter("I2C2","I2C2");
m.setParameter("H","H4");
m.setParameter("L","L4");
m.setParameter("W","PWM4");
m.setParameter("Magnets","MAGX, MAGY, MAGZ");
m.setParameter("ICUs","I2C0, I2C1, I2C2");
m.setParameter("IMUs","IMU0, IMU1, IMU2, IMUSPI");
m.setParameter("Pins","HPH_15, LPH_15, WPH_15   -  High,  Low,  PWM");
writeconsoleln(m.serializeout());
addTransmitList(m);
}




#ifdef TTGO
void CSystemMgr::initPins() {

  Pins["36"] = 36;
  Pins["39"] = 39;
  Pins["34"] = 34;
  Pins["35"] = 35;
  Pins["32"] = 32;
  Pins["33"] = 33;
  Pins["25"] = 25;
  Pins["14"] = 14;
  Pins["13"] = 13;
  Pins["2"] = 2;
  Pins["1"] = 1;
  Pins["3"] = 3;
  Pins["23"] = 23;
  Pins["4"] = 4;
  Pins["0"] = 0;
  Pins["22"] = 22;
  Pins["21"] = 21;
  pwmPins["36"]=36;
  pwmPins["39"]=39;
  pwmPins["34"]=34;
  pwmPins["35"]=35;
  pwmPins["32"]=32;
  pwmPins["33"]=33;
  pwmPins["25"]=25;
  pwmPins["14"]=14;
  pwmPins["13"]=13;
  pwmPins["2"]=2;
  pwmPins["1"]=1;
  pwmPins["3"]=3;
  pwmPins["23"]=23;
  pwmPins["4"]=4;
  pwmPins["0"]=0;
  pwmPins["22"]=22;
  pwmPins["21"]=21;
}
#else

void CSystemMgr::initI2CMap(){
  
  I2CMap["0_74"]="0>Temp_OBC - 0x4A/74";

  I2CMap["0_73"]="2> Temp X1 - 0x49/73";
  I2CMap["0_72"]="2> Temp X - 0x48/72";

  
  I2CMap["1_73"]="2> Temp Y1 - 0x49/73";
  I2CMap["1_72"]="2> Temp Y - 0x48/72";

  I2CMap["1_96"]="1> 96";  
  I2CMap["1_54"]="1> 54";
  I2CMap["1_8"]=" 1> 8";
  
  I2CMap["2_99"]="2> Mag Z - 0x63/99";
  I2CMap["2_97"]="2> Mag Y - 0x61/97";
  I2CMap["2_96"]="2> Mag X - 0x60/96";
  I2CMap["2_73"]="2> Temp Z1 - 0x49/73";
  I2CMap["2_72"]="2> Temp Z - 0x48/72";

};

void CSystemMgr::initPins() {
  
  Pins["PA_0"]=PA_0;
  //Pins["PA_1"]=PA_1;
  Pins["PA_10"]=PA_10;
  Pins["PA_11"]=PA_11;
  Pins["PA_12"]=PA_12;
  Pins["PA_13"]=PA_13;
  Pins["PA_14"]=PA_14;
  Pins["PA_15"]=PA_15;
  Pins["PA_2"]=PA_2;
//  Pins["PA_3"]=PA_3;
  Pins["PA_4"]=PA_4;
//  Pins["PA_5"]=PA_5;
  Pins["PA_6"]=PA_6;
  //Pins["PA_7"]=PA_7;
  Pins["PA_8"]=PA_8;
  Pins["PA_9"]=PA_9;
//  Pins["PB_0"]=PB_0;
 // Pins["PB_1"]=PB_1;
//  Pins["PB_10"]=PB_10;
  //Pins["PB_11"]=PB_11;
  //Pins["PB_12"]=PB_12;
  //Pins["PB_13"]=PB_13;
  Pins["PB_14"]=PB_14;
  Pins["PB_15"]=PB_15;
  Pins["PB_2"]=PB_2;
  Pins["PB_3"]=PB_3;
  Pins["PB_4"]=PB_4;
  //Pins["PB_5"]=PB_5;
  Pins["PB_6"]=PB_6;
  Pins["PB_7"]=PB_7;
  Pins["PB_8"]=PB_8;
  Pins["PB_9"]=PB_9;
  //Pins["PC_0"]=PC_0;
  //Pins["PC_1"]=PC_1;
  Pins["PC_10"]=PC_10;
  Pins["PC_11"]=PC_11;
  Pins["PC_12"]=PC_12;
  Pins["PC_13"]=PC_13;
  Pins["PC_14"]=PC_14;
  Pins["PC_15"]=PC_15;
  Pins["PC_2"]=PC_2;
  Pins["PC_3"]=PC_3;
  //Pins["PC_4"]=PC_4;
  //Pins["PC_5"]=PC_5;
  Pins["PC_6"]=PC_6;
  Pins["PC_7"]=PC_7;
  Pins["PC_8"]=PC_8;
  Pins["PC_9"]=PC_9;
  Pins["PD_0"]=PD_0;
  Pins["PD_1"]=PD_1;
  Pins["PD_10"]=PD_10;
  Pins["PD_11"]=PD_11;
  Pins["PD_12"]=PD_12;
  Pins["PD_13"]=PD_13;
  Pins["PD_14"]=PD_14;
  Pins["PD_15"]=PD_15;
  Pins["PD_2"]=PD_2;
  Pins["PD_3"]=PD_3;
  Pins["PD_4"]=PD_4;
  Pins["PD_5"]=PD_5;
  Pins["PD_6"]=PD_6;
  Pins["PD_7"]=PD_7;
  Pins["PD_8"]=PD_8;
  Pins["PD_9"]=PD_9;
  Pins["PE_0"]=PE_0;
  Pins["PE_1"]=PE_1;
  Pins["PE_10"]=PE_10;
  Pins["PE_11"]=PE_11;
  Pins["PE_12"]=PE_12;
  Pins["PE_13"]=PE_13;
  Pins["PE_14"]=PE_14;
  Pins["PE_15"]=PE_15;
  Pins["PE_2"]=PE_2;
  Pins["PE_3"]=PE_3;
  Pins["PE_4"]=PE_4;
  Pins["PE_5"]=PE_5;
  Pins["PE_6"]=PE_6;
  Pins["PE_7"]=PE_7;
  Pins["PE_8"]=PE_8;
  Pins["PE_9"]=PE_9;
  Pins["PF_0"]=PF_0;
  Pins["PF_1"]=PF_1;
  Pins["PF_10"]=PF_10;
  Pins["PF_11"]=PF_11;
  Pins["PF_12"]=PF_12;
  Pins["PF_13"]=PF_13;
  Pins["PF_14"]=PF_14;
  Pins["PF_15"]=PF_15;
  Pins["PF_2"]=PF_2;
  Pins["PF_3"]=PF_3;
  Pins["PF_4"]=PF_4;
  Pins["PF_5"]=PF_5;
  Pins["PF_6"]=PF_6;
  Pins["PF_7"]=PF_7;
  Pins["PF_8"]=PF_8;
  Pins["PF_9"]=PF_9;
  Pins["PG_0"]=PG_0;
  Pins["PG_1"]=PG_1;
  Pins["PG_10"]=PG_10;
  //Pins["PG_11"]=PG_11;
  //Pins["PG_12"]=PG_12;
  //Pins["PG_13"]=PG_13;
  
  Pins["PG_14"]=PG_14;
  Pins["PG_15"]=PG_15;
  Pins["PG_2"]=PG_2;
  Pins["PG_3"]=PG_3;
  Pins["PG_4"]=PG_4;
  Pins["PG_5"]=PG_5;
  Pins["PG_6"]=PG_6;
  Pins["PG_7"]=PG_7;
  Pins["PG_8"]=PG_8;
  Pins["PG_9"]=PG_9;
  Pins["PH_0"]=PH_0;
//  Pins["PH_1"]=PH_1;  ??Can go High not Low
  Pins["PH_10"]=PH_10;
  Pins["PH_11"]=PH_11;
  Pins["PH_12"]=PH_12;
  Pins["PH_13"]=PH_13;
  Pins["PH_14"]=PH_14;
  Pins["PH_15"]=PH_15;
  Pins["PH_2"]=PH_2;
  Pins["PH_3"]=PH_3;
//  Pins["PH_4"]=PH_4;
  Pins["PH_5"]=PH_5;
  Pins["PH_6"]=PH_6;

  
  Pins["PH_7"]=PH_7;
  Pins["PH_8"]=PH_8;
  Pins["PH_9"]=PH_9;
  
  Pins["PI_0"]=PI_0;
  Pins["PI_1"]=PI_1;
//  Pins["PI_10"]=PI_10;
//  Pins["PI_11"]=PI_11;
  Pins["PI_12"]=PI_12;
  Pins["PI_13"]=PI_13;
  Pins["PI_14"]=PI_14;

  
  Pins["PI_15"]=PI_15;
  Pins["PI_2"]=PI_2;
  Pins["PI_3"]=PI_3;
  Pins["PI_4"]=PI_4;
  Pins["PI_5"]=PI_5;
  Pins["PI_6"]=PI_6;
  Pins["PI_7"]=PI_7;
  Pins["PI_8"]=PI_8;
  Pins["PI_9"]=PI_9;
  Pins["PJ_0"]=PJ_0;
  Pins["PJ_1"]=PJ_1;
  Pins["PJ_10"]=PJ_10;
  Pins["PJ_11"]=PJ_11;
  Pins["PJ_12"]=PJ_12;
  Pins["PJ_13"]=PJ_13;
  Pins["PJ_14"]=PJ_14;
  //Pins["PJ_15"]=PJ_15;
  Pins["PJ_2"]=PJ_2;
  Pins["PJ_3"]=PJ_3;
//  Pins["PJ_4"]=PJ_4;
  Pins["PJ_5"]=PJ_5;
  Pins["PJ_6"]=PJ_6;
  Pins["PJ_7"]=PJ_7;
  Pins["PJ_8"]=PJ_8;
  Pins["PJ_9"]=PJ_9;
  Pins["PK_0"]=PK_0;
  Pins["PK_1"]=PK_1;
  Pins["PK_2"]=PK_2;
  Pins["PK_3"]=PK_3;
  Pins["PK_4"]=PK_4;
  Pins["PK_5"]=PK_5;
  Pins["PK_6"]=PK_6;
  Pins["PK_7"]=PK_7;
  
  pwmPins["PH_15"]=PH_15;
  pwmPins["PK_1"]=PK_1;
  pwmPins["PJ_11"]=PJ_11;
  pwmPins["PG_7"]=PG_7;
  pwmPins["PC_7"]=PC_7;
  pwmPins["PC_6"]=PC_6;
  pwmPins["PA_8"]=PA_8;
}

#endif


void CSystemMgr::setupIMUI2C(TwoWire *wire){  
  BNO080 myIMU;

  myIMU.enableDebugging();
  CMsg m;
  m.setSYS("TESTIMUI2C");
  for(int count=0; count<5;count++){

    if (myIMU.begin(IMUADDRESS1,*wire) ){  //IMUADDRESS1

      m.setINFO("I2C IMU FOUND");
      wire->setClock(400000); //Increase I2C data rate to 400kHz
    
      myIMU.enableRotationVector(50); //Send data update every 50ms
    
      long ct=getTime();
      while(getTime()<(ct+IMU_WAIT_TIME)){
         if (myIMU.dataAvailable() == true) {
          m.setParameter("PITCH",(float)myIMU.getPitch() * 180.0 / PI); // Convert pitch to degrees            
          m.setParameter("ROLL",(float)myIMU.getRoll() * 180.0 / PI); // Convert roll to degrees
          m.setParameter("YAW",(float)myIMU.getYaw() * 180.0 / PI); // Convert yaw / heading to degrees          
         }

        
      }   
    }
    else{
      m.setERROR("I2C IMU Not Deteced");
      delay(500);
       
    }
  }  
  writeconsoleln(m.serializeout());
  addTransmitList(m);
  return;
}


void CSystemMgr::setupIMUSPI(){
  //writeconsoleln("B xxxxxxxxxxxxxxxxxx1");
#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)  
  BNO080 myIMU;
  CMsg m;
  m.setSYS("TESTIMUSPI");
for(int retries=0;retries<5;retries++){
    if(myIMU.beginSPI(IMU_OBC_NSS, IMU_OBC_WAKE, IMU_OBC_INT, IMU_OBC_RST) == false)
    {
    m.setERROR("SPI IMU Not Deteced");     
      //writeconsoleln("C xxxxxxxxxxxxxxxxxx1");
    }
    else{ 
     //  myIMU.enableRotationVector(50); //Send data update every 50ms
     //   myIMU.enableGyro(50); //Send data update every 50ms
     myIMU.enableGyroIntegratedRotationVector(50); //Send data update every 50ms
  
      //writeconsoleln("D xxxxxxxxxxxxxxxxxx1");
     m.setINFO("SPI IMU FOUND");     
    }
  }
  writeconsoleln("E xxxxxxxxxxxxxxxxxx1");
  writeconsoleln(m.serializeout());
  addTransmitList(m);
#endif  
}

void CSystemMgr::pinsOn(){
  CMsg m;
  m.setSYS("Pins ON");
  for (auto x:Pins){
    writeconsoleln(x.first.c_str());     
    digitalWrite(x.second, HIGH);    
    delay(250);
  }
  writeconsoleln(m.serializeout());
  addTransmitList(m);
}

void CSystemMgr::pinsOff(){
  CMsg m;
  m.setSYS("Pins OFF");
  for (auto x:Pins){
    writeconsoleln(x.first.c_str());
     
    digitalWrite(x.second, LOW);    
    delay(250);
  }  
  writeconsoleln(m.serializeout());
  addTransmitList(m);
}


void CSystemMgr::phone(){
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
 
  enablePhone();
   
  Serial1.begin(PHONE_BAUD_RATE);
#else  
   Serial1.begin(PHONE_BAUD_RATE,SERIAL_8N1, PHONE_TX, PHONE_RX);
#endif  

  Serial1.write("Hello");
   sendSerial("INIT"); //to synchronise Arduino and app
}

void CSystemMgr::sendSerial(const char* cmd) {    //Send to Phone
  Serial1.print(1);
  Serial1.flush();
  Serial1.print(",");
  Serial1.flush();
  Serial1.println(cmd);
  Serial1.flush();
  delayMicroseconds(500); 


  Serial.print("Sending to phone => ");
  Serial.print(cmd);

  CMsg m;
  m.setSYS("SendSerial");
  m.setINFO(cmd);
  writeconsoleln(m.serializeout());
  
  addTransmitList(m);
}


void CSystemMgr::burn(){
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  // pinMode(PI_10, OUTPUT);
  enableBurnWire();
  delay(10000);
  disableBurnWire();
  CMsg m;
  m.setSYS("BURN");
  m.setINFO("ACTIVATED");
  writeconsoleln(m.serializeout());
  
  addTransmitList(m);
  #endif
}




void CSystemMgr::enableI2C(){
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
 
  enableSensors();  
  enableMagsMotors();  

  CMsg m;
  m.setSYS("ENABLE I2C");
  m.setINFO("enableSensors");
  m.setCOMMENT("enableMags");
  writeconsoleln(m.serializeout());
  
  addTransmitList(m);
  #endif
}

void CSystemMgr::loopWire(TwoWire *wire,const char * s) {
  byte error, address;
  int nDevices=0;
  int naddress;
  enableSensors();
  enableMagsMotors();  
  
  CMsg m;
  m.setSYS("LoopWire");
  m.setINFO("Scanning");
  for(byte address = 1; address < 127; address++ ) {
    std::string strI;
    std::string strIM;
    strI=s;
    strI+="_";

    writeconsole(".");
    wire->beginTransmission(address);
    error = wire->endTransmission();
    if (error == 0) {      
      //if (address<16)  writeconsole("0");
      
      std::string pstr= "Address_";
      pstr+=address; 
      naddress=address;
      strI+=tostring(naddress);
      strIM=I2CMap[strI];
      m.setParameter(pstr,address);
      m.setParameter(strI,strIM);
      nDevices++;
    }
    else if (error==4) {
      writeconsole("Unknow error at address 0x");
      if (address<16) {
        writeconsole("0");
      }
      writeconsoleln(address);  //HEX
      std::string pstr= "UnknownAddress_";
      pstr+=address; 
      m.setParameter(pstr,address);
    }    
  }
  if (nDevices == 0) {
    m.setINFO("No devices found.");
  }
 
  writeconsoleln(m.serializeout());
  
  addTransmitList(m);
  delay(150);          
}





void CSystemMgr::SendCmd(std::string str) {
  PinName  n = Pins[str];
  char action = 'H';
  
  if ((str[0] == 'H') || (str[0] == 'L') || (str[0] == 'W')) {
    action = str[0];
    str.erase(0, 1);
    writeconsole(">");
  }

  n = Pins[str];



  if (str == "RADIO") {
    CMsg m;
    m.setSYS("Radio");
    m.setINFO("setup");
    CRadio *pradio=(CRadio *)getSystem("RADIO");
    if(pradio!=NULL) pradio->setup();
    writeconsoleln(m.serializeout());
    addTransmitList(m);
  return;
  }

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
if  (str == "MOTORX"){
  testMotor("MOTORX");
  return;
}

if  (str == "MOTORY"){
  testMotor("MOTORY");
  return;
}

if  (str == "MOTORZ"){
  testMotor("MOTORZ");
  return;
}

if  (str == "MAGX"){
  testMAG(MAG_ADDRESS_X);
  return;
}

if  (str == "MAGDX"){
  testMAGDrive(MAG_ADDRESS_X);
  return;
}

if  (str == "MAGY"){
  testMAG(MAG_ADDRESS_Y);
  return;
}

if  (str == "MAGDY"){
  testMAGDrive(MAG_ADDRESS_Y);
  return;
}

if  (str == "MAGZ"){
  testMAG(MAG_ADDRESS_Z);
  return;
}

if  (str == "MAGDZ"){
  testMAGDrive(MAG_ADDRESS_Z);
  return;
}


if  (str == "ADCSON"){
  enableADCS();
  return;
}

if  (str == "ADCSOFF"){
  disableADCS();
  return;
}


#endif

  if (str == "?") {
    showTests();
    return;
  }



  if (str == "PINSON") {
    pinsOn();
    return;
  }

  if (str == "PINSOFF") {
    pinsOff();
    return;
  }

  if (str == "PHONEON") {
    phone();
    return;
  }

  if (str == "PHONEOFF") {
    disablePhone();
    return;
  }

  if (str == "BURN") {
    burn();
    return;
  }


  if (str == "IMUSPI") {
    //writeconsoleln("A xxxxxxxxxxxxxxxxxx1");
    setupIMUSPI();
    return;
  }
  
  if (str == "IMU0") {
    writeconsoleln("IMU0");
    Wire.begin();
    delay(10);
    setupIMUI2C(&Wire);
    return;
  }
  
  if (str == "IMU1") {
    writeconsoleln("IMU1");
    Wire1.begin();
    delay(10);
    setupIMUI2C(&Wire1);
    return;
  }

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)  
  if (str == "IMU2") {
    writeconsoleln("IMU2");
    
    delay(10);
    setupIMUI2C(getWire2());
    return;
  }
  #endif


  if (str == "I2C0") {
    enableI2C();
    writeconsoleln("I2C0");
    Wire.begin();
    delay(10);
    loopWire(&Wire, "0");
    return;
  }

  if (str == "I2C1") {
    enableI2C();
    writeconsoleln("I2C1");
    Wire1.begin();
    delay(10);
    loopWire(&Wire1,"1");
    return;
  }

  if (str == "I2C2") {
    enableI2C();
    writeconsoleln("I2C2");

    #ifndef TTGO
    
    loopWire(getWire2(),"2");
    #endif
    return;
  }

  if (action == 'H') {
    writeconsoleln("High");
    pinMode(n, OUTPUT); ///Set is to output mode   
    delay(10);
    digitalWrite(n, HIGH);
    return;
  }

  if (action == 'L') {
    writeconsoleln("Low");
    pinMode(n, OUTPUT);   ///Set is to output mode
    delay(10);
    digitalWrite(n, LOW);
    return;
  }

  if (action == 'W'){
    writeconsoleln("PWM");
    if (pwmPins.find(str) != pwmPins.end()) {
      writeconsole(" Found");
      #ifdef TTGO
        ledcSetup(_channel, freq, resolution);   // configure LED PWM functionalitites
        ledcAttachPin(n, _channel);  // attach the channel to the GPIO to be controlled
        delay(50);
        ledcWrite(_channel, (int) 130); 
    
      #else
        analogWriteResolution(12);   
        for (int count=0;count<4000;count+=100){
          analogWrite(n,count);
          writeconsole("PWM: ");
          writeconsole(count);
          delay(200);
        }
      #endif
    }
  
    return;    
  }
 writeconsoleln("");
}


void CSystemMgr::testMotor(const char *s){
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  writeconsoleln("ENTER void CSystemMgr::testMotor(char axis)");  
  
  enableMagsMotors();        
  enableADCS();

  CMotorController *pM=(CMotorController *)getSystem(s);
  if(pM!=NULL) pM->test();
  //disableMagsMotors();  
//  disableADCS();
  #endif
}

void CSystemMgr::testMAGDrive(char addr){
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  writeconsoleln("ENTER void CSystemMgr::testMAGDrive(char addr)");  
  enableSensors();  
  enableMagsMotors();  


  CMDrive MAG;
  delay(10);
  
  MAG.Name("TestMag");
  
  MAG.config(addr,getWire2());
  
  MAG.setup();
  
  MAG.Speed(1.0);
  

  delay(5000);
  
  MAG.Speed(-1.0);
  
  delay(5000);
  
  
  MAG.Speed(0.0);
  
  disableMagsMotors();
  
  writeconsoleln("EXIT void CSystemMgr::testMAGDrive(char addr)");
  #endif
}

void CSystemMgr::testMAG(char addr){
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  Adafruit_DRV8830 drv;


  enableSensors();
  enableMagsMotors();
  
  if (! drv.begin(addr,   getWire2() )) {
    writeconsoleln("Failed to find DRV8830");
    disableMagsMotors();
    return;
  }
  writeconsoleln("Adafruit DRV8830 found!");


   writeconsoleln("Forward");
  drv.run(FORWARD);
  drv.setSpeed(255);
  delay(5000);

  writeconsoleln("Release");
  drv.run(RELEASE);
  delay(500);

  writeconsoleln("Backward");
  drv.run(BACKWARD);
  drv.setSpeed(255);
  delay(5000);

  drv.run(RELEASE);
  writeconsoleln("Mag test Done");
  disableMagsMotors();
  return;
  #endif
}
