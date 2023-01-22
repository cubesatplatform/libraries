#include  <string>
#include "system_mgr.h"
#include <messages.h>
#include "mgr_defs.h"
  


void CSystemMgr::callCustomFunctions(CMsg &msg){
  
  std::string act=msg.get(_ACT); 
  
  mapcustommsg(chkAll) 
  mapcustommsg(chkRadio)
  mapcustommsg(chkTemperature)
  mapcustommsg(chkIRArrays)
  mapcustommsg(chkBattery)
  mapcustommsg(chkRotation)
  mapcustommsg(chkMagField)
  mapcustommsg(chkMags)
  mapcustommsg(chkMessages)
  mapcustommsg(IMU)
  mapcustommsg(chkIMUI)
  mapcustommsg(chkIMUS)

  mapcustom(enableI2C)
  mapcustom(disableI2C)
  mapcustom(showTests)
  
  mapcustommsg(getData)
  mapcustommsg(loopWire)
  mapcustom(randomState)
  
  mapcustommsg(pinHigh)
  mapcustommsg(pinLow)  
  mapcustommsg(pinPWM)  

  mapcustommsg(pinRead)
  mapcustommsg(pinReadState)
  mapcustommsg(resetWire)
  mapcustommsg(sendBeacon)

  mapcustommsg(testDataMap)
  mapcustommsg(tBeam)

  mapcustommsg(runSystem)
  

  mapcustom(pinsOn)
  mapcustom(pinsOff)

  mapcustom(phone)
  mapcustom(chargePhone)
  //mapcustom(disablePhone)

  mapcustommsg(IMU)
  mapcustommsg(ir)
  mapcustommsg(motor)
  mapcustommsg(mag)
  mapcustommsg(temp)
  mapcustommsg(sendPicture)


  if(act=="MESSAGESLISTCLEAR") {MMM.MessageList.clear();return;}
  if(act=="TRANSMITLISTCLEAR") {MMM.TransmitList.clear();return;}
  if(act=="TRANSMITTEDLISTCLEAR") {MMM.TransmittedList.clear();return;}

  if(act=="SENDDATA") {MMM.sendData(msg);return;}  


  CSystemObject::callCustomFunctions(msg);
}  
 