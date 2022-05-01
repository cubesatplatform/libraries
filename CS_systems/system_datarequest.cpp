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
	CMessages* pM=getMessages();
	pM->moveDataToTransmit();
}
