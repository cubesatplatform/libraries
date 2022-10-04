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


void CKeyboard::sendCmd(std::string str){  
  std::string cmd=_simpleCMDs[str];
  if(cmd.size()>1){
    CMsg m(cmd);
    
    writeconsoleln(m.serialize());
    m.setTO(CUBESAT);
    addTransmitList(m); 
  //  updateRadio(m);  
  }
  else{
    if(str.size()>1){
      CMsg m(str);
      
// if((m.getTO()==radio.getIAM())||(m.getTO()=="BSALL")){  //TO:BSALL~ACT:SENDFILE~PATH:UPDATE~FIRST:0~LAST:20
if(0){
        std::string act=m.getACT();
        
       // if(act=="SENDFILE") sendFile(m);
        
      }
      else {
        m.setTO(CUBESAT);  
        addTransmitList(m);         
     //   updateRadio(m);  
      }
    }
  }      
}    


void CKeyboard::loop() {
  
  if(Serial.available()){
    writeconsole(".XSCVXCV");
        _input = Serial.read();
        if(_input =='?'){
          displayCMDs();    
          _cmdstr="";
          _input=' '; 
          return;
        }
        if((_input !='\n')&&(_input !=' ')) _cmdstr+=_input;              
    }
  //while(Serial2.available())   Serial2.print(Serial2.read());

  if(_input =='\n'){
    writeconsoleln(_cmdstr.c_str());
    sendCmd(_cmdstr);   
    _cmdstr="";
    _input=' ';
  }
}



void CKeyboard::dataLoop(){
  if (millis()>_dlast+20){
    _dlast=millis();
    for (auto m:_simpleCMDs){
        sendCmd(m.first);
        delay(10);
      }
  }
}


void CKeyboard::sendSerial(const char* cmd) {    //Send to Phone
  Serial2.print(_id);
  Serial2.flush();
  Serial2.print(",");
  Serial2.flush();
  Serial2.println(cmd);
  Serial2.flush();
  delayMicroseconds(500); 
  _id++;

  writeconsoleln(cmd);
}

void CKeyboard::callCustomFunctions(CMsg &msg)  {  //Used to be NewMsg
  
  std::string sys=msg.getSYS();
  std::string act=msg.getACT();

  msg.writetoconsole();

  if(act=="DATALOOP") dataLoop();
  if(act=="DISPLAYCMDS") displayCMDs();
  
}
