#pragma once

#if !defined(_DEFS_H)
#define _DEFS_H


#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
    #define PORTENTA    

#else
    #define TTGO
#endif
//#define MYESP32
//#define ESP32_GATEWAY




#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
#define PORTENTA
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

#define P_I2C0  PH_7 
#define P_I2C0  PH_8 
#define P_I2C1  PB_6 
#define P_I2C1  PB_7 
#define P_I2C2  PH_11  
#define P_I2C2  PH_12  
  
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

/*
#define RADIO1_NSS PinNameToIndex(PC_13)
#define RADIO1_BUSY PinNameToIndex(PD_4)
#define RADIO1_RST PinNameToIndex(PC_15)
#define RADIO1_DIO1 PinNameToIndex(PE_3)
*/
#define RADIO1_DIO2 PD_5
#define RADIO1_TX PG_3
#define RADIO1_RX PG_10

/*
#define RADIO2_NSS PinNameToIndex(PI_4)
#define RADIO2_BUSY PinNameToIndex(PH_10)
#define RADIO2_RST PinNameToIndex(PH_14)
#define RADIO2_DIO1 PinNameToIndex(PI_5)
*/
#define RADIO2_DIO2 PH_9
#define RADIO2_TX PA_6
#define RADIO2_RX PA_4

#define PHONE_ENABLE PA_11


#define MAG_ENABLE PH_15

#define SENSORS_ENABLE  PA_12

#define BURN_WIRE PI_10

/*
#define IMU_OBC_NSS PinNameToIndex(PB_4)
#define IMU_OBC_RST PinNameToIndex(PI_14)
#define IMU_OBC_INT PinNameToIndex(PI_15)
#define IMU_OBC_WAKE PinNameToIndex(PB_3)
*/

#define MOTOR_X_FG PJ_10
#define MOTOR_X_SPEED PA_8
#define MOTOR_X_DIR PC_6

#define MOTOR_Y_FG PI_7
#define MOTOR_Y_SPEED PC_7
#define MOTOR_Y_DIR PJ_7

#define MOTOR_Z_FG PI_6
#define MOTOR_Z_SPEED PH_6
#define MOTOR_Z_DIR PK_1

#define CAN1_TX PH_13
#define CAN1_RX PB_8



#define R1_TXEN PG_3
#define R1_RXEN PG_10

#define R1_TXEN PG_3
#define R1_RXEN PG_10
#define R1_NSS PC_13
#define R1_DIO1 PE_3
#define R1_NRST PC_15
#define R1_BUSY PD_4

#define R2_TXEN PA_6
#define R2_RXEN PA_4
#define R2_NSS PI_4
#define R2_DIO1 PI_5
#define R2_NRST PH_14
#define R2_BUSY PH_10

#define PORT_I2C_SDA2 PH_12
#define PORT_I2C_SCL2 PH_11

#define PIN_PANELS PA_12
#define PIN_MAGNETS PH_15
#define PIN_ADCS PI_0
#define MOSI_PIN   PC_3
#define MISO_PIN   PC_2
#define SCLK_PIN   PI_1
 

#define R3_RX_ENABLE PG_3
#define R3_TX_ENABLE PD_5


#define IMU_CS PJ_8
#define IMU_WAKE PJ_9
#define IMU_INT PI_15
#define IMU_RST PI_14


#define RW_SIG_X  PA_8
#define RW_FG_X  PJ_10
#define RW_DIR_X  PC_6

#define RW_SIG_Y  PC_7
#define RW_FG_Y  PI_7
#define RW_DIR_Y  PJ_7

#define RW_SIG_Z  PH_6
#define RW_FG_Z PI_6 
#define RW_DIR_Z  PK_1

#define PWM_SIG PK_1

#else

#define RW_SIG_X  2
#define RW_FG_X  14
#define RW_DIR_X  13

#define RW_SIG_Y  25
#define RW_FG_Y  33
#define RW_DIR_Y  32

#define RW_SIG_Z  35
#define RW_FG_Z 15 
#define RW_DIR_Z  4

#define PWM_SIG 4
#endif


#define TRANSMITMSGLENGTH 220
#define PRINT_ASCIIART
  
#define IMUADDRESS1  0x4B
#define IMUADDRESS2  0x4A

#define IRARRAY_ADDRESS_X1  0x34
#define IRARRAY_ADDRESS_X2  0x33
#define IRARRAY_ADDRESS_Y1  0x34
#define IRARRAY_ADDRESS_Y2  0x33
#define IRARRAY_ADDRESS_Z1  0x34
#define IRARRAY_ADDRESS_Z2  0x33
#define TEMP_X1 0x33
#define TEMP_X2 0x33
#define TEMP_Y1 0x33
#define TEMP_Y2 0x33
#define TEMP_Z1 0x33
#define TEMP_Z2 0x33
#define TEMP_OBC 0x33
#define TEMP_MOTORS 0x33


#define MAG_ADDRESS_X  0x43
#define MAG_ADDRESS_Y  0x43
#define MAG_ADDRESS_Z  0x43

#define PING_INTERVAL             2       // seconds, how much time to wait between PINGs sent from this node
#define FREQUENCY_900M30S         915.0   // MHz carrier, for E22-900M30S
#define FREQUENCY_400M30S         433.0   // MHz carrier, for E22-400M30S
#define FREQUENCY_GATEWAY         433.0   // MHz carrier, for ESP32 single-channel gateway
#define BANDWIDTH                 500  //125.0   // kHz dual-sideband
#define SPREADING_FACTOR          9       // 2^9 chips
#define CODING_RATE               7       // 4/7 coding
#define SYNC_WORD                 0x12    // private network
#define OUTPUT_POWER              14      // +14 dBm
#define CURRENT_LIMIT             120.0    // mA  was 60
#define PREAMBLE_LENGTH           8       // symbols
#define TCXO_VOLTAGE              2.4     // V


#define LORAFREQUENCY 433.0   //FREQUENCY_900M30S


#define ESP32_GATEWAY_NSS_BUILTIN               16
#define ESP32_GATEWAY_DIO0_BUILTIN              26
#define ESP32_GATEWAY_NRST_BUILTIN             -1// RADIOLIB_PIN_UNUSED
#define ESP32_GATEWAY_DIO1_BUILTIN              33


#define TTGO_SCK        5
#define TTGO_MISO       19
#define TTGO_MOSI       27
#define TTGO_SS         18
#define TTGO_DIO0       26
#define TTGO_DIO1       33
#define TTGO_DIO2       32
#define TTGO_RST        23

#define TTGO_I2C_SDA         21
#define TTGO_I2C_SCL         22

//const byte MLX90640_address = 0x33; //Default 7-bit unshifted address of the MLX90640
#define TA_SHIFT 8 //Default shift for MLX90640 in open air



//Radio1MHz
#define R1_NSS 25
#define R1_DIO1 26
#define R1_NRST 27
#define R1_BUSY 28

//Radio2MHz
#define R2_NSS 29
#define R2_DIO1 30
#define R2_NRST 31
#define R2_BUSY 32

//RadioGHz
#define R3_NSS 33
#define R3_DIO1 34
#define R3_NRST 35
#define R3_BUSY 36

 
 

/*
 * 00:38:32.536 -> Scanning...
00:38:32.536 -> I2C device found at address 0x49  !
00:38:32.536 -> I2C device found at address 0x69  !
00:38:32.536 -> done
00:38:32.536 -> 
00:38:32.536 -> Wire1
00:38:32.536 -> Scanning...
00:38:32.536 -> I2C device found at address 0x08  !
00:38:32.536 -> I2C device found at address 0x49  !
00:38:32.582 -> I2C device found at address 0x60  !
00:38:32.582 -> I2C device found at address 0x69  !
00:38:32.582 -> done
00:38:32.582 -> 
00:38:32.582 -> Wire2
00:38:32.582 -> Scanning...
00:38:32.582 -> I2C device found at address 0x48  !
00:38:32.582 -> I2C device found at address 0x49  !
00:38:32.582 -> I2C device found at address 0x4A  !
00:38:32.582 -> I2C device found at address 0x4B  !
00:38:32.582 -> I2C device found at address 0x60  !
00:38:32.582 -> I2C device found at address 0x61  !
00:38:32.582 -> I2C device found at address 0x63  !
00:38:32.582 -> I2C device found at address 0x68  !
00:38:32.582 -> I2C device found at address 0x69  
 */




#define ADDRESS_IMU1     0x4B
#define ADDRESS_TEMPOBC  0x4A


#define ADDRESS_IMU2     0x4B
#define ADDRESS_TEMPADCS  0x4A

#define ADDRESS_MAGX     0x60
#define ADDRESS_MAGY     0x61
#define ADDRESS_MAGZ     0x63


#define ADDRESS_PORTENTA     0x08
#define ADDRESS_PORTENTA1     0x60

#define ADDRESS_IMU   0x4B


#define ADDRESS_IRARRAY    0x49
#define ADDRESS_IRARRAY1    0x48

#define ADDRESS_TEMP    0x69
#define ADDRESS_TEMP1   0x68
#define ADDRESS_TEMP2    0x4A


#define ADDRESS_RW_X
#define ADDRESS_RW_X_PWM
#define ADDRESS_RW_X_DIR
#define ADDRESS_RW_X_HALL


#define ADDRESS_RW_Y
#define ADDRESS_RW_Y_PWM
#define ADDRESS_RW_Y_DIR
#define ADDRESS_RW_Y_HALL


#define ADDRESS_RW_Z
#define ADDRESS_RW_Z_PWM
#define ADDRESS_RW_Z_DIR
#define ADDRESS_RW_Z_HALL


#define PHONE_BAUD_RATE 115200
#define PHONE_TX 23
#define PHONE_RX 4


#endif
