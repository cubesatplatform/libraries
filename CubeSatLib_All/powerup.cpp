#include "defs.h"
#include "powerup.h"

#include "arduino.h"

#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)

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
    digitalWrite(MAGMOTORPOWER_ENABLE, HIGH);  
    digitalWrite(ADCS_ENABLE, HIGH);  
    delay(200);
    }

void disableADCS(){  
    digitalWrite(MAGMOTORPOWER_ENABLE, LOW);  
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





