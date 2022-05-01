#pragma once

#define RADIOTXDELAY 100
#define RADIOWAITFORACK 15000
#define RADIOWAITFORCOMPLETE 15000
#define SLEEPTIME 15*1000    //1 Orbit  90*60*1000 Min


#define PING_INTERVAL             2       // seconds, how much time to wait between PINGs sent from this node


#define LORAFREQUENCY             443.0   //FREQUENCY_900M30S
#define RADIOLORAFREQUENCY        443.0   //FREQUENCY_900M30S
#define RADIO2LORAFREQUENCY       445.0   //FREQUENCY_900M30S
#define FREQUENCY_400M30S         443.0   // MHz carrier, for E22-400M30S
#define FREQUENCY_GATEWAY         443.0   // MHz carrier, for ESP32 single-channel gateway
#define BANDWIDTH                 125  //125.0   // kHz dual-sideband
#define SPREADING_FACTOR          9       // 2^9 chips
#define CODING_RATE               7       // 4/7 coding
#define SYNC_WORD                 0x12    // private network
#define OUTPUT_POWER              12      // +14 dBm
#define OUTPUT_POWER_TTGO         16
#define CURRENT_LIMIT             140.0    // mA  was 60,  120
#define PREAMBLE_LENGTH           8       // symbols
#define TCXO_VOLTAGE              1.6     // V




#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
    #define PORTENTA_E22_400M30S
    //#define PORTENTA_E22_900M30S

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

#else
    #define TTGO  //TTGO1
    //#define MYESP32
    //#define ESP32_GATEWAY

    #define TTGO_SCK        5
    #define TTGO_MISO       19
    #define TTGO_MOSI       27
    #define TTGO_SS         18
    #define TTGO_DIO0       26
    #define TTGO_DIO1       33
    #define TTGO_DIO2       32
    #define TTGO_RST        23


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


#endif






#define ESP32_GATEWAY_NSS_BUILTIN               16
#define ESP32_GATEWAY_DIO0_BUILTIN              26
#define ESP32_GATEWAY_NRST_BUILTIN             -1// RADIOLIB_PIN_UNUSED
#define ESP32_GATEWAY_DIO1_BUILTIN              33


 