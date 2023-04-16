#include "cpin.h"
#include <consoleio.h>

int CPin::_ledChannel=0;



void CPin::config (std::string pin, int freq){
    #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
    #else
        int pinnum=atoi(pin.c_str());
        config( pinnum, freq);
    #endif
    
}


void CPin::config(PinName pin,int freq){
    _ledPin=pin;
    _freq=freq;
    #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
        if led!=NULL delete led;
        led = new mbed::PwmOut(_ledPin);
        led->period_us(1000000 / _freq); // Set PWM frequency
    #else
        pinMode(_ledPin, OUTPUT);
        _pinChannel=_ledChannel;
        _ledChannel++;
        ledcSetup(_pinChannel, _freq, _resolution);
        ledcAttachPin(_ledPin, _pinChannel);            
    #endif
}

void CPin::pct(float p){
    p=abs(p);

    #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
    //led->write((float) brightness / _resolution); // Write PWM duty cycle as a float value between 0 and 1
    if(led!=NULL)
        led->write((float) p / 100.0); // Write PWM duty cycle as a float value between 0 and 1
    #else 
    if(p<=0)
    _brightness=0;
    else if (p>=100)
    _brightness=4000;
    else {
    _brightness=(int)p*40.0;
    }
    ledcWrite(_pinChannel, _brightness);  
    #endif
}



void CPin::print(){
    writeconsoleln(_brightness);

}

