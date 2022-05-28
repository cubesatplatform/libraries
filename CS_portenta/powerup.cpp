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
    delay(200);
    }

void disablePhone(){  
    digitalWrite(PHONE_ENABLE, LOW);  
    delay(200);
    }    


void enableMagsMotors(){  
    digitalWrite(MAGMOTORPOWER_ENABLE, HIGH);  
    
    delay(200);
    }

void disableMagsMotors(){  
    digitalWrite(MAGMOTORPOWER_ENABLE, LOW);      
    delay(200);
    }    


void enableSensors(){  
    digitalWrite(SENSORS_ENABLE, HIGH);      
    delay(200);
    }

void disableSensors(){  
    digitalWrite(SENSORS_ENABLE, LOW);      
    delay(200);
    }    


void enableBurnWire(){  
    digitalWrite(BURN_WIRE, HIGH);  
    delay(200);
    }

void disableBurnWire(){  
    digitalWrite(BURN_WIRE, LOW);  
    delay(200);
    }    


void enableADCS(){  
    digitalWrite(ADCS_ENABLE, HIGH);  
    
    delay(200);
    }

void disableADCS(){  
    digitalWrite(ADCS_ENABLE, LOW);  
    
    delay(200);
    }    

    #else
void enablePhone(){  
   
    }

void disablePhone(){  
   
    }    


void enableMagsMotors(){  
   
    }

void disableMagsMotors(){  
   
    }    


void enableSensors(){  
   
    }

void disableSensors(){  
   
    }    


void enableBurnWire(){  
   
    }

void disableBurnWire(){  
   
    }    



void enableADCS(){  
    
    }

void disableADCS(){  
    
    }    

#endif





