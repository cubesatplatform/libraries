#pragma once

#include <Arduino.h>

#define POWERKEY  "POWERKEY"

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
#else    
    #define TTGO_I2C_SDA         21
    #define TTGO_I2C_SCL         22

    //#define MYESP32
    //#define ESP32_GATEWAY    
#endif


#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
    //#define PORTENTA_E22_900M30S
    #define PORTENTA_E22_400M30S

    //https://github.com/arduino/ArduinoCore-API/blob/master/api/Common.h
    //https://github.com/arduino/ArduinoCore-mbed/blob/master/variants/PORTENTA_H7_M7/mbed_config.h
    //#include <common.h>
    //#include <mbed_config.h>
    //#include <pinDefinitions.h>

    #define P_GPIO0 PC_13
    #define P_GPIO1 PC_15
    #define P_GPIO2 PD_4
    #define P_GPIO3 PD_5
    #define P_GPIO4 PE_3
    #define P_GPIO5 PG_3
    #define P_GPIO6 PG_10
    
    #define P_PWM1  PA_8      //Breakout board goes from 0-9
    #define P_PWM2  PC_6
    #define P_PWM3  PC_7
    #define P_PWM5  PJ_11
    #define P_PWM6  PK_1
    #define P_PWM7  PH_15
    #define P_PWM8  PJ_7
    #define P_PWM9  PJ_10
    #define P_PWM10 PH_6

    //#define P_ADC_A0  PA0_C
    //#define P_ADC_A1  PA1_C
    //#define P_ADC_A2  PC2_C
    //#define P_ADC_A3  PC3_C
    #define P_ADC_A4  PC_2
    #define P_ADC_A5  PC_3
    #define P_ADC_A6  PA_4
    #define P_ADC_A7  PA_6
    
    #define P_CAM_D3_P PI_7
    #define P_CAM_D3_N PI_6
    #define P_CAM_D2_P PI_4
    #define P_CAM_D2_N PH_14
    #define P_CAM_D1_P PH_12
    #define P_CAM_D1_N PH_11
    #define P_CAM_D0_P PH_10
    #define P_CAM_D0_N PH_9
    #define P_CAM_CK_P PI_5
    #define P_CAM_CK_N PA_6
    #define P_CAM_HS PA_4
        
    #define P_UART0_RTS PI_10
    #define P_UART0_CTS PI_13
    
    #define P_UART1_RTS PI_14
    #define P_UART1_CTS PI_15
    
    #define P_UART0_TX  PA_0
    #define P_UART0_RX  PI_9
    
    #define P_UART1_TX  PA_9
    #define P_UART1_RX  PA_10
    
    #define P_UART2_TX  PG_14
    #define P_UART2_RX  PG_9
    
    #define P_UART3_TX  PJ_8
    #define P_UART3_RX  PJ_9

    #define P_SPI1_MOSI PC_3 
    #define P_SPI1_MISO PC_2 
    #define P_SPI1_SCK PI_1 

    #define P_I2C0_SCL  PH_7 
    #define P_I2C0_SDA  PH_8 
    #define P_I2C1_SCL  PB_6 
    #define P_I2C1_SDA  PB_7 
    #define P_I2C2_SCL  PH_11  
    #define P_I2C2_SDA  PH_12  
    
    #define P_UART0_TX  PA_0
    #define P_UART0_RX  PI_9
    #define P_UART1_TX  PA_9 
    #define P_UART1_RX  PA_10
    #define P_UART2_TX  PG_14  
    #define P_UART2_RX  PG_9
    #define P_UART3_TX  PJ_8
    #define P_UART3_RX  PJ_9

    #define P_USB0DMINUS PA_11
    #define P_USB0DPLUS PA_12

    #define CAN1_TX PH_13
    #define CAN1_RX PB_8

    #define PIN_PANELS PA_12
    #define PIN_MAGNETS PH_15
    #define PIN_ADCS PI_0
    #define MOSI_PIN   PC_3
    #define MISO_PIN   PC_2
    #define SCLK_PIN   PI_1
    
    #define R3_RX_ENABLE PG_3
    #define R3_TX_ENABLE PD_5

#endif


  

