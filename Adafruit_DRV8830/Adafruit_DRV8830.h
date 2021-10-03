/**************************************************************************/
/**
  @file     Adafruit_DRV8830.h

  Author: Limor Fried (Adafruit Industries)
  License: BSD (see license.txt)

  This is a library for the Adafruit DRV8830 I2C motor driver
  ----> http://www.adafruit.com/products/4558

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

*/
/**************************************************************************/
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Arduino.h>
#include <Wire.h>

/** Default DRV8830 I2C address. */
#define DRV8830_I2CADDR_DEFAULT 0x60 ///< I2C address
#define DRV8830_CONTROL  0x00     ///< Control register
#define DRV8830_FAULT  0x01     ///< Fault condition register

#define DRV8830_FAULT_ILIMIT 0x10
#define DRV8830_FAULT_OVERTEMP 0x08
#define DRV8830_FAULT_UVLOW 0x04
#define DRV8830_FAULT_OVERCURR 0x02


typedef enum _drv8830_dir_t {
  RELEASE = 0,
  BACKWARD = 1,
  FORWARD = 2,
  BRAKE = 3,
} DRV8830_Direction;


/**************************************************************************/
/*!
    @brief  DRV8830 driver.
*/
/**************************************************************************/
class Adafruit_DRV8830 {
public:
  Adafruit_DRV8830();
  bool begin(uint8_t addr = DRV8830_I2CADDR_DEFAULT, TwoWire *theWire = &Wire);
  bool setSpeed(uint8_t throttle);
  bool run(DRV8830_Direction dir);
  uint8_t getFaults(void);
  bool clearFaults(void);

private:
  Adafruit_I2CDevice *i2c_dev = NULL;
  Adafruit_I2CRegister *ctrl_reg = NULL;
  Adafruit_I2CRegister *fault_reg = NULL;
};
