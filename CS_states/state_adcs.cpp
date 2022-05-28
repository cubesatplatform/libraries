#include "state_defs.h"
#include "state_adcs.h"
#include <powerup.h>

CADCSState::CADCSState() {
  Name("ADCS");
  setMaxTime(3*TIMEORBIT);
  //add some items to the map
  availablesystems["MT"] = true;
  availablesystems["MAGX"] = true;
  availablesystems["MAGY"] = true;
  availablesystems["MAGZ"] = true;
  availablesystems["MOTORX"] = true;
  availablesystems["MOTORY"] = true;
  availablesystems["MOTORZ"] = true;
};

CADCSState::~CADCSState() {};

void CADCSState::setup() {CStateObj::init();setMaxTime(3*TIMEORBIT);};
void CADCSState::stateMsg(CMsg &msg){_statemsg=msg;};
void CADCSState::enter() {  
    

   CStateObj::enter();   
   }

void CADCSState::exit() { 
  //   Don't disable.  Need to keep stuff spinning   disableMagsMotors
  

  CStateObj::exit();    
  }
