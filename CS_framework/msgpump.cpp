#include "msgpump.h"


void CMsgPump::loop() {
	//Gets messages receieved from radio, pushes to message list and then pumps them out
  MMM.moveReceived();
  pump();

}


void CMsgPump::pump(){

CMsg msg;

  
int count=0;
int size=MMM.MessageList.size();


while(count<size){
  
  
  count++;

  msg = MMM.MessageList.MList.front();
  MMM.MessageList.MList.pop_front();

  writeconsoleln("------------------>Msg Pump");
  msg.writetoconsole();
  

	if (msg.isReadyToProcess()){		
		if(msg.Parameters.size()) {
      std::string sys=msg.get(_SYS,_MANAGER);
      CSystemObject *pObj=getSystem(sys.c_str());
      if(pObj!=NULL) pObj->newMsg(msg);   //Satellite          }
      else{
        
        writeconsole(sys); writeconsoleln("   Pump()  No System Object");
        msg.writetoconsole();
      }

    }
  
	}
	else{
		
		
    MMM.MessageList.MList.push_back(msg);    
	}

  }
  //MSG->MessageList.clear();//Probable make sure messages have all been processed.  I think they will as only thing that can add messages should be the loop    
}



void CMsgPump::stats(CMsg &msg){    
  
  CMsg m;
  int dmSize=MMM.DataMap.size();    
  int rSize=MMM.ReceivedList.size();    
  int mSize=MMM.MessageList.size();     
  int tSize=MMM.TransmitList.size();    
  int tdSize=MMM.TransmittedList.size(); 

  m.set("RL",rSize);
  m.set("ML",mSize);
  m.set("TL",tSize);
  m.set("TDL",tdSize);  
  m.set("DM",dmSize);  

  addTransmitList(m);
}

void CMsgPump::saveToCloud(CMsg &msg){
  int val=msg.get(_VALUE,1);

  if (val) MMM.saveToCloud(true);
  else MMM.saveToCloud(false);
}


void CMsgPump::callCustomFunctions(CMsg &msg){
  
	std::string sys = msg.get(_SYS);
	std::string act = msg.get(_ACT);

  
  if (act=="PRUNE"){MMM.prune(); return;}
  if (act=="CLEAR"){
    MMM.MessageList.clear();
    MMM.ReceivedList.clear();
    MMM.TransmitList.clear();
    MMM.TransmittedList.clear();
    MMM.clearDataMap();
    return;
    }
  if (act=="DISPLAYLIST"){int val=msg.get(_VALUE,0);MMM.displayList(val); return;}

  if (act=="MLPOPFRONT"){ MMM.MessageList.pop_front(); return;}
  if (act=="MLPOPBACK"){ MMM.MessageList.pop_back(); return;}
  if (act=="MLPUSHBACK"){ MMM.MessageList.push_back(msg); return;}
  if (act=="MLPUSHFRONT"){ MMM.MessageList.push_front(msg); return;} 
  if (act=="MLCLEAR"){ MMM.MessageList.clear(); return;}
  if (act=="MLPRUNE"){ MMM.MessageList.prune(); return;}

  if (act=="RLPOPFRONT"){ MMM.ReceivedList.pop_front(); return;}
  if (act=="RLPOPBACK"){ MMM.ReceivedList.pop_back(); return;}
  if (act=="RLPUSHBACK"){ MMM.ReceivedList.push_back(msg); return;}
  if (act=="RLPUSHFRONT"){ MMM.ReceivedList.push_front(msg); return;} 
  if (act=="RLCLEAR"){ MMM.ReceivedList.clear(); return;}
  if (act=="RLPRUNE"){ MMM.ReceivedList.prune(); return;}

  if (act=="TLPOPFRONT"){ MMM.TransmitList.pop_front(); return;}
  if (act=="TLPOPBACK"){ MMM.TransmitList.pop_back(); return;}
  if (act=="TLPUSHBACK"){ MMM.TransmitList.push_back(msg); return;}
  if (act=="TLPUSHFRONT"){ MMM.TransmitList.push_front(msg); return;} 
  if (act=="TLCLEAR"){ MMM.TransmitList.clear(); return;}
  if (act=="TLPRUNE"){ MMM.TransmitList.prune(); return;}

  if (act=="DLPOPFRONT"){ MMM.TransmittedList.pop_front(); return;}
  if (act=="DLPOPBACK"){ MMM.TransmittedList.pop_back(); return;}
  if (act=="DLPUSHBACK"){ MMM.TransmittedList.push_back(msg); return;}
  if (act=="DLPUSHFRONT"){ MMM.TransmittedList.push_front(msg); return;} 
  if (act=="DLCLEAR"){ MMM.TransmittedList.clear(); return;}
  if (act=="DLPRUNE"){ MMM.TransmittedList.prune(); return;}

  if (act=="MOVERECEIVED"){ MMM.moveReceived(); return;}

  //mapcustom(MMM.clearDataMap);
  //mapcustommsg(MMM.sendDataMap)
  //mapcustom(MMM.displayFreeHeap)
  mapcustommsg(saveToCloud)

  CSystemObject::callCustomFunctions(msg);
}
