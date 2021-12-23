#include "state_detumble.h"
#include <portentafs.h>


CDetumbleState::CDetumbleState() {
  Name("DETUMBLE");
/*
  #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
    CFS fs;

    fs.filename(DETUMBLE_FILE);
    _detumblecount=fs.readFile();
    writeconsoleln(_detumblecount);
  #endif
*/
};

CDetumbleState::~CDetumbleState() {};

void CDetumbleState::setup() {CSystemObject::init();};

void CDetumbleState::stateMsg(CMsg &msg){_statemsg=msg;};

void CDetumbleState::enter() {
  
  _detumblecount++;
  CStateObj::enter();
  writeconsoleln("Enter Detumble"); 
    #if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)    
    CFS fs;
//  fs.writeFile(100);
//  fs.deleteFile();
    fs.filename(DETUMBLE_FILE);
    fs.writeFile(_detumblecount);    
    #endif
  
  }
  
void CDetumbleState::exit() { CStateObj::exit();writeconsoleln("Exit Detumble"); }
