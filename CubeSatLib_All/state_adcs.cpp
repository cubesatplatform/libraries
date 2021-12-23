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
void CADCSState::enter() { CStateObj::enter();writeconsoleln("Enter ADCS"); }
void CADCSState::exit() { CStateObj::exit();writeconsoleln("Exit ADCS"); }
