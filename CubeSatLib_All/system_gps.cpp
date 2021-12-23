
#include "defs.h"

#if defined(TTGO)  || defined( TTGO1) 

#include "system_gps.h"   
#include "systemobject.h"
#include <string>
#include <TinyGPS++.h>
#include <axp20x.h>

#include "consoleio.h"


HardwareSerial GPSL(1);


CGPS::CGPS()  {  
  Name("GPS");   
  init();
  }


void CGPS::init()
{
   
  CSystemObject::init();
  
  setForever();
  setInterval(5000);    
}

void CGPS::setup()
{
  //Serial.begin(115200);
  //Wire.begin(TTGO_I2C_SDA, TTGO_I2C_SCL);
  //  initBoard();
  #ifdef TTGO1
   initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);  
   #else 

  if (!axp.begin(Wire, AXP192_SLAVE_ADDRESS)) {
    writeconsole("AXP192 Begin PASS");
  } else {
    writeconsole("AXP192 Begin FAIL");
  }
  axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
  axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
  axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
  #endif
  GPSL.begin(9600, SERIAL_8N1, 34, 12);   //17-TX 18-RX
  delay(1500);
  setState("PLAY");
}

void CGPS::loop()
{
  CMsg m;
  runOnce(m);
}

void CGPS::runOnce(CMsg &msg)
{
  bool flag=false;
  unsigned int counter=10000;
   while (GPSL.available()&&counter){
      gpsl.encode(GPSL.read());
      flag=true;
      counter--;
   }

   if(flag){
    latitude=gpsl.location.lat();
    longitude=gpsl.location.lng();
    altitude=gpsl.altitude.feet();
    SIV=gpsl.satellites.value();
   // Yr=gpsl.time.year();
    //Mon=gpsl.time.month();
    //Day=gpsl.time.day();
    Hr=gpsl.time.hour();
    Min=gpsl.time.minute();
    Sec=gpsl.time.second();
    //Speed=gpsl.time.kmph()

    //Output();   //Called automatically
   }
 
}



/*
void CGPS::runOnce(CMsg &msg)
{
  if (gps.encode(Serial1.read())){
    if (gps.location.isValid()) {
        latitude=gps.location.lat();        
        longitude=gps.location.lng();
    } 

    
    if (gps.date.isValid()) {
        Mon=gps.date.month();        
        Day=gps.date.day();        
        Yr=gps.date.year();
    } 
    
    if (gps.time.isValid()) {
       Hr=gps.time.hour();
        Min=gps.time.minute();
        Sec=gps.time.second();        
        //gps.time.centisecond();
    } 
  } 

}
*/


CMsg CGPS::fillData(){  //Easier to send as long   convert to decimal when receive
  
   CMsg m;
   m.setParameter("table","gps");
   m.setParameter("lat",latitude);
   m.setParameter("lon",longitude);
   m.setParameter("alt",altitude);
   m.setParameter("siv",(long)SIV);
   m.setParameter("hr",(long) Hr);
   m.setParameter("min",(long) Min);
   m.setParameter("sec",(long) Sec);
   m.setParameter("mon",(long) Mon);
   m.setParameter("day",(long) Day);
   m.setParameter("yr",(long) Yr);

   
  return m;
 }



#endif 