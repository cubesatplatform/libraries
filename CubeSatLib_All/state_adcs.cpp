#include "state_adcs.h"


CADCSState::CADCSState() {
  Name("ADCS");
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

void CADCSState::setup() {CStateObj::init();};
void CADCSState::stateMsg(CMsg &msg){_statemsg=msg;};
void CADCSState::enter() {  
    CMsg m;
    m.setTABLE("LOG");
    m.setINFO("Enter ADCS");  
    
    addTransmitList(m);
    writeconsoleln(m.serializeout()) ;

   CStateObj::enter();   
   }

void CADCSState::exit() { 
  CMsg m;
  m.setTABLE("LOG");
  m.setINFO("Exit ADCS");  
  
  addTransmitList(m);

  CStateObj::exit();  
  }
