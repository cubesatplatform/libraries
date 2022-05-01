#include "portentafs.h"
#include <consoleio.h>


#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)

#include <LittleFS_Portenta_H7.h>
LittleFS_MBED myFS;


CFS::CFS(){
  setup();
  }

void CFS::setFilename(const char  *fn){
  std::string tmpstr=fn;

  if (tmpstr.size()==0)
    return;
  _fileName = MBED_LITTLEFS_FILE_PREFIX;
  _fileName +="/";
  _fileName +=fn;

  _fileName = MBED_LITTLEFS_FILE_PREFIX;
  _fileName +="/";
  _fileName +=fn;
}


void CFS::setup()
{
  
  if (!myFS.init()) 
  {
    CMsg m;
    m.setTABLE("LOG");
    m.setINFO("LITTLEFS Mount Failed");  
    addTransmitList(m);
    writeconsoleln(m.serializeout()) ;
    return;
  }
}

bool CFS::deleteFile(){  
  if (_fileName.size()==0)
    return false;
  if (remove(_fileName.c_str()) == 0) 
  {
    return true;
  }
  return false;
}
 


int CFS::readFile(){  
  if (_fileName.size()==0)
    return 0;
  char buff[BUF_SIZE];
  int count=0;
    
  FILE *file = fopen(_fileName.c_str(), "r");
  
  if (file) 
  {
    writeconsoleln(" => Open to read OK");
    
    bzero(buff, BUF_SIZE);
    
    uint32_t numRead = fread((uint8_t *) buff, BUF_SIZE - 1, 1, file);
   
    if ( numRead )
    {
      sscanf(buff, "%d", &count);
    }
    
    fclose(file);

  }
  return count;
}


void CFS::writeFile(int count){  
  if (_fileName.size()==0)
    return;
  char buff[BUF_SIZE];  

  writeconsole("Times have been run = "); writeconsoleln(count);
  
  sprintf(buff, "%d\n", count);
  
  FILE *file = fopen(_fileName.c_str(), "w");
  
  if (file) 
  {
    writeconsoleln(" => Open to write OK");
    
    fwrite((uint8_t *) buff, 1, BUF_SIZE - 1, file) ;
    fclose(file);
  }

  
}
#endif