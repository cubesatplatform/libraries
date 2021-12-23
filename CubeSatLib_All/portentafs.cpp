#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)

#include "portentafs.h"
#include "consoleio.h"


#include <LittleFS_Portenta_H7.h>

LittleFS_MBED myFS;

void CFS::filename(const char  *fn){
  fileName = MBED_LITTLEFS_FILE_PREFIX;
  fileName +="/";
  fileName +=fn;

  fileName = MBED_LITTLEFS_FILE_PREFIX;
  fileName +="/";
  fileName +=fn;
}


void CFS::setup()
{
  

  //myFS = new LittleFS_MBED();

  if (!myFS.init()) 
  {
    writeconsoleln("LITTLEFS Mount Failed");    
    return;
  }
}

bool CFS::deleteFile(){  
  writeconsole("Deleting file: "); writeconsole(fileName.c_str());  
  if (remove(fileName.c_str()) == 0) 
  {
    return true;
  }
  return false;
}
 


int CFS::readFile(){  
  writeconsole("Reading from  file: "); writeconsole(fileName.c_str());  
  char buff[BUF_SIZE];
  int count=0;
    
  FILE *file = fopen(fileName.c_str(), "r");
  
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
  char buff[BUF_SIZE];  

  writeconsole("Times have been run = "); writeconsoleln(count);
  
  sprintf(buff, "%d\n", count);
  
  FILE *file = fopen(fileName.c_str(), "w");
  
  if (file) 
  {
    writeconsoleln(" => Open to write OK");
    
    fwrite((uint8_t *) buff, 1, BUF_SIZE - 1, file) ;
    fclose(file);
  }
  
}
#endif