/*
 * I2C address change for MLX90640
 */


#include <Wire.h>
#include <defs.h>

#define I2C_BUFFER_LENGTH 128
#define DEBUG 1


// ==================
// SETUP
// ==================
void setup() {
  digitalWrite(SENSORS_ENABLE, HIGH);  
    delay(200);
  #if defined(DEBUG)
    Serial.begin(115200);
    while (!Serial){
    }
    delay(44);
  #endif
  
  Wire1.begin();
  Wire1.setClock(400000);

  delay(10);

  uint16_t readData = 0;

  // read actual address value from the sensor with 0x33 address
  // should be 0x33 (0xBE33)
  int result =  MLX90640_I2CRead(0x33, 0x240F, 2, &readData);
  #ifdef DEBUG
  Serial.print("result: ");
  Serial.println(result);
  Serial.println("----");
  Serial.print("I2C address BE33: ");
  Serial.println(readData, HEX);
  #endif

  delay(100);

  // write 0x00 to erase the address register
  result = MLX90640_I2CWrite(0x33, 0x240F, 0);
  #ifdef DEBUG
  Serial.print("result: ");
  Serial.println(result);
  Serial.println("----");
  #endif

  delay(100);
  
  // read the address regiseter back to see if it was erased
  // should be 0x00
  result =  MLX90640_I2CRead(0x33, 0x240F, 2, &readData);
  #ifdef DEBUG
  Serial.print("result: ");
  Serial.println(result);
  Serial.println("----");
  Serial.print("I2C address 0: ");
  Serial.println(readData, HEX);
  #endif

  // write the new address 0x32 (0xBE32)
  result = MLX90640_I2CWrite(0x33, 0x240F, 0xBE32);
  #ifdef DEBUG
  Serial.print("result: ");
  Serial.println(result);
  Serial.println("----");
  #endif

  delay(100);

  // in this place the power cycle is needed to read data from the device with changed I2C address
  result =  MLX90640_I2CRead(0x32, 0x240F, 2, &readData);
  #ifdef DEBUG
  Serial.print("result: ");
  Serial.println(result);
  Serial.println("----");
  Serial.print("I2C address BE32: ");
  Serial.println(readData, HEX);
  
  Serial.println();
  Serial.println("---- done ----");
  #endif
}


// ==================
// LOOP
// ==================
void loop() {
  // nothing here, everything happens in setup()
}

//Read a number of words from startAddress. Store into Data array.
//Returns 0 if successful, -1 if error
int MLX90640_I2CRead(uint8_t _deviceAddress, uint16_t startAddress, unsigned int nWordsRead, uint16_t *data)
{

  //Caller passes number of 'unsigned ints to read', increase this to 'bytes to read'
  uint16_t bytesRemaining = nWordsRead * 2;

  //It doesn't look like sequential read works. Do we need to re-issue the address command each time?

  uint16_t dataSpot = 0; //Start at beginning of array

  //Setup a series of chunked I2C_BUFFER_LENGTH byte reads
  while (bytesRemaining > 0)
  {
    Wire1.beginTransmission(_deviceAddress);
    Wire1.write(startAddress >> 8); //MSB
    Wire1.write(startAddress & 0xFF); //LSB
    if (Wire1.endTransmission(false) != 0) //Do not release bus
    {
      //Serial.println("No ack read");
      return (0); //Sensor did not ACK
    }

    uint16_t numberOfBytesToRead = bytesRemaining;
    if (numberOfBytesToRead > I2C_BUFFER_LENGTH) numberOfBytesToRead = I2C_BUFFER_LENGTH;

    Wire1.requestFrom((uint8_t)_deviceAddress, numberOfBytesToRead);
    if (Wire1.available())
    {
      for (uint16_t x = 0 ; x < numberOfBytesToRead / 2; x++)
      {
        //Store data into array
        data[dataSpot] = Wire1.read() << 8; //MSB
        data[dataSpot] |= Wire1.read(); //LSB

        dataSpot++;
      }
    }

    bytesRemaining -= numberOfBytesToRead;

    startAddress += numberOfBytesToRead / 2;
  }

  return (0); //Success
}


//Write two bytes to a two byte address
int MLX90640_I2CWrite(uint8_t _deviceAddress, uint16_t writeAddress, uint16_t data)
{
  Wire1.beginTransmission((uint8_t)_deviceAddress);
  Wire1.write(writeAddress >> 8); //MSB
  Wire1.write((writeAddress & 0xFF)); //LSB
  Wire1.write(data >> 8); //MSB
  Wire1.write(data & 0xFF); //LSB
  if (Wire1.endTransmission() != 0)
  {
    //Sensor did not ACK
    //Serial.println("Error: Sensor did not ack");
    return (-1);
  }

  uint16_t dataCheck;
  MLX90640_I2CRead(_deviceAddress, writeAddress, 1, &dataCheck);
  if (dataCheck != data)
  {
    //Serial.println("The write request didn't stick");
    return -2;
  }

  return (0); //Success
}
