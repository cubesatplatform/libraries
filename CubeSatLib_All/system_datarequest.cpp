#include "system_datarequest.h"
#include <messages.h>

void CDataRequest::loop() {
  writeconsoleln("DataRequest Loop");
 //Pumps messages through the system


 CMsg m;
 m.setSYS("GPS");
 m.setACT("OUTPUT");
 addMessageList(m);
}

void CDataRequest::moveData() {
	CMessages* pmsgs=getMessages();



  for(auto m:pmsgs->DataList){  
		pmsgs->TransmitList.push_back(m);

		//parameters["TIME"]
		//if (cmsg.parameters["CID"]==m.CID()
	}
}
