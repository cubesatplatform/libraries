#include  <string>
#include "system_mgr.h"
#include <messages.h>
#include "mgr_defs.h"
#include <powerup.h>



void CSystemMgr::callCustomFunctions(CMsg &msg){
  CSystemObject::callCustomFunctions(msg);
  CMessages* pMSG= getMessages();
  std::string act=msg.getACT(); 
  
  writeconsole(" CallCustomFunctions :  "); writeconsoleln(act);
  msg.serializeout();
  
  if(act=="CHKRADIO") {chkRadio(msg); return; }
  if(act=="CHKTEMPERATURE") {chkTemperature(msg); return; }
  if(act=="CHKIRARRAYS") {chkIRArrays(msg); return; }
  if(act=="CHKBATTERY") {chkBattery(msg); return; }
  if(act=="CHKROTATION") {chkRotation(msg); return; }
  if(act=="CHKMAGFIELD") {chkMagField(msg); return; }
  if(act=="CHKMESSAGES") {chkMessages(msg); return; }
  
  if(act=="ENABLEI2C") {enableI2C(); return; }
  if(act=="DISABLEI2C") {disableI2C(); return; }
  
  if(act=="SHOWTESTS") {showTests(); return; }
  
  if(act=="GETDATA") {getData(msg); return; }  
  
  if(act== "MAGDX"){  testMAGDrive(MAG_ADDRESS_X); return; }
  if(act== "MAGDY"){  testMAGDrive(MAG_ADDRESS_Y); return; }
  if(act== "MAGDZ"){  testMAGDrive(MAG_ADDRESS_Z); return; }

  if(act== "MBLOGICON"){  enableMBLogic(); writeconsoleln("Enable MotorBoard Logic"); return; }
  if(act== "MBLOGICOFF"){  disableMBLogic(); writeconsoleln("Disable MotorBoard Logic"); return; }

  if(act== "SENSORSON"){  enableSensors(); writeconsoleln("Enable Sensors"); return; }
  if(act== "SENSORSOFF"){  disableSensors(); writeconsoleln("Disable Sensors"); return; }

  if(act== "65VON"){  enable65V();  writeconsoleln("Enable 6.5V"); return; }
  if(act== "65VOFF"){  disable65V();  writeconsoleln("Disable 6.5V"); return; }

  if(act== "ALLON"){enable65V();    enableSensors();    enableMBLogic();    writeconsoleln("All ON"); return;}
  if(act== "ALLOFF"){disable65V();    disableSensors();    disableMBLogic();    writeconsoleln("All OFF"); return;}  

  if(act=="PINHIGH"){ pinHigh(msg); return;}
  if(act=="PINLOW"){ pinLow(msg); return;}
  if(act=="PINPWM"){ pinPWM(msg); return;}

  if(act=="PINREAD"){ pinRead(msg); return;}
  if(act=="PINREADSTATE"){ pinReadState(msg); return;}
  if(act=="BEACON") sendBeacon(msg);   
	
  if(act=="FILLDATA"){ testDataMap(msg); return;}
  if(act=="TBEAM"){ tBeam(msg); return;}
   
  //if(act=="DATAMAPCLEAR") {pMSG->DataMap.clear();return;}
  if(act=="MESSAGESLISTCLEAR") {pMSG->MessageList.clear();return;}
  if(act=="TRANSMITLISTCLEAR") {pMSG->TransmitList.clear();return;}
  if(act=="TRANSMITTEDLISTCLEAR") {pMSG->TransmittedList.clear();return;}
  if(act=="SENDDATA") {pMSG->sendData(msg);return;}  

  if(act== "?") {    showTests();    return;  }

  if(act== "PINSON") {    pinsOn();    return;  }
  if(act== "PINSOFF") {    pinsOff();    return;  }
  
  if((act== "IMUI2C") || (act== "IMUSPI")) {    testIMU(msg);    return;  }
  
  if(act== "I2C0") {   loopWire(&Wire, "0");   return; }
  if(act== "I2C1") {   loopWire(&Wire1,"1");   return; }
  if(act== "I2C2") {   loopWire(&Wire2,"2");   return; }

  if(act == "RESETI2C0") { resetWire(&Wire, "0");    return;  }
  if(act == "RESETI2C1") { resetWire(&Wire1,"1");    return;  }

  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
  if(act== "PHONEON") { phone();    return;  }
  if(act== "PHONEOFF") { disablePhone();    return;  }  
  if((act== "MOTORX") || (act== "MOTORY") || (act== "MOTORZ") ){  testMotor(msg);  return;}
  if((act== "IRX1") || (act== "IRX2") ||  (act== "IRY1") || (act== "IRY2") ||  (act== "IRZ1") || (act== "IRZ2") ){  testIR(msg);  return;}
  if((act== "MAGX") || (act== "MAGY") || (act== "MAGZ") ){testMAG(msg);return;}
  if((act== "TEMPX1") || (act== "TEMPX2") || (act== "TEMPY1") || (act== "TEMPY2") || (act== "TEMPZ1") || (act== "TEMPZ2") || (act== "TEMPOBC") || (act== "TEMPADCS")  ){    testTemp(msg);    return;  }
  if(act == "RESETI2C2") { resetWire(&Wire2,"2");    return;  }
  #endif
  
}  
 