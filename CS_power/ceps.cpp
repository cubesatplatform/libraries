#include <Wire.h>
#include "ceps.h"




/*! \brief Initialize the EPS class
 *
 *  Initialize the EPS class and specify the address of the EPS module 
 *
 * \param addr_ Address of the EPS module 
 * \param readI2C_  I2C wrapper function to read desired number of data from the specified device and register
 * \param writeI2C_  I2C wrapper function to write desired number of data to the specified device and register
 * \return No return value 
 */



#define RECEIVEDMESSAGEWAITTIME 2*24*60*60*1000  //2 days

#define VOLTMIN 5.0
#define MCUTEMPMAX 40.0
#define VOLTCHARGED 6.0


void CEPS::config(char addr, TwoWire *twowire){
  _address=addr;
  _pWire=twowire;
  setForever();
  setInterval(500);  
  setModifiedTime(getTime());
  
  }


uint8_t  CEPS::readI2C(uint8_t reg, uint8_t * data, uint16_t nbyte)   
{
  if(getErrorCount()==getErrorThreshold()){
    incErrorCount();
    _pWire->begin();
    return 0; 
  }
  _pWire->beginTransmission(_address);
  _pWire->write(&reg,1);
  int status=_pWire->endTransmission();
  if(status>1)
    incErrorCount();
  _pWire->requestFrom(_address, nbyte);    // request 6 bytes from slave device #8

  long counter=0;

  uint16_t k = 0;
  //writeconsole("Available :"); writeconsoleln(_pWire->available());
  while ((_pWire->available())&&(counter<10000))
  { // slave may send less than requested
    uint8_t c = _pWire->read(); // receive a byte as character
    data[k++] = c;
    counter++;
  }
  return 0;
}

uint8_t CEPS::writeI2C(uint8_t reg, uint8_t * data, uint16_t nbyte)
{
  _pWire->beginTransmission(_address);
  _pWire->write(&reg,1);
  _pWire->write(data,nbyte);
  int status=_pWire->endTransmission();
  if(status>1)
    incErrorCount();
  return 0;
}

void CEPS::setup(){
  
  init();
  if(_pWire==NULL)
    config(_address);
  
}


void CEPS::checkPower(){
 
  //Maybe No logic in here   just save data?
  CMsg msg=getDataMap(_SATINFO); 
  std::string satState=msg.get(_STATE,_BLANK);

  
  if(satState!=_LOWPOWER){
    if(readBatteryVoltage()<VOLTMIN) { goState(_LOWPOWER);return;}
    if(readMCUTemp()>MCUTEMPMAX) {goState(_LOWPOWER);return;}
  }

  if(satState==_LOWPOWER){
    if(readBatteryVoltage()>VOLTCHARGED) {goState(_NORMAL);return;}
  }
  
  //if(getTime()>getReceivedTimestamp()+RECEIVEDMESSAGEWAITTIME){    //Switch radios  }
  

}

void CEPS::loop(){
  readEPS();
  checkPower();
  
}


void CEPS::readEPS(){   //FIX THIS  JUST FOR TESTING
  CMsg m;
 // writeSWSelflock(0);
  writeSHDChrg(0);
  /*
  writeSHD3V3(0);
  delay(300);
  writeSHD3V3(1);
  delay(300);

  writeSHD3V3(0);
  delay(300);
  writeSHD3V3(1);
  delay(300);
*/
  m.set(_NAME,_SYSPOWER);
  m.set(BatteryVolt,readBatteryVoltage());  
  m.set(BatteryCrnt,readBatteryCurrent());  
  m.set(BCRVolt,readBCRVoltage());
  m.set(BCRCrnt,readBCRCurrent());   
  m.set(P3V3Crnt,read3V3Current());
  m.set(P5VCrnt,read5VCurrent());
  m.set(LUP3V3,readLUP3V3());
  m.set(LUP5V,readLUP5V());
  m.set(MCUTemp,readMCUTemp());    
  m.set(InputConditions,readInputConditions());
  m.set(OutputConditions,readOutputConditions());
  m.set(OutputConditions2,readOutputConditions2());
  m.set(PowerONCycles,readPowerONCycles());
  m.set(VUnderVolt,readVUnderVoltage());
  m.set(VShortCircuit,readVShortCircuit());
  m.set(VOverTemp,readVOverTemperature());
  m.set(Ver,readSoftwareVersion());
  m.set(Defaults1,readDefaults1());
  m.set(Defaults12,readDefaults12());
  m.set(ChargeCycles,readChargeCycles());

  TEMPERATURE_INFO  tBatt=readBatteryTemperature();
  m.set(BatTemp1,tBatt.readTemp1());
  m.set(BatTemp2,tBatt.readTemp2());
  m.set(BatTemp3,tBatt.readTemp3());
  m.set(BatTemp4,tBatt.readTemp4());  

  tBatt=readMaxTemperature();
  m.set(MaxTemp1,tBatt.readTemp1());
  m.set(MaxTemp2,tBatt.readTemp2());
  m.set(MaxTemp3,tBatt.readTemp3());
  m.set(MaxTemp4,tBatt.readTemp4());

  tBatt= readMinTemperature();
  m.set(MinTemp1,tBatt.readTemp1());
  m.set(MinTemp2,tBatt.readTemp2());
  m.set(MinTemp3,tBatt.readTemp3());
  m.set(MinTemp4,tBatt.readTemp4());

  tBatt=readTemperatureSensor();
  m.set(TSTemp1,tBatt.readTemp1());
  m.set(TSTemp2,tBatt.readTemp2());
  m.set(TSTemp3,tBatt.readTemp3());
  m.set(TSTemp4,tBatt.readTemp4());
    
  AXIS_INFO axis=readXAxisInfo();
  m.set(XVolt,axis.readVoltage());
  m.set(XCrntM,axis.readCurrentM());
  m.set(XCrntP,axis.readCurrentP());

  axis=readYAxisInfo();
  m.set(YVolt,axis.readVoltage());
  m.set(YCrntM,axis.readCurrentM());
  m.set(YCrntP,axis.readCurrentP());

  axis=readZAxisInfo();
  m.set(ZVolt,axis.readVoltage());
  m.set(ZCrntM,axis.readCurrentM());
  m.set(ZCrntP,axis.readCurrentP());

  addDataMap(m);
}


void CEPS::init(){
  
  CSystemObject::init();
  setForever();
  setInterval(30000);
  setErrorThreshold(150);
  
  setState(_PLAY);
}



/*! \brief Read the battery voltage
 *
 * I2C will read 2 bytes and will calculate the battery voltage using the conversion constant
 * \return Return battery voltage 
 */
float CEPS::readBatteryVoltage(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_BATTERY_VOLTAGE,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  float voltage = value *  BATTERY_VOLTAGE_CC;
  return voltage;
}

/*! \brief Read the battery current 
 *
 * I2C will read 2 bytes and will calculate the battery current using the conversion constant
 * \return Return battery current 
 */
float CEPS::readBatteryCurrent(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_BATTERY_CURRENT,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  float current = value *  BATTERY_CURRENT_CC;
  return current;
}

/*! \brief Read the Ext battery pack voltage
 *
 * I2C will read 2 bytes and will calculate the Ext battery pack voltage using the conversion constant
 * \return Return battery voltage 
 */
float CEPS::readBCRVoltage(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_BCR_VOLTAGE,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  float voltage = value *  BCR_VOLTAGE_CC;
  return voltage;
}

/*! \brief Read the Ext. battery pack current 
 *
 * I2C will read 2 bytes and will calculate the Ext. battery pack current using the conversion constant
 * \return Return battery voltage 
 */
float CEPS::readBCRCurrent(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_BCR_CURRENT,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  float current = value *  BCR_CURRENT_CC;
  return current;
}

/*! \brief Read voltage and current for the solar panel X axis
 *
 *  This function will read following values:
 *    - X Voltage
 *    - X- Current
 *    - X+ Current
 *
 * \return Function returns AXIS_INFO type of variable 
 */
AXIS_INFO CEPS::readXAxisInfo(void)
{
  AXIS_INFO axis;
  uint8_t data[2]; 

  readI2C(EPS_RCMD_X_VOLTAGE,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  axis.setVoltage(value *  X_VOLTAGE_CC);

  readI2C(EPS_RCMD_XM_CURRENT,data,2);
  value =  (data[0] << 8) | (data[1]);
  axis.setCurrentM(value *  X_CURRENT_CC);

  readI2C(EPS_RCMD_XP_CURRENT,data,2);
  value =  (data[0] << 8) | (data[1]);
  axis.setCurrentP(value *  X_CURRENT_CC);
  return axis;
}

/*! \brief Read voltage and current for the solar panel Y axis
 *
 *  This function will read following values:
 *    - Y Voltage
 *    - Y- Current
 *    - Y+ Current
 *
 * \return Function returns AXIS_INFO type of variable 
 */
AXIS_INFO CEPS::readYAxisInfo(void)
{
  AXIS_INFO axis;
  uint8_t data[2]; 

  readI2C(EPS_RCMD_Y_VOLTAGE,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  axis.setVoltage(value *  Y_VOLTAGE_CC);

  readI2C(EPS_RCMD_YM_CURRENT,data,2);
  value =  (data[0] << 8) | (data[1]);
  axis.setCurrentM(value *  Y_CURRENT_CC);

  readI2C(EPS_RCMD_YP_CURRENT,data,2);
  value =  (data[0] << 8) | (data[1]);
  axis.setCurrentP(value *  Y_CURRENT_CC);
  return axis;
}

/*! \brief Read voltage and current for the solar panel Z axis
 *
 *  This function will read following values:
 *    - Z Voltage
 *    - Z- Current
 *    - Z+ Current
 *
 * \return Function returns AXIS_INFO type of variable 
 */
AXIS_INFO CEPS::readZAxisInfo(void)
{
  AXIS_INFO axis;
  uint8_t data[2]; 

  readI2C(EPS_RCMD_Z_VOLTAGE,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  axis.setVoltage(value *  Z_VOLTAGE_CC);

  readI2C(EPS_RCMD_ZM_CURRENT,data,2);
  value =  (data[0] << 8) | (data[1]);
  axis.setCurrentM(value *  Z_CURRENT_CC);

  readI2C(EPS_RCMD_ZP_CURRENT,data,2);
  value =  (data[0] << 8) | (data[1]);
  axis.setCurrentP(value *  Z_CURRENT_CC);
  return axis;
}

/*! \brief Read the 3V3 power supply current 
 *
 * I2C will read 2 bytes and will calculate the current using the conversion constant
 * \return Return 3V3 power supply current 
 */
float CEPS::read3V3Current(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_3V3_CURRENT,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  float current = value *  P3V3_CURRENT_CC;
  return current;
}

/*! \brief Read the 5V power supply current 
 *
 * I2C will read 2 bytes and will calculate the current using the conversion constant
 * \return Return 5V power supply current 
 */
float CEPS::read5VCurrent(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_3V3_CURRENT,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  float current = value *  P5V_CURRENT_CC;
  return current;
}

/*! \brief Read 3V3 Latch-up protected output status
 *
 *  This function will return current 3V3 Latch-up protected output status (ON\OFF);
 *
 * \return Functions returns current status (True or False) 
 */
uint16_t CEPS::readLUP3V3(void)
{

  uint8_t data[2]; 
  readI2C(EPS_RCMD_LUP_3V3,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  return value;
}

/*! \brief Read 5V Latch-up protected output status
 *
 *  This function will return current 5V Latch-up protected output status (ON\OFF);
 *
 * \return Functions returns current status (True or False) 
 */
uint16_t CEPS::readLUP5V(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_LUP_5V,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  return value;
}

/*! \brief Read MCU temperature
 *
 *   This function will read the current MCU temperature.
 *
 * \return Return the current MCU temperature. 
 */
float CEPS::readMCUTemp(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_MCU_TEMP,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  float temp = ((value *  MCU_TEMPERATURE_CC) - 0.986)/0.00355;
  return temp;
}

/*! \brief Read battery temperature sensors
 *
 *  Battery Cell has 4 temperature sensors. This function will read all 4 temperature 
 *  sensors and store this values in variable of type TEMPERATURE_INFO. TEMPERATURE_INFO class 
 *  can store up to 4 temperature values.
 *
 * \return Return temperature reading stored in the variable of type TEMPERATURE_INFO 
 */
TEMPERATURE_INFO CEPS::readBatteryTemperature(void)
{
  TEMPERATURE_INFO temp;
  uint8_t data[2]; 

  readI2C(EPS_RCMD_BAT_TEMP_SENS1,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  temp.setTemp1(value *  BATTERY_TEMPERATURE_CC);

  readI2C(EPS_RCMD_BAT_TEMP_SENS2,data,2);
  value =  (data[0] << 8) | (data[1]);
  temp.setTemp2(value *  BATTERY_TEMPERATURE_CC);

  readI2C(EPS_RCMD_BAT_TEMP_SENS3,data,2);
  value =  (data[0] << 8) | (data[1]);
  temp.setTemp3(value *  BATTERY_TEMPERATURE_CC);

  readI2C(EPS_RCMD_BAT_TEMP_SENS4,data,2);
  value =  (data[0] << 8) | (data[1]);
  temp.setTemp4(value *  BATTERY_TEMPERATURE_CC);

  return temp;
}

/*! \brief Read the input conditions
 *
 *  Read the input conditions of the EPS devices. More info about the input conditions can be
 *  found in Table 2 of the EPS datasheet.
 *
 * \return Return input conditions 
 */
uint16_t CEPS::readInputConditions(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_INPUT_CONDITION,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  return value;
}

/*! \brief Read the output conditions
 *
 *  Read the output conditions of the EPS devices. More info about the output conditions can be
 *  found in Table 3 of the EPS datasheet.
 *
 * \return Return output conditions 
 */
uint16_t CEPS::readOutputConditions(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_OUTPUTS_CONDITIONS,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  return value;
}

/*! \brief Read the output conditions
 *
 *  Read the output conditions of the EPS devices. More info about the output conditions can be
 *  found in Table 4 of the EPS datasheet.
 *
 * \return Return output conditions 
 */
uint16_t CEPS::readOutputConditions2(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_OUTPUTS_CONDITIONS2,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  return value;
}

/*! \brief Get number of power ON Cycles
 *
 *  Read how many times power on cycle occured.
 *
 * \return Return number of powe ON Cycles
 */
uint16_t CEPS::readPowerONCycles(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_PWR_ON_CYCLE,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  return value;
}

/*! \brief Get number of Under voltages
 *
 *  Read how many times undervoltage occurred.
 *
 * \return Return number of undervoltage conditions
 */
uint16_t CEPS::readVUnderVoltage(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_V_UNDER_VOLTAGE,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  return value;
}

/*! \brief Get number of short-circuit conditions 
 *
 *  Read how many times short-circuit conditions occurred.
 *
 * \return Return number of short-circuit conditions
 */
uint16_t CEPS::readVShortCircuit(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_V_SHORT_CIRCUIT,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  return value;
}

/*! \brief Get number of over-temperature conditions 
 *
 *  Read how many times over-temperature conditions occurred.
 *
 * \return Return number of over-temperature conditions
 */
uint16_t CEPS::readVOverTemperature(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_V_OVER_TEMPERATURE,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  return value;
}

/*! \brief Read battery pack MAX temperature 
 *
 *  This function will read MAX temperature for 4 temperatures senesors which are located at 
 *  the battery pack.
 *
 * \return Return MAX temperatures for each temperature sensor. This values are stored in the variable of type TEMPERATURE_INFO 
 */
TEMPERATURE_INFO CEPS::readMaxTemperature(void)
{
  TEMPERATURE_INFO temp;
  uint8_t data[2]; 
  readI2C(EPS_RCMD_MAX_TEMP1,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  temp.setTemp1(value *  MAX_TEMPERATURE_CC);

  readI2C(EPS_RCMD_MAX_TEMP2,data,2);
  value =  (data[0] << 8) | (data[1]);
  temp.setTemp2(value *  MAX_TEMPERATURE_CC);

  readI2C(EPS_RCMD_MAX_TEMP3,data,2);
  value =  (data[0] << 8) | (data[1]);
  temp.setTemp3(value *  MAX_TEMPERATURE_CC);

  readI2C(EPS_RCMD_MAX_TEMP4,data,2);
  value =  (data[0] << 8) | (data[1]);
  temp.setTemp4(value *  MAX_TEMPERATURE_CC);

  return temp;

}

/*! \brief Read battery pack MIN temperature 
 *
 *  This function will read MIN temperature for 4 temperatures senesors which are located at 
 *  the battery pack.
 *
 * \return Return MIN temperatures for each temperature sensor. This values are stored in the variable of type TEMPERATURE_INFO 
 */
TEMPERATURE_INFO CEPS::readMinTemperature(void)
{
  TEMPERATURE_INFO temp;
  uint8_t data[2]; 
  readI2C(EPS_RCMD_MIN_TEMP1,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  temp.setTemp1(value *  MIN_TEMPERATURE_CC);

  readI2C(EPS_RCMD_MIN_TEMP2,data,2);
  value =  (data[0] << 8) | (data[1]);
  temp.setTemp2(value *  MIN_TEMPERATURE_CC);

  readI2C(EPS_RCMD_MIN_TEMP3,data,2);
  value =  (data[0] << 8) | (data[1]);
  temp.setTemp3(value *  MIN_TEMPERATURE_CC);

  readI2C(EPS_RCMD_MIN_TEMP4,data,2);
  value =  (data[0] << 8) | (data[1]);
  temp.setTemp4(value *  MIN_TEMPERATURE_CC);

  return temp;
}

/*! \brief Read external temperature sensors
 *
 *  EPS unit has 3 external temperature sensors + 1 reserved sensor dedicated for the future use. This function will read all 3 temperature sensors
 *  and store this values in variable of type TEMPERATURE_INFO. TEMPERATURE_INFO class 
 *  can store up to 4 temperature values.
 *
 * \return Return temperature reading stored in the variable of type TEMPERATURE_INFO 
 */
TEMPERATURE_INFO CEPS::readTemperatureSensor(void)
{
  TEMPERATURE_INFO temp;
  uint8_t data[2]; 

  readI2C(EPS_RCMD_TEMP_SENSOR_5,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  temp.setTemp1(value *  EXT_TEMPERATURE_CC);

  readI2C(EPS_RCMD_TEMP_SENSOR_6,data,2);
  value =  (data[0] << 8) | (data[1]);
  temp.setTemp2(value *  EXT_TEMPERATURE_CC);

  readI2C(EPS_RCMD_TEMP_SENSOR_7,data,2);
  value =  (data[0] << 8) | (data[1]);
  temp.setTemp3(value *  EXT_TEMPERATURE_CC);

  //left for the futur use
  temp.setTemp4(0x00);

  return temp;

}

/*! \brief Read EPS Software version
 *
 *  This functions read the current EPS software version.
 *
 * \return  Return the current software version.
 */
uint16_t CEPS::readSoftwareVersion(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_SOFTWARE_VERSION,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  return value;
}

/*! \brief Read the Defaults values of LUPs and Fast charge modes
 *
 *  This function will read Defaults values of LUPs and Fast charge modes. See table 5 of the EPS datasheet.
 *
 * \return Return Defaults values of Lups and Fast charge modes
 */
uint16_t CEPS::readDefaults1(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_DEFAULTS1,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  return value;
}

/*! \brief Read the Defaults values of Output 1 to 6 
 *
 *  This function will read Defaults values of Output 1 to 6. See table 6 of the EPS datasheet.
 *
 * \return Return Defaults values of Output 1 to 6 
 */
uint16_t CEPS::readDefaults12(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_DEFAULTS2,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  return value;
}

/*! \brief Read number of charging cycles
 *
 *  Read the total number of charging cycles of the EPS device
 *
 * \return Return number of charging cycles 
 */
uint16_t CEPS::readChargeCycles(void)
{
  uint8_t data[2]; 
  readI2C(EPS_RCMD_CHARGE_CYCLES,data,2);
  uint16_t value =  (data[0] << 8) | (data[1]);
  return value;
}

/*! \brief Configure the SW Selflock 
 *
 * \param state Configure the state for the selflock 
 *      - #EPS_CONFIG_STATE_OFF
 *      - #EPS_CONFIG_STATE_ON
 * \return No return value 
 */
void CEPS::writeSWSelflock(uint8_t state)
{
  writeI2C(EPS_WCMD_SW_SELFLOCK,&state,1);
  return;
}

/*! \brief Enable Battery bus 
 *
 * \param state Configure the state for Battery bus
 *      - #EPS_CONFIG_STATE_OFF
 *      - #EPS_CONFIG_STATE_ON
 * \return No return value 
 */
void CEPS::writeVBATEN(uint8_t state)
{
  writeI2C(EPS_WCMD_VBATEN,&state,1);
  return;
}

/*! \brief Enable BCR bus 
 *
 * \param state Configure the state for BCS bus
 *      - #EPS_CONFIG_STATE_OFF
 *      - #EPS_CONFIG_STATE_ON
 * \return No return value 
 */
void CEPS::writeBCROutEN(uint8_t state)
{
  writeI2C(EPS_WCMD_BCROUTEN,&state,1);
  return;
}

/*! \brief Enable 3V3 bus 
 *
 * \param state Configure the state for 3V3 bus
 *      - #EPS_CONFIG_STATE_OFF
 *      - #EPS_CONFIG_STATE_ON
 * \return No return value 
 */
void CEPS::writeSHD3V3(uint8_t state)
{
  writeI2C(EPS_WCMD_SHD3V3,&state,1);
  return;
}

/*! \brief Enable 5V bus 
 *
 * \param state Configure the state for 5V bus
 *      - #EPS_CONFIG_STATE_OFF
 *      - #EPS_CONFIG_STATE_ON
 * \return No return value 
 */
void CEPS::write5V(uint8_t state)
{
  writeI2C(EPS_WCMD_SHD5V,&state,1);
  return;
}

/*! \brief Enable LUP3V3 
 *
 * \param state Configure the state for LUP3V3
 *      - #EPS_CONFIG_STATE_OFF
 *      - #EPS_CONFIG_STATE_ON
 * \return No return value 
 */
void CEPS::writeLUP3V3(uint8_t state)
{
  writeI2C(EPS_WCMD_LUP3V3,&state,1);
  return;
}

/*! \brief Enable LUP5V 
 *
 * \param state Configure the state for LUP5V
 *      - #EPS_CONFIG_STATE_OFF
 *      - #EPS_CONFIG_STATE_ON
 * \return No return value 
 */
void CEPS::writeLUP5V(uint8_t state)
{
  writeI2C(EPS_WCMD_LUP5V,&state,1);
  return;
}

/*! \brief Enable/Disable battery charger 
 *
 * \param state Configure the state for battery charger 
 *      - #EPS_CONFIG_STATE_OFF
 *      - #EPS_CONFIG_STATE_ON
 * \return No return value 
 */
void CEPS::writeSHDChrg(uint8_t state)
{
  writeI2C(EPS_WCMD_SHDCHRG,&state,1);
  return;
}

/*! \brief  Configure charger option 
 *
 * \param state Configure the battery charger option 
 *      - #EPS_CONFIG_CHARGE_FAST
 *      - #EPS_CONFIG_CHARGE_SLOW
 * \return No return value 
 */
void CEPS::writeChrgI1(uint8_t state)
{
  writeI2C(EPS_WCMD_CHRG_I1,&state,1);
  return;
}

/*! \brief  Configure charger option 
 *
 * \param state Configure the battery charger option 
 *      - #EPS_CONFIG_CHARGE_FAST
 *      - #EPS_CONFIG_CHARGE_SLOW
 * \return No return value 
 */
void CEPS::writeChrgI2(uint8_t state)
{
  writeI2C(EPS_WCMD_CHRG_I2,&state,1);
  return;
}

/*! \brief Configure output 
 *
 * \param state Configure the state for the output 
 *      - #EPS_OUTPUT_STATE_AUTO_OFF
 *      - #EPS_OUTPUT_STATE_AUTO_ON
 *      - #EPS_OUTPUT_STATE_FORCED_OFF
 *      - #EPS_OUTPUT_STATE_FORCED_ON
 * \return No return value 
 */
void CEPS::writeOut1(uint8_t state)
{
  writeI2C(EPS_WCMD_OUT1,&state,1);
  return;
}

/*! \brief Configure output 
 *
 * \param state Configure the state for the output 
 *      - #EPS_OUTPUT_STATE_AUTO_OFF
 *      - #EPS_OUTPUT_STATE_AUTO_ON
 *      - #EPS_OUTPUT_STATE_FORCED_OFF
 *      - #EPS_OUTPUT_STATE_FORCED_ON
 * \return No return value 
 */
void CEPS::writeOut2(uint8_t state)
{
  writeI2C(EPS_WCMD_OUT2,&state,1);
  return;

}

/*! \brief Configure output 
 *
 * \param state Configure the state for the output 
 *      - #EPS_OUTPUT_STATE_AUTO_OFF
 *      - #EPS_OUTPUT_STATE_AUTO_ON
 *      - #EPS_OUTPUT_STATE_FORCED_OFF
 *      - #EPS_OUTPUT_STATE_FORCED_ON
 * \return No return value 
 */
void CEPS::writeOut3(uint8_t state)
{
  writeI2C(EPS_WCMD_OUT3,&state,1);
  return;
}

/*! \brief Configure output 
 *
 * \param state Configure the state for the output 
 *      - #EPS_OUTPUT_STATE_AUTO_OFF
 *      - #EPS_OUTPUT_STATE_AUTO_ON
 *      - #EPS_OUTPUT_STATE_FORCED_OFF
 *      - #EPS_OUTPUT_STATE_FORCED_ON
 * \return No return value 
 */
void CEPS::writeOut4(uint8_t state)
{
  writeI2C(EPS_WCMD_OUT4,&state,1);
  return;
}

/*! \brief Configure output 
 *
 * \param state Configure the state for the output 
 *      - #EPS_OUTPUT_STATE_AUTO_OFF
 *      - #EPS_OUTPUT_STATE_AUTO_ON
 *      - #EPS_OUTPUT_STATE_FORCED_OFF
 *      - #EPS_OUTPUT_STATE_FORCED_ON
 * \return No return value 
 */
void CEPS::writeOut5(uint8_t state)
{
  writeI2C(EPS_WCMD_OUT5,&state,1);
  return;
}

/*! \brief Configure output 
 *
 * \param state Configure the state for the output 
 *      - #EPS_OUTPUT_STATE_AUTO_OFF
 *      - #EPS_OUTPUT_STATE_AUTO_ON
 *      - #EPS_OUTPUT_STATE_FORCED_OFF
 *      - #EPS_OUTPUT_STATE_FORCED_ON
 * \return No return value 
 */
void CEPS::writeOut6(uint8_t state)
{
  writeI2C(EPS_WCMD_OUT6,&state,1);
  return;
}

/*! \brief Configure battery heater 
 *
 * \param state Configure the state for the output 
 *      - #EPS_OUTPUT_STATE_AUTO_OFF
 *      - #EPS_OUTPUT_STATE_AUTO_ON
 *      - #EPS_OUTPUT_STATE_FORCED_OFF
 *      - #EPS_OUTPUT_STATE_FORCED_ON
 * \return No return value 
 */
void CEPS::writeHeater1(uint8_t state)
{
  writeI2C(EPS_WCMD_HEATER1,&state,1);
  return;
}

/*! \brief Configure battery heater 
 *
 * \param state Configure the state for the output 
 *      - #EPS_OUTPUT_STATE_AUTO_OFF
 *      - #EPS_OUTPUT_STATE_AUTO_ON
 *      - #EPS_OUTPUT_STATE_FORCED_OFF
 *      - #EPS_OUTPUT_STATE_FORCED_ON
 * \return No return value 
 */
void CEPS::writeHeater2(uint8_t state)
{
  writeI2C(EPS_WCMD_HEATER2,&state,1);
  return;
}

/*! \brief Configure battery heater 
 *
 * \param state Configure the state for the output 
 *      - #EPS_OUTPUT_STATE_AUTO_OFF
 *      - #EPS_OUTPUT_STATE_AUTO_ON
 *      - #EPS_OUTPUT_STATE_FORCED_OFF
 *      - #EPS_OUTPUT_STATE_FORCED_ON
 * \return No return value 
 */
void CEPS::writeHeater3(uint8_t state)
{
  writeI2C(EPS_WCMD_HEATER3,&state,1);
  return;
}

/*! \brief Set voltage for the given axis 
 *
 * \param voltage_ Voltage value 
 * 
 * \return No return value 
 */
void AXIS_INFO::setVoltage(float voltage_)
{
  voltage = voltage_;
}

/*! \brief Set current- for the given axis 
 *
 * \param current_m_ Set the current- value
 * 
 * \return No return value 
 */
void AXIS_INFO::setCurrentM(float current_m_)
{
  current_m = current_m_;
}

/*! \brief Set current+ for the given axis 
 *
 * \param current_p_ Set the current- value
 * 
 * \return No return value 
 */
void AXIS_INFO::setCurrentP(float current_p_)
{
  current_p = current_p_;
}

/*! \brief Read voltage value for the given axis 
 *
 * \return Voltage value 
 */
float AXIS_INFO::readVoltage(void)
{
  return voltage;

}

/*! \brief Read current- value for the given axis 
 *
 * \return current- value 
 */
float AXIS_INFO::readCurrentM(void)
{
  return current_m;

}

/*! \brief Read current+ value for the given axis 
 *
 * \return current+ value 
 */
float AXIS_INFO::readCurrentP(void)
{
  return current_p;
}

/*! \brief Set temperature temp1 
 *
 * \param voltage_ Temperature value  
 * 
 * \return No return value 
 */
void TEMPERATURE_INFO::setTemp1(float temp1_)
{
  temp1 = temp1_;
}

/*! \brief Set temperature temp2 
 *
 * \param voltage_ Temperature value  
 * 
 * \return No return value 
 */
void TEMPERATURE_INFO::setTemp2(float temp2_)
{
  temp2 = temp2_;
}

/*! \brief Set temperature temp3 
 *
 * \param voltage_ Temperature value  
 * 
 * \return No return value 
 */
void TEMPERATURE_INFO::setTemp3(float temp3_)
{
  temp3 = temp3_;
}

/*! \brief Set temperature temp4 
 *
 * \param voltage_ Temperature value  
 * 
 * \return No return value 
 */
void TEMPERATURE_INFO::setTemp4(float temp4_)
{
  temp4 = temp4_;
}


/*! \brief Read temperature value from the desired sensor
 *
 * \return Temperature value
 */
float TEMPERATURE_INFO::readTemp1(void)
{
  return temp1;
}

/*! \brief Read temperature value from the desired sensor
 *
 * \return Temperature value
 */
float TEMPERATURE_INFO::readTemp2(void)
{
  return temp2;
}

/*! \brief Read temperature value from the desired sensor
 *
 * \return Temperature value
 */
float TEMPERATURE_INFO::readTemp3(void)
{
  return temp3;
}

/*! \brief Read temperature value from the desired sensor
 *
 * \return Temperature value
 */
float TEMPERATURE_INFO::readTemp4(void)
{
  return temp4;
}

    
CMsg TEMPERATURE_INFO::read(){
  CMsg m;
  m.set(_TEMP1,temp1);
  m.set(_TEMP2,temp2);
  m.set(_TEMP3,temp3);
  m.set(_TEMP4,temp4);

  return m;
 }


CMsg AXIS_INFO::read(){
  CMsg m;

  m.set(_VOLTAGE,voltage);
  m.set(_CURRENTM,current_m);
  m.set(_CURRENTP,current_p);
  return m;
 }


void CEPS::callCustomFunctions(CMsg &msg){
  
  std::string act=msg.get(_ACT);  
  int val=msg.get(_VALUE,0); 
  
	//if((act=="GET")) {getData(msg);  return;}
  if (act=="READBATTERYVOLTAGE") {transmitResult(act,tostring(readBatteryVoltage())); return;}
  if (act=="READBATTERYCRNT") {transmitResult(act,tostring(readBatteryCurrent()));return;}
  if (act=="READBCRVOLTAGE") {transmitResult(act,tostring(readBCRVoltage()));return;}

  if (act=="READBCRCURRENT") {transmitResult(act,tostring(readBCRCurrent()));return;}
  if (act=="READ3V3CURRENT") {transmitResult(act,tostring(read3V3Current()));return;}
  if (act=="READ5VCURRENT") {transmitResult(act,tostring(read5VCurrent()));return;}

  if (act=="READLUP3V3") {transmitResult(act,tostring(readLUP3V3()));return;}
  if (act=="READLUP5V") {transmitResult(act,tostring(readLUP5V()));return;}

  if (act=="READMCUTEMP") {transmitResult(act,tostring(readMCUTemp()));return;}
  if (act=="READINPUTCONDITIONS") {transmitResult(act,tostring(readInputConditions()));return;}
  if (act=="READOUTPUTCONDITIONS") {transmitResult(act,tostring(readOutputConditions()));return;}
  if (act=="READOUTPUTCONDITIONS2") {transmitResult(act,tostring(readOutputConditions2()));return;}
  
  if (act=="READPOWERONCYCLES") {transmitResult(act,tostring(readPowerONCycles()));return;}
  if (act=="READVUNDERVOLT") {transmitResult(act,tostring(readVUnderVoltage()));return;}
  if (act=="READVSHORTCIRCUIT") {transmitResult(act,tostring(readVShortCircuit()));return;}
  if (act=="READVOVERTEMP") {transmitResult(act,tostring(readVOverTemperature()));return;}
  if (act=="READSOFTWAREVERSION") {transmitResult(act,tostring(readSoftwareVersion()));return;}
  
  if (act=="READDEAFAULTS1") {transmitResult(act,tostring(readDefaults1()));return;}
  if (act=="READDEAFAULTS12") {transmitResult(act,tostring(readDefaults12()));return;}
  if (act=="READCHARGECYCLES") {transmitResult(act,tostring(readChargeCycles()));return;}


  if (act=="WRITESWSELFLOCK") {writeSWSelflock(val);return;}

  if (act=="WRITEVBATEN") {writeVBATEN(val);return;}
  if (act=="WRITEBCROUTEN") {writeBCROutEN(val);return;}
  if (act=="WRITESHD3V3") {writeSHD3V3(val);return;}
  if (act=="WRITE5V") {write5V(val);return;}
  if (act=="WRITELUP3V3") {writeLUP3V3(val);return;}
  if (act=="WRITELUP5V") {writeLUP5V(val);return;}
  if (act=="WRITESHDCHRG") {writeSHDChrg(val);return;}
  if (act=="WRITECHRGI1") {writeChrgI1(val);return;}
  if (act=="WRITECHRGI2") {writeChrgI2(val);return;}
  if (act=="WRITEOUT1") {writeOut1(val);return;}
  if (act=="WRITEOUT2") {writeOut2(val);return;}
  if (act=="WRITEOUT3") {writeOut3(val);return;}
  if (act=="WRITEOUT4") {writeOut4(val);return;}
  if (act=="WRITEOUT5") {writeOut5(val);return;}
  if (act=="WRITEOUT6") {writeOut6(val);return;}
  if (act=="WRITEHEATER1") {writeHeater1(val);return;}
  if (act=="WRITEHEATER2") {writeHeater2(val);return;}
  if (act=="WRITEHEATER3") {writeHeater3(val);return;}

  if (act=="READXAXISINFO") {transmitResult(act,readXAxisInfo().read());return;}
  if (act=="READYAXISINFO") {transmitResult(act,readYAxisInfo().read());return;}
  if (act=="READZAXISINFO") {transmitResult(act,readZAxisInfo().read());return;}

  if (act=="READBATTERYTEMPERATURE") {transmitResult(act,readBatteryTemperature().read());return;}
  if (act=="READMAXTEMPERATURE") {transmitResult(act,readMaxTemperature().read());return;}
  if (act=="READMINTEMPERATURE") {transmitResult(act,readMinTemperature().read());return;}
  if (act=="READTEMPERATURESENSOR") {transmitResult(act,readTemperatureSensor().read());return;}

  CSystemObject::callCustomFunctions(msg);  
}