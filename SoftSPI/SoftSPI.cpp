#include "SoftSPI.h"

#define SSPI_SCK_PIN 0x0002 // PI1
#define SSPI_MISO_PIN 0x0004 // PC2
#define SSPI_MOSI_PIN 0x0008 // PC3
#define SSPI_SCK_HIGH GPIOI->ODR |= (SSPI_SCK_PIN);
#define SSPI_SCK_LOW GPIOI->ODR &= ~(SSPI_SCK_PIN);
#define SSPI_MOSI_HIGH GPIOC->ODR |= (SSPI_MOSI_PIN);
#define SSPI_MOSI_LOW GPIOC->ODR &= ~(SSPI_MOSI_PIN);


SoftSPI::SoftSPI(uint8_t mosi, uint8_t miso, uint8_t sck) {
_mosi = mosi;
_miso = miso;
_sck = sck;
_delay = 2;
_cke = 0;
_ckp = 0;
_order = MSBFIRST;
//wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
// PI1 - SCK
// PC2 - MISO
// PC3 - MOSI
//--------------------------------------------------------------------------
RCC->AHB4ENR |= RCC_AHB4ENR_GPIOCEN;
__DMB();
GPIOC->MODER &= ~((GPIO_MODER_MODE2)|(GPIO_MODER_MODE3));
GPIOC->MODER |= (GPIO_MODER_MODE3_0);
GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD2);
GPIOC->PUPDR |= (GPIO_PUPDR_PUPD2_0);
RCC->AHB4ENR |= RCC_AHB4ENR_GPIOIEN;
__DMB();
GPIOI->MODER &= ~(GPIO_MODER_MODE1);
GPIOI->MODER |= (GPIO_MODER_MODE1_0);
//wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
// SS22
RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN;
__DMB();
GPIOA->MODER &= ~(GPIO_MODER_MODE6);
GPIOA->MODER |= (GPIO_MODER_MODE6_0);
GPIOA->ODR |= 0x0040;
//wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
// SS23
RCC->AHB4ENR |= RCC_AHB4ENR_GPIOGEN;
__DMB();
GPIOG->MODER &= ~(GPIO_MODER_MODE10);
GPIOG->MODER |= (GPIO_MODER_MODE10_0);
GPIOG->ODR |= 0x0400;
//wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
// SS24
RCC->AHB4ENR |= RCC_AHB4ENR_GPIOIEN;
__DMB();
GPIOI->MODER &= ~(GPIO_MODER_MODE7);
GPIOI->MODER |= (GPIO_MODER_MODE7_0);
GPIOI->ODR |= 0x0080;
//wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
// RST2
RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN;
__DMB();
GPIOA->MODER &= ~(GPIO_MODER_MODE4);
GPIOA->MODER |= (GPIO_MODER_MODE4_0);
//GPIOA->ODR |= 0x0010;
GPIOA->ODR &= ~0x0010;
//wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
// RST3
RCC->AHB4ENR |= RCC_AHB4ENR_GPIOJEN;
__DMB();
GPIOJ->MODER &= ~(GPIO_MODER_MODE7);
GPIOJ->MODER |= (GPIO_MODER_MODE7_0);
//GPIOJ->ODR |= 0x0080;
GPIOJ->ODR &= ~0x0080;
//wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
// RST4
RCC->AHB4ENR |= RCC_AHB4ENR_GPIOIEN;
__DMB();
GPIOI->MODER &= ~(GPIO_MODER_MODE6);
GPIOI->MODER |= (GPIO_MODER_MODE6_0);
//GPIOI->ODR |= 0x0040;
GPIOI->ODR &= ~0x0040;
}

void SoftSPI::begin() {
}

void SoftSPI::end() {
}

void SoftSPI::setBitOrder(uint8_t order) {
_order = order & 1;
}

void SoftSPI::setDataMode(uint8_t mode) {
switch (mode) {
case SPI_MODE0:
_ckp = 0;
_cke = 0;
break;
case SPI_MODE1:
_ckp = 0;
_cke = 1;
break;
case SPI_MODE2:
_ckp = 1;
_cke = 0;
break;
case SPI_MODE3:
_ckp = 1;
_cke = 1;
break;
}
digitalWrite(_sck, _ckp ? HIGH : LOW);
}

void SoftSPI::setClockDivider(uint8_t div)
{
}

void SoftSPI::wait(void) {
asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
//asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
//asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
//asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
//asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
//asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
//asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
//asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
//asm volatile("sev;nop;nop;nop;nop;nop;nop;nop;nop;sev;"); // 8-nops 0.5us
}

uint8_t SoftSPI::transfer(uint8_t data)
{
uint8_t k;
uint8_t rxdata = 0;
SSPI_SCK_LOW;
for(k=0;k<8;k++)
{
SSPI_SCK_LOW;
wait();
if(data & 0x80)
{
SSPI_MOSI_HIGH;
}
else
{
SSPI_MOSI_LOW;
}
data = data<<1;
wait();
SSPI_SCK_HIGH;
wait();
rxdata = rxdata << 1;
if(GPIOC->IDR & (SSPI_MISO_PIN))
{
rxdata |= 0x01;
}
SSPI_SCK_LOW;
}
return rxdata;
}

uint16_t SoftSPI::transfer16(uint16_t data)
{
uint16_t rdata = 0;
rdata = transfer(data>>8)<<8;
rdata |= transfer(data&0x00FF);
return rdata;
}
void SoftSPI::beginTransaction(SPISettings settings)
{
}
void SoftSPI::endTransaction(void)
{
}
void SoftSPI::transfer(void* ptr, size_t size)
{
uint8_t * data = (uint8_t *)ptr;
uint16_t k;
for(k=0;k<size;k++)
{
transfer(data[k]);
}
}
void SoftSPI::usingInterrupt(int)
{
}
void SoftSPI::notUsingInterrupt(int)
{
}
void SoftSPI::attachInterrupt()
{
}
void SoftSPI::detachInterrupt()
{
}
