#include "defs.h"
#include "powerup.h"

#include "arduino.h"

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)

void enablePhone(){  
    digitalWrite(PHONE_ENABLE, HIGH);  
    }

void disablePhone(){  
    digitalWrite(PHONE_ENABLE, LOW);  
    }    


void enableMags(){  
    digitalWrite(MAG_ENABLE, HIGH);  
    }

void disableMags(){  
    digitalWrite(MAG_ENABLE, LOW);  
    }    


void enableSensors(){  
    digitalWrite(SENSORS_ENABLE, HIGH);  
    }

void disableSensors(){  
    digitalWrite(SENSORS_ENABLE, LOW);  
    }    


void enableBurnWire(){  
    digitalWrite(BURN_WIRE, HIGH);  
    }

void disableBurnWire(){  
    digitalWrite(BURN_WIRE, LOW);  
    }    


    #else
void enablePhone(){  
   
    }

void disablePhone(){  
   
    }    


void enableMags(){  
   
    }

void disableMags(){  
   
    }    


void enableSensors(){  
   
    }

void disableSensors(){  
   
    }    


void enableBurnWire(){  
   
    }

void disableBurnWire(){  
   
    }    

    #endif





