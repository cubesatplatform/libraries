#include "Arduino.h"
#include "powerup.h"


#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)


#define PHONE_ENABLE PA_11
#define MAGMOTORPOWER_ENABLE PH_15   //For MAGs I2C this is all you need
#define SENSORS_ENABLE  PA_12
#define ADCS_ENABLE  PI_0            //For I2C on ADCS board or Motors you need  this
#define BURN_WIRE PI_10

void enablePhone(){  
    digitalWrite(PHONE_ENABLE, HIGH);
    delay(100);
    }

void disablePhone(){  
    digitalWrite(PHONE_ENABLE, LOW);  
    delay(100);
    }    


void enable65V(){  
    digitalWrite(MAGMOTORPOWER_ENABLE, HIGH);  
    enableMBLogic();
    //delay(100);
    }

void disable65V(){  
    digitalWrite(MAGMOTORPOWER_ENABLE, LOW);      
    delay(100);
    }    


void enableSensors(){  
    digitalWrite(SENSORS_ENABLE, HIGH);      
    delay(100);
    }

void disableSensors(){  
    digitalWrite(SENSORS_ENABLE, LOW);      
    delay(100);
    }    


void enableBurnWire(){  
    digitalWrite(BURN_WIRE, HIGH);  
    delay(100);
    }

void disableBurnWire(){  
    digitalWrite(BURN_WIRE, LOW);  
    delay(100);
    }    


void enableMBLogic(){  
    digitalWrite(ADCS_ENABLE, HIGH);  
    
    delay(100);
    }

void disableMBLogic(){  
    digitalWrite(ADCS_ENABLE, LOW);  
    
    delay(100);
    }    

    #else
void enablePhone(){  
   
    }

void disablePhone(){  
   
    }    


void enable65V(){  
   
    }

void disable65V(){  
   
    }    


void enableSensors(){  
   
    }

void disableSensors(){  
   
    }    


void enableBurnWire(){  
   
    }

void disableBurnWire(){  
   
    }    



void enableMBLogic(){  
    
    }

void disableMBLogic(){  
    
    }    

#endif





