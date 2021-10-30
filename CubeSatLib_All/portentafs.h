


/****************************************************************************************************************************
  LittleFS_Counting.ino - Filesystem wrapper for LittleFS on the Mbed Portenta_H7
  
  For MBED Portenta_H7 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/LittleFS_Portenta_H7
  Licensed under MIT license

  Version: 1.0.2

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      09/09/2021 Initial coding to support MBED Portenta_H7
  1.0.1   K Hoang      13/09/2021 Select fix LittleFS size of 1024KB
  1.0.2   K Hoang      14/09/2021 Back to using auto LittleFS to fix bug
*****************************************************************************************************************************/

#define _LFS_LOGLEVEL_          1

#include <string>


class CFS{
  std::string fileName;
  int BUF_SIZE=32;
  
public:
  CFS(){setup();setFilename("counts.txt");}
  ~CFS(){}
  void setFilename(const char  *fn);
  void setup();
  bool deleteFile();
  int readFile();
  void writeFile(int count);
};


