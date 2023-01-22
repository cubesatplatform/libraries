/*
  Portenta - TestSDCARD

  The sketch shows how to mount an SDCARD and list its content.

  The circuit:
   - Portenta H7 + Vision Shield
   - Portenta H7 + Portenta Breakout

  This example code is in the public domain.
*/

#include <consoleio.h>
#include "sd_fs.h"


#if defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_PORTENTA_H7_M7)
#include "SDMMCBlockDevice.h"
#include "FATFileSystem.h"

SDMMCBlockDevice block_device;
mbed::FATFileSystem fs("fs");

#define FSPATH "/fs/"


std::string CFileSystemObj::getFullPath(const char * path){
  std::string str;
  writeconsole("File: "); writeconsoleln(path);
  if(path==NULL)
    return str;
  std::string strPath=FSPATH;
  strPath+=path;

  writeconsole("Reading file: "); writeconsoleln(strPath);
  return strPath;
}



void CFileSystemObj::mountFS() {

  writeconsoleln("Mounting SDCARD...");
  int err =  fs.mount(&block_device);
  if (err) {
    // Reformat if we can't mount the filesystem
    // this should only happen on the first boot
    writeconsoleln("No filesystem found, please check on computer and manually format");
    
  }  else    writeconsoleln("Filesystem mounted");
  
  
}

void CFileSystemObj::formatFS() {
    int err = fs.reformat(&block_device);  // seriously don't want to format good data
    if (err) {        writeconsoleln("Could not Format Filesystem");    }
    CMsg m;
}
  

CMsg CFileSystemObj::listDir(const char * path,std::list<std::string> *plist){
  DIR *dir;
  struct dirent *ent;
  int dirIndex = 0;
  CMsg m;
  std::string spath,val;

  m.set(_SYS,"DIRECTORY");
  writeconsoleln("List SDCARD content: ");
  spath=path;

  if ((dir = opendir("/fs")) != NULL) {
    // Print all the files and directories within directory (not recursively)
    while ((ent = readdir (dir)) != NULL) {
      writeconsoleln(ent->d_name);   //No easy way to get filesize without opening file
      std::string str;
      str=ent->d_name;
      
      if (!spath.size()){
      
        if(plist!=NULL){       
          plist->push_back(str);
          }
      
        val=tostring(dirIndex);
        m.set(val,str);
        dirIndex++;
      }

      else{
      
        if(plist!=NULL){
          if(str.find(path)!=std::string::npos)          
            plist->push_back(str);
        }
      
        if(str.find(path)!=std::string::npos){      
          val=tostring(dirIndex);
          m.set(val,str);
          dirIndex++;
        }      
      }
    }
    closedir (dir);
  } else {   m.set(_ERROR,"Error opening SDCARD");    writeconsoleln("Error opening SDCARD\n");  return m;  }
  if(dirIndex == 0) {   m.set(_ERROR,"Empty SDCARD"); writeconsoleln("Empty SDCARD");  } 
 return m;
}




void CFileSystemObj::readCharsFromFile(const char * path)
{
  writeconsole("readCharsFromFile: "); writeconsole(path);

  if(path==NULL)
    return;

 std::string strPath=FSPATH;
    strPath+=path;

  FILE *file = fopen(strPath.c_str(), "r");

  if (file)  {    writeconsoleln(" => Open OK");  }  else  {    writeconsoleln(" => Open Failed");    return;  }

  unsigned char c;

  while (true)
  {
    c = fgetc(file);

    if ( feof(file) )
    {
      break;
    }
    else
      writeconsole(c);
  }

  fclose(file);
}

int CFileSystemObj::readFileBinary(const char * path,std::vector<unsigned char> *pbyteVector){
  bool isBinary=false;

  std::string str,strPath=getFullPath(path);
  if(strPath.find(".bin")!=std::string::npos) isBinary=true;

  if(!isBinary){
    return 0;
  }


  FILE *file = fopen(strPath.c_str(), "r");

  if (file)  {    writeconsoleln(" => Open OK");  }  else  {    writeconsoleln(" => Open Failed");    return 0;  }

  unsigned char c;
  uint32_t numRead = 1;
  int count=0;

  
  

  fseek ( file , 0 , SEEK_END );
  long size=ftell (file);

  fseek ( file , 0 , SEEK_SET );

  pbyteVector->reserve(size);

  while (numRead)
  {
    numRead = fread( &c, sizeof(c), 1, file);

    if (numRead){
     pbyteVector->push_back(c);
     str+=c;
     count++;
    }
  }

  fclose(file);
  pbyteVector->shrink_to_fit();
  return numRead;
}

std::string CFileSystemObj::readFile(const char * path)
{
  
  std::string str,strPath=getFullPath(path);

  FILE *file = fopen(strPath.c_str(), "r");

  if (file)  {    writeconsoleln(" => Open OK");  }  else  {    writeconsoleln(" => Open Failed");    return str;  }

  unsigned char c;
  uint32_t numRead = 1;

  while (numRead)
  {
    numRead = fread( &c, sizeof(c), 1, file);

    if (numRead){
     writeconsole(c);
     str+=c;
    }
  }

  fclose(file);
  return str;
}

void CFileSystemObj::writeFile(const char * path, const unsigned char * message, size_t messageSize)
{
  writeconsole("writeFile "); writeconsoleln((int)messageSize);
  if(!messageSize)
    return;
  std::string strPath=getFullPath(path);
  FILE *file = fopen(strPath.c_str(), "w");

  if (file)  {    writeconsoleln(" => Open OK");  }  else  {    writeconsoleln(" => Open Failed");    return;  }

  if (fwrite( message, 1, messageSize, file))  {    writeconsoleln("* Writing OK");  }
  else  {    writeconsoleln("* Writing failed");  }

  fclose(file);
}

void CFileSystemObj::appendFileBinary(const char * path, const char * path1){
  std::string strPath=getFullPath(path);
  std::string strPath1=getFullPath(path);

  FILE *file = fopen(strPath.c_str(), "a");
  FILE *file1 = fopen(strPath1.c_str(), "r");

  if (file)  {    writeconsoleln(" => File Open OK");  }  else  {    writeconsoleln(" => File Open Failed");    return;  }

  if (file1)  {    writeconsoleln(" => File 1 Open OK");  }  else  {    writeconsoleln(" => File 1 Open Failed");    return;  }

  unsigned char c;
  uint32_t numRead = 1;

  while (numRead)
  {
    numRead = fread( &c, sizeof(c), 1, file1);

    if (numRead){
     fwrite( &c, 1, 1, file);
    }
  }
fclose(file);
fclose(file1);

}

void CFileSystemObj::appendFile(const char * path, const unsigned char * message, size_t messageSize)
{
  if(!messageSize)
    return;
  std::string strPath=getFullPath(path);

  FILE *file = fopen(strPath.c_str(), "a");

  if (file)  {    writeconsoleln(" => Open OK");  }  else  {    writeconsoleln(" => Open Failed");    return;  }

  if (fwrite( message, 1, messageSize, file))   {    writeconsoleln("* Appending OK");  }  else  {    writeconsoleln("* Appending failed");  }

  fclose(file);
}



void CFileSystemObj::deleteFile(const char * path)
{
  std::string strPath=getFullPath(path);

  if (remove(strPath.c_str()) == 0)  {    writeconsoleln(" => OK");  }  else  {    writeconsoleln(" => Failed");    return;  }
}


void CFileSystemObj::deleteFiles(const char * path){
writeconsole(" deleteFiles");   writeconsoleln(path);
CMsg m=listDir();

  for (auto x:m.Parameters){
    std::string str;
    str=x.second;
    if(str.find(path)!=std::string::npos){     
      deleteFile(str.c_str());
    }
  }
}


void CFileSystemObj::renameFile(const char * path1, const char * path2)
{
  std::string strPath1=getFullPath(path1);
  std::string strPath2=getFullPath(path2);

  if (rename(strPath1.c_str(), strPath2.c_str()) == 0)  {    //writeconsoleln(" => OK");
  }  else  {    writeconsoleln(" => Failed");    return;  }
}

void CFileSystemObj::testFileIO(const char * path)
{
    writeconsole("Testing file I/O with: "); writeconsole(path);

    #define BUFF_SIZE  512
    #define FILE_SIZE_KB  64
  std::string strPath=getFullPath(path);

  static uint8_t buf[BUFF_SIZE];

  FILE *file = fopen(strPath.c_str(), "w");

 if (file)  {    writeconsoleln(" => Open OK");  }  else  {    writeconsoleln(" => Open Failed");    return;  }

  size_t i;
  writeconsoleln("- writing" );

  uint32_t start = millis();

  size_t result = 0;

  // Write a file with FILE_SIZE_KB
  for (i = 0; i < FILE_SIZE_KB * 2; i++)
  {
    result = fwrite(buf, BUFF_SIZE, 1, file);

    if ( result != 1)
    {
    //  writeconsole("Write result = "); writeconsoleln(result);
    //  writeconsole("Write error, i = "); writeconsoleln(i);

      break;
    }
  }

  writeconsoleln("");
  uint32_t end = millis() - start;

 // writeconsole(i / 2);
  writeconsole(" Kbytes written in (ms) ");
  
  fclose(file);


  /////////////////////////////////

  file = fopen(path, "r");

  start = millis();
  end = start;
  i = 0;

  if (file)
  {
    start = millis();
    writeconsoleln("- reading" );

    result = 0;

    fseek(file, 0, SEEK_SET);

    // Read a file with FILE_SIZE_KB
    for (i = 0; i < FILE_SIZE_KB * 2; i++)
    {
      result = fread(buf, BUFF_SIZE, 1, file);

      if ( result != 1 )
      {
      //  writeconsole("Read result = "); writeconsoleln(result);
      //  writeconsole("Read error, i = "); writeconsoleln(i);

        break;
      }
    }

    writeconsoleln("");
    end = millis() - start;

   // writeconsole((i * BUFF_SIZE) / 1024);
    writeconsole(" Kbytes read in (ms) ");
   // writeconsoleln(end);

    fclose(file);
  }
  else
  {
    writeconsoleln("- failed to open file for reading");
  }
}


void CFileSystemObj::test(){

  char fileName1[] = "/fs/hello1.txt";
  char fileName2[] = "/fs/hello2.txt";

  //char message[]  = "Hello from " BOARD_NAME "\n";
  //unsigned char message[]  = "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
  unsigned char message[]  = "012345678901234567890";
  writeFile(fileName1, message, sizeof(message));


  for(int count=0;count<100;count++){
    char buffer [300];    
    snprintf ( buffer, 30, "/fs/fn%d", count );
    std::string fn;//= MBED_LITTLEFS_FILE_PREFIX;
    fn+=std::string(buffer);
   
    writeFile(fn.c_str(), message, sizeof(message));
  }

  
  for(int count=0;count<100;count++){
    char buffer [300];    
    snprintf ( buffer, 30, "/fs/fn%d", count );
    std::string fn;//= MBED_LITTLEFS_FILE_PREFIX;
    fn+=std::string(buffer);
   
    writeconsoleln(readFile(fn.c_str()).c_str());
  }
  
  
  readFile(fileName1);

  appendFile(fileName1, message, sizeof(message));  
  readFile(fileName1);

  renameFile(fileName1, fileName2);
  readCharsFromFile(fileName2);

  deleteFile(fileName2);
  readFile(fileName2);

  testFileIO(fileName1);
  testFileIO(fileName2);
  deleteFile(fileName1);
  deleteFile(fileName2);

  writeconsoleln( "\nTest complete" );
}


void CFileSystemObj::printFileTest() {
    FILE *myFile;
    test();
    
  int myTime = millis()/1000; // seconds since ???
  char myFileName[] = "fs/00000000.json";
  myFileName[3] = myTime/10000000 % 10 + '0';
  myFileName[4] = myTime/1000000 % 10 + '0';
  myFileName[5] = myTime/100000 % 10 + '0';
  myFileName[6] = myTime/10000 % 10 + '0';
  myFileName[7] = myTime/1000 % 10 + '0';
  myFileName[8] = myTime/100 % 10 + '0';
  myFileName[9] = myTime/10 % 10 + '0';
  myFileName[10] = myTime % 10 + '0';
  //const char *myFileName2 = myFileName;
  myFile = fopen(myFileName, "a");  // "a" for append or make it if file not there
    writeconsoleln(myFileName);

  //  fprintf(myFile,"test \r\n");
    fprintf(myFile,"{\"protected\":{\"ver\":\"v1\",\"alg\":\"none\",\"iat\":1603914126},");
    fprintf(myFile,"\"signature\":\"0\",\"payload\":{\"device_type\":\"TEST\",\"interval_ms\":10,");
    fprintf(myFile,"\"sensors\":[{\"name\":\"sensor1\",\"units\":\"N/A\"}],");
    fprintf(myFile,"\"values\":[234,423,534,654]}}\r\n");

  fclose(myFile);
  delay(30000);

}


void CFileSystemObj::readMsgList(const char * path,std::list<CMsg > *pMList ){
  std::list<CMsg> ml;  
  std::string str,strPath=getFullPath(path);

  FILE *file = fopen(strPath.c_str(), "r");

  if (file)  {    writeconsoleln(" => Open OK");  }  else  {    writeconsoleln(" => Open Failed");    return;  }

  unsigned char c;
  uint32_t numRead = 1;

  while (numRead)
  {
    numRead = fread( &c, sizeof(c), 1, file);

    if (numRead){
     //writeconsole(c);
     if(c==char(13)){
       CMsg m(str);  
       pMList->push_back(m);
       str="";
     }
     str+=c;
    }
  }

  fclose(file);

  if(str.size()){
    CMsg m(str);  
    pMList->push_back(m);
    str="";
  }

  return;
}


void CFileSystemObj::writeMsgList(const char * path,std::list<CMsg > *pMList ){

 std::string strPath=getFullPath(path);
  FILE *file = fopen(strPath.c_str(), "w");

  if (file)  {    writeconsoleln(" => Open OK");  }  else  {    writeconsoleln(" => Open Failed");    return;  }

  for(auto m:*pMList){
    std::string str=m.serialize();
    str+=char(13);
    if (fwrite((uint8_t *) str.c_str(), 1, str.size(), file)){    writeconsoleln("* Writing OK");  }  else  {    writeconsoleln("* Writing failed");  }
  }


  fclose(file);

}

void CFileSystemObj::appendFile(const char * path, const char * path1){
  std::string str=readFile(path1);
  appendFile(path,(unsigned char *)str.c_str(),str.size());
}

CMsg CFileSystemObj::fileSize(const char * path){
  CMsg m;

  m.set(_SYS,"FS");
  m.set(_PATH,path);

  std::string str,strPath=getFullPath(path);

  FILE *file = fopen(strPath.c_str(), "r");

  if (file)  {    writeconsoleln(" => Open OK");  }  else  {    writeconsoleln(" => Open Failed");    return str;  }

  long size=0;
  uint32_t numRead = 1;
  
  unsigned char c;

  while (numRead)
  {
    numRead = fread( &c, sizeof(c), 1, file);

    if (numRead){
     size++;
    }
  }

  fclose(file);
  m.set("SIZE",size);
  return m;
}

#else
void CFileSystemObj::mountFS(){}
void CFileSystemObj::renameFile(const char * path1, const char * path2){}
void CFileSystemObj::deleteFile(const char * path){}
void CFileSystemObj::appendFile(const char * path, const unsigned char * message, size_t messageSize){}
void CFileSystemObj::appendFile(const char * path, const char * path1){}
void CFileSystemObj::writeFile(const char * path, const unsigned char * message, size_t messageSize){}
std::string CFileSystemObj::readFile(const char * path){}
void CFileSystemObj::readCharsFromFile(const char * path){}
void CFileSystemObj::listDir(){}
void CFileSystemObj::printFileTest(){}

void CFileSystemObj::testFS(){}
void CFileSystemObj::testFileIO(const char * path){}
std::list<CMsg> CFileSystemObj::readMsgList(const char * path){}
void CFileSystemObj::writeMsgList(const char * path,std::list<CMsg > *pMList );
void CFileSystemObj::readMsgList(const char * path,std::list<CMsg > *pMList );
CMsg CFileSystemObj::fileSize(const char * path){}
std::string CFileSystemObj::getFullPath(const char * path){}
#endif