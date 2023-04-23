#include "kb.h"


void CKeyboard::displayCMDs(){  
  writeconsoleln();
  writeconsoleln("Commands: ");
  writeconsoleln("----------------------------------------------------------------");
  for (auto it = _simpleCMDs.begin(); it != _simpleCMDs.end(); it++) {    
    std::string tmpstr = "   "+it->first + ": " + it->second;
    writeconsoleln(tmpstr.c_str());
  }
  writeconsoleln("----------------------------------------------------------------");
}

void CKeyboard::sendCmd(CMsg &msg){  
  std::string s=msg.get(_VALUE);
  sendCmd(s);
}

void CKeyboard::sendCmd(std::string str){  
  writeconsoleln("Keyboard");
  


  CMsg m(str);
  if((IAM()==m.get(_TO)) && (m.get(_TO)!=_BLANK)){       
    if((IAM()=="ADR1") && (m.get(_TO)==_BLANK)){
      m.set(_TO,"ADR1");
    }
    addMessageList(m);   
  }
  else {    
    if((IAM()=="ADR1") && (m.get(_TO)==_BLANK)){
      m.set(_TO,"ADR1");
    }
    addTransmitList(m);     
  }
}    


void CKeyboard::loop() {
  
  if(Serial.available()){

  _input = Serial.read();
  if(_input =='?'){
    displayCMDs();    
    _cmdstr=_BLANK;
    _input=' '; 
    return;
  }
  if((_input !='\n')&&(_input !=' ')) 
  _cmdstr+=_input;              
  }


  if(_input =='\n'){  
    sendCmd(_cmdstr);   
    _cmdstr=_BLANK;
    _input=' ';
  }
}



void CKeyboard::dataLoop(){
  if (getTime()>_dlast+20){
    _dlast=getTime();
    for (auto m:_simpleCMDs){
        sendCmd(m.first);
        delay(10);
      }
  }
}




void CKeyboard::callCustomFunctions(CMsg &msg)  {  //Used to be NewMsg
  std::string sys=msg.get(_SYS);
  std::string act=msg.get(_ACT);
  

  mapcustom(dataLoop)
  mapcustom(displayCMDs)

 // if(act==DATALOOP) dataLoop();
 // if(act=="DISPLAYCMDS") displayCMDs();
  CSystemObject::callCustomFunctions(msg);
}
