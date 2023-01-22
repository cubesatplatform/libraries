#include "state_adcs.h"


CADCSState::CADCSState() {  
  setMaxTime(3*TIMEORBIT);  
};

CADCSState::~CADCSState() {};

void CADCSState::setup() {CStateObj::init();setMaxTime(3*TIMEORBIT);};

void CADCSState::enter() {  
    

   CStateObj::enter();   
   }

void CADCSState::exit() { 
  

  CStateObj::exit();    
  }
