#include <defs.h>
#include "i2cinfo.h"

#ifdef PORTENTAXXX

void CI2CInfo::setup(){
    addMap(0,ADDRESS_IMU, "ADCS IMU");    
    addMap(0,ADDRESS_IRARRAY, "X Panel IRArray");
    addMap(0,ADDRESS_IRARRAY1, "X1 Panel IRArray");
    
    addMap(0,ADDRESS_TEMP, "Z Panel Thermometer");
    addMap(0,ADDRESS_TEMP1, "Z1 Panel Thermometer");

    addMap(1,ADDRESS_PORTENTA, "PORTENTA Internal");
    addMap(1,ADDRESS_PORTENTA1, "PORTENTA Internal");

    
    addMap(1,ADDRESS_IRARRAY, "Y Panel IRArray");
    addMap(1,ADDRESS_IRARRAY1, "Y1 Panel IRArray");
    
    addMap(1,ADDRESS_TEMP, "Y Panel Thermometer");
    addMap(1,ADDRESS_TEMP1, "Y1 Panel Thermometer");
    
    addMap(2,ADDRESS_IRARRAY, "X Panel IRArray");    
    addMap(2,ADDRESS_IRARRAY1, "X1 Panel IRArray");
    addMap(2,ADDRESS_TEMP, "X Panel Thermometer");
    addMap(2,ADDRESS_TEMP1, "X1 Panel Thermometer");

    addMap(2,ADDRESS_MAGX, "Mag X");
    addMap(2,ADDRESS_MAGY, "Mag Y");
    addMap(2,ADDRESS_MAGZ, "Mag Z");    

    addMap(2,ADDRESS_TEMP2, "OBC Thermometer");
    addMap(2,ADDRESS_IMU, "OBC IMU");
}

  
void CI2CInfo::addMap(int channel,char address,const char *desc){
  int x=channel*100+address;
  info[x]=std::string(desc);    
}

std::string CI2CInfo::getInfo(int channel,char address){
    int x=channel*100+address;
    return(info[x]);    
}



void CI2CInfo::loopWire(TwoWire *wire,int channel=0)
{
  byte error, address;
  int nDevices;
  

  //if(wire==&Wire) channel=0;
  //if(wire==&Wire1) channel=1;
  //if(wire==&Wire2) channel=2;


  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    wire->beginTransmission(address);                              //
    error = wire->endTransmission();                               //

    if (error == 0)
    {
  
      Serial.print("I2C device found at address 0x");
      if (address<16) Serial.print("0");
      Serial.print(address,HEX);
            
      Serial.print("     ");
      Serial.println(getInfo(channel,address).c_str());    
      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16)  Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");


  delay(500);           // wait .5 seconds for next scan
}

#endif