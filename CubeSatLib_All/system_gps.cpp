
#include "defs.h"

#if defined(TTGO)  || defined( TTGO1) 

#include "system_gps.h"   
#include "systemobject.h"
#include <string>
#include <TinyGPS++.h>
#include <axp20x.h>

#include "consoleio.h"



#define GPS_RX_PIN 34
#define GPS_TX_PIN 12
#define BUTTON_PIN 38
#define BUTTON_PIN_MASK GPIO_SEL_38
#define GPS_BAND_RATE      9600

#define I2C_SDA                     21
#define I2C_SCL                     22


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
   Serial1.begin(GPS_BAND_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);


  delay(1500);
  setState("PLAY");
}

void CGPS::loop()
{
  if(subscribers(Name())){
    CMsg m;
    runOnce(m);
  }
}

void CGPS::runOnce(CMsg &msg)
{
  bool flag=false;
  unsigned int counter=10000;
  while ((Serial1.available() > 0)&&counter) {  
      gps.encode(Serial1.read());
      flag=true;
      counter--;
   }

   if(flag){
    latitude=gps.location.lat();
    longitude=gps.location.lng();
    altitude=gps.altitude.feet();
    SIV=gps.satellites.value();
   // Yr=gpsl.time.year();
    //Mon=gpsl.time.month();
    //Day=gpsl.time.day();
    Hr=gps.time.hour();
    Min=gps.time.minute();
    Sec=gps.time.second();
    //Speed=gpsl.time.kmph()

    //Output();   //Called automatically
   }
 
}



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

   addDataList(m);

   
  return m;
 }



#endif 