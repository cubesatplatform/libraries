

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
#else

#include <system_gps.h>
#include <systemobject.h>
#include <string>
#include <TinyGPS++.h>
#include <axp20x.h>

#include <consoleio.h>


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

CGPS::~CGPS() {}

void CGPS::init()
{
   
  CSystemObject::init();
  
  setForever();
  setInterval(15000);    
}

void CGPS::setup()
{
   Serial1.begin(GPS_BAND_RATE, SERIAL_8N1, _RX, _TX);


  delay(1500);
  setState("PLAY");
}


void CGPS::config(CMsg &msg){
  std::string strTX=msg.getParameter("TX");
  std::string strRX=msg.getParameter("RX");
  
  
  _TX=Pins[strTX];
  _RX=Pins[strRX];
  setup();
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

    fillData();
   }
}


void CGPS::fillData(){  //Easier to send as long   convert to decimal when receive  
  CMsg m;
  std::string pos="lat:";
  pos+=tostring(latitude)+std::string(",lon:")+tostring(longitude)+std::string(",alt:")+tostring(altitude)+std::string(",sats:")+tostring(SIV)+std::string(",time:")+tostring(Hr)+std::string(",")+tostring(Min)+std::string(",")+tostring(Sec);
  m.setNAME("GPS");
  m.setParameter("POS",pos);
  m.setTIME(getTime());

  addDataMap(std::string("GPS"),m);
 }



#endif 