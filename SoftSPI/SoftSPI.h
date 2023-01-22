
#ifndef _SOFTSPI_H
#define _SOFTSPI_H
#include <Arduino.h>
#include <SPI.h>
#include "config.h"

class SoftSPI : public SPIClass {
private:

void wait(void);
private:
uint8_t _cke;
uint8_t _ckp;
uint8_t _delay;
uint8_t _miso;
uint8_t _mosi;
uint8_t _sck;
uint8_t _order;
public:
SoftSPI(uint8_t mosi, uint8_t miso, uint8_t sck);
virtual void begin();
virtual void end();
void setBitOrder(uint8_t);
void setDataMode(uint8_t);
void setClockDivider(uint8_t);
virtual uint8_t transfer(uint8_t);
virtual void transfer(void*, size_t);
virtual void usingInterrupt(int);
virtual void notUsingInterrupt(int);
virtual void attachInterrupt();
virtual void detachInterrupt();
uint16_t transfer16(uint16_t data);
virtual void beginTransaction(SPISettings settings);
virtual void endTransaction(void);
};
#endif
