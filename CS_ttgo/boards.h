#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "utilities.h"


#define LILYGO_TBeam_V1_X

#if defined(LILYGO_TBeam_V1_X)
#include <axp20x.h>
AXP20X_Class PMU;

bool initPMU()
{
    if (PMU.begin(Wire, AXP192_SLAVE_ADDRESS) == AXP_FAIL) {
        Serial.println("AXP192 Address Error  Resetting Wire too");
        Wire.begin(I2C_SDA, I2C_SCL);
        return false;
    }
    /*
     * The charging indicator can be turned on or off
     * * * */
    // PMU.setChgLEDMode(LED_BLINK_4HZ);

    /*
    * The default ESP32 power supply has been turned on,
    * no need to set, please do not set it, if it is turned off,
    * it will not be able to program
    *
    *   PMU.setDCDC3Voltage(3300);
    *   PMU.setPowerOutPut(AXP192_DCDC3, AXP202_ON);
    *
    * * * */

    /*
     *   Turn off unused power sources to save power
     * **/

/*
    PMU.setPowerOutPut(AXP192_DCDC1, AXP202_OFF);
    PMU.setPowerOutPut(AXP192_DCDC2, AXP202_OFF);
    PMU.setPowerOutPut(AXP192_LDO2, AXP202_OFF);
    PMU.setPowerOutPut(AXP192_LDO3, AXP202_OFF);
    PMU.setPowerOutPut(AXP192_EXTEN, AXP202_OFF);
*/
    /*
     * Set the power of LoRa and GPS module to 3.3V
     **/
    PMU.setLDO2Voltage(3300);   //LoRa VDD
    PMU.setLDO3Voltage(3300);   //GPS  VDD
    PMU.setDCDC1Voltage(3300);  //3.3V Pin next to 21 and 22 is controlled by DCDC1

    PMU.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
    PMU.setPowerOutPut(AXP192_LDO2, AXP202_ON);
    PMU.setPowerOutPut(AXP192_LDO3, AXP202_ON);

    //PMU.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
    //PMU.setPowerOutPut(AXP192_EXTEN, AXP202_ON);

    pinMode(PMU_IRQ, INPUT_PULLUP);
    attachInterrupt(PMU_IRQ, [] {
        // pmu_irq = true;
    }, FALLING);

    PMU.adc1Enable(AXP202_VBUS_VOL_ADC1 |
                   AXP202_VBUS_CUR_ADC1 |
                   AXP202_BATT_CUR_ADC1 |
                   AXP202_BATT_VOL_ADC1,
                   AXP202_ON);

    PMU.enableIRQ(AXP202_VBUS_REMOVED_IRQ |
                  AXP202_VBUS_CONNECT_IRQ |
                  AXP202_BATT_REMOVED_IRQ |
                  AXP202_BATT_CONNECT_IRQ,
                  AXP202_ON);
    PMU.clearIRQ();

    delay(500);
    Serial.println("PMU init done   ON");

    return true;
}

void disablePeripherals()
{
    Serial.println("PMU   OFF");
  //  PMU.setPowerOutPut(AXP192_DCDC1, AXP202_OFF);   //Dont ever turn this off   Don't come back on
   // PMU.setPowerOutPut(AXP192_LDO2, AXP202_OFF);   
   // PMU.setPowerOutPut(AXP192_LDO3, AXP202_OFF);
}
#else
#define initPMU()
#define disablePeripherals()
#endif

SPIClass SDSPI(HSPI);

void findWire(){
    for(byte address = 1; address < 127; address++ ) {
    

    Serial.print(".");
    
    Wire.beginTransmission(address);
    
    int error = Wire.endTransmission();
    
    if (error == 0) {      
        Serial.println("I2C device found at address 0x" + String(address,HEX) + " !");      
        }
    }
}

void initBoard()
{
    SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);
    Wire.begin(I2C_SDA, I2C_SCL);

#ifdef HAS_GPS
    Serial1.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
#endif

  }

