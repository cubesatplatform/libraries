#pragma once


#include <string>
#include <list>
#include <msg.h>


void mountFS();
std::string getFullPath(const char * path);
void formatFS();
void renameFile(const char * path1, const char * path2);
void deleteFile(const char * path);
void deleteFiles(const char * path);
void appendFile(const char * path, const char * message, size_t messageSize);
void appendFile(const char * path, const char * path1);
void writeFile(const char * path, const char * message, size_t messageSize);
std::string readFile(const char * path);
int readFileBinary(const char * path, std::vector<char> *pbyteVector);
void readMsgList(const char * path,std::list<CMsg *> *pMList );
void writeMsgList(const char * path,std::list<CMsg *> *pMList );
void readCharsFromFile(const char * path);
CMsg listDir(const char * path=NULL,std::list<std::string> *plist=NULL);
void printFileTest();

void test();
void testFileIO(const char * path);