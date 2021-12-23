

#ifdef PORTENTA
#include <map>
#include "defs.h"

std::map<std::string, PinName> obcPinMap;
void fillOBCMap(){

obcPinMap["P_GPIO0"]=PC_13;
obcPinMap["P_GPIO1"]=PC_15;
obcPinMap["P_GPIO2"]=PD_4;
obcPinMap["P_GPIO3"]=PD_5;
obcPinMap["P_GPIO4"]=PE_3;
obcPinMap["P_GPIO5"]=PG_3;
obcPinMap["P_GPIO6"]=PG_10;
 
obcPinMap["P_PWM1"]=PA_8; //Breakout board goes from 0-9
obcPinMap["P_PWM2"]=PC_6;
obcPinMap["P_PWM3"]=PC_7;
obcPinMap["P_PWM5"]=PJ_11;
obcPinMap["P_PWM6"]=PK_1;
obcPinMap["P_PWM7"]=PH_15;
obcPinMap["P_PWM8"]=PJ_7;
obcPinMap["P_PWM9"]=PJ_10;
obcPinMap["P_PWM10"]=PH_6;

//obcPinMap["P_ADC_A0"]=PA_0_C;
//obcPinMap["P_ADC_A1"]=PA1_C;
//obcPinMap["P_ADC_A2"]=PC_2_C;
//obcPinMap["P_ADC_A3"]=PC_3_C;
obcPinMap["P_ADC_A4"]=PC_2;
obcPinMap["P_ADC_A5"]=PC_3;
obcPinMap["P_ADC_A6"]=PA_4;
obcPinMap["P_ADC_A7"]=PA_6;
 
obcPinMap["P_CAM_D3_P"]=PI_7;
obcPinMap["P_CAM_D3_N"]=PI_6;
obcPinMap["P_CAM_D2_P"]=PI_4;
obcPinMap["P_CAM_D2_N"]=PH_14;
obcPinMap["P_CAM_D1_P"]=PH_12;
obcPinMap["P_CAM_D1_N"]=PH_11;
obcPinMap["P_CAM_D0_P"]=PH_10;
obcPinMap["P_CAM_D0_N"]=PH_9;
obcPinMap["P_CAM_CK_P"]=PI_5;
obcPinMap["P_CAM_CK_N"]=PA_6;
obcPinMap["P_CAM_HS"]=PA_4;
 
obcPinMap["P_UART0_RTS"]=PI_10;
obcPinMap["P_UART0_CTS"]=PI_13;
 
obcPinMap["P_UART1_RTS"]=PI_14;
obcPinMap["P_UART1_CTS"]=PI_15;
 
obcPinMap["P_UART0_TX"]=PA_0;
obcPinMap["P_UART0_RX"]=PI_9;
 
obcPinMap["P_UART1_TX"]=PA_9;
obcPinMap["P_UART1_RX"]=PA_10;
 
obcPinMap["P_UART2_TX"]=PG_14;
obcPinMap["P_UART2_RX"]=PG_9;
 
obcPinMap["P_UART3_TX"]=PJ_8;
obcPinMap["P_UART3_RX"]=PJ_9;

obcPinMap["P_SPI1_MOSI"]=PC_3;
obcPinMap["P_SPI1_MISO"]=PC_2;
obcPinMap["P_SPI1_SCK"]=PI_1;

obcPinMap["P_I2C0"]=PH_7;
obcPinMap["P_I2C0"]=PH_8;
obcPinMap["P_I2C1"]=PB_6;
obcPinMap["P_I2C1"]=PB_7;
obcPinMap["P_I2C2"]=PH_11; 
obcPinMap["P_I2C2"]=PH_12;
 
obcPinMap["P_UART0_TX"]=PA_0;
obcPinMap["P_UART0_RX"]=PI_9;
obcPinMap["P_UART1_TX"]=PA_9;
obcPinMap["P_UART1_RX"]=PA_10;
obcPinMap["P_UART2_TX"]=PG_14; 
obcPinMap["P_UART2_RX"]=PG_9;
obcPinMap["P_UART3_TX"]=PJ_8;
obcPinMap["P_UART3_RX"]=PJ_9;

obcPinMap["RADIO1_NSS"]=PC_13;
obcPinMap["RADIO1_BUSY"]=PD_4;
obcPinMap["RADIO1_RST"]=PC_15;
obcPinMap["RADIO1_DIO1"]=PE_3;
obcPinMap["RADIO1_DIO2"]=PD_5;
obcPinMap["RADIO1_TX"]=PG_3;
obcPinMap["RADIO1_RX"]=PG_10;

obcPinMap["RADIO2_NSS"]=PI_4;
obcPinMap["RADIO2_BUSY"]=PH_10;
obcPinMap["RADIO2_RST"]=PH_14;
obcPinMap["RADIO2_DIO1"]=PI_5;
obcPinMap["RADIO2_DIO2"]=PH_9;
obcPinMap["RADIO2_TX"]=PA_6;
obcPinMap["RADIO2_RX"]=PA_4;

obcPinMap["PHONE_ENABLE"]=PA_11;

obcPinMap["MAG_ENABLE"]=PH_15;

obcPinMap["SENSORS_ENABLE"]=PA_12;

obcPinMap["BURN_WIRE"]=PI_10;

obcPinMap["IMU_OBC_NSS"]=PB_4;
obcPinMap["IMU_OBC_RST"]=PI_14;
obcPinMap["IMU_OBC_INT"]=PI_15;
obcPinMap["IMU_OBC_WAKE"]=PB_3;

obcPinMap["MOTOR_X_FG"]=PJ_10;
obcPinMap["MOTOR_X_SPEED"]=PA_8;
obcPinMap["MOTOR_X_DIR"]=PC_6;

obcPinMap["MOTOR_Y_FG"]=PI_7;
obcPinMap["MOTOR_Y_SPEED"]=PC_7;
obcPinMap["MOTOR_Y_DIR"]=PJ_7;

obcPinMap["MOTOR_Z_FG"]=PI_6;
obcPinMap["MOTOR_Z_SPEED"]=PH_6;
obcPinMap["MOTOR_Z_DIR"]=PK_1;

obcPinMap["CAN1_TX"]=PH_13;
obcPinMap["CAN1_RX"]=PB_8;

}
#endif