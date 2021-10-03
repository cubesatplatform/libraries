/**************************************************************************/
/*!
  @file     Adafruit_DRV8830.cpp

  @mainpage Adafruit DRV8830 I2C motor driver

  @section intro Introduction

  This is a library for the Adafruit DRV8830 I2C motor driver
  ----> http://www.adafruit.com/products/4558

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  @section author Author

  Limor Fied (Adafruit Industries)

  @section license License

  BSD (see license.txt)
*/
/**************************************************************************/
#include "Adafruit_DRV8830.h"

/**************************************************************************/
/*!
    @brief  Instantiates a new DRV8830 class
*/
/**************************************************************************/
Adafruit_DRV8830::Adafruit_DRV8830() {}

/**************************************************************************/
/*!
    @brief  Sets up the I2C connection and tests that the sensor was found.
    @param addr The 7-bit I2C address of the DRV8830
    @param theWire Pointer to an I2C device we'll use to communicate
    default is Wire
    @return true if sensor was found, otherwise false.
*/
/**************************************************************************/
bool Adafruit_DRV8830::begin(uint8_t addr, TwoWire *theWire) {
  if (i2c_dev) {
    delete i2c_dev;
  }
  i2c_dev = new Adafruit_I2CDevice(addr, theWire);

  /* Try to instantiate the I2C device. */
  if (!i2c_dev->begin()) {
    return false;
  }

  // define the main power control register
  ctrl_reg = new Adafruit_I2CRegister(i2c_dev, DRV8830_CONTROL);
  fault_reg = new Adafruit_I2CRegister(i2c_dev, DRV8830_FAULT);

  return true;
}



/**************************************************************************/
/*!
    @brief Set the PWM output
    @param throttle 0-255 value which will be mapped as best as possible
    to the DRV8830 PWM/DAC system
    @return False if unable to communicate with DRV
*/
/**************************************************************************/
bool Adafruit_DRV8830::setSpeed(uint8_t throttle) {
  throttle = map(throttle, 0, 255, 0x0, 0x3F);
  if (throttle < 6) throttle = 6;

  //Serial.println(throttle, HEX);
  Adafruit_I2CRegisterBits dac =
      Adafruit_I2CRegisterBits(ctrl_reg, 6, 2); // # bits, bit_shift
  return dac.write(throttle);
}

/**************************************************************************/
/*!
    @brief Set the motor spin direction / H-bridge configuration
    @param direction Action you want for the motor. Can be FORWARD, BACKWARD,
    BRAKE or RELEASE
    @return False if unable to communicate with DRV
*/
/**************************************************************************/
bool Adafruit_DRV8830::run(DRV8830_Direction direction) {
  Adafruit_I2CRegisterBits dirbits =
      Adafruit_I2CRegisterBits(ctrl_reg, 2, 0); // # bits, bit_shift
  return dirbits.write((uint8_t) direction);
}


/**************************************************************************/
/*!

*/
/**************************************************************************/
uint8_t Adafruit_DRV8830::getFaults(void) {
  uint8_t fault = fault_reg->read();
  fault &= 0x1F;
  return fault;
}

bool Adafruit_DRV8830::clearFaults(void) {
  Adafruit_I2CRegisterBits clear =
      Adafruit_I2CRegisterBits(fault_reg, 1, 7); // # bits, bit_shift
  return clear.write(1);
}

