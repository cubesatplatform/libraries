#pragma once


#include <string>
#include <list>


void mountFS();
void formatFS();
void renameFile(const char * path1, const char * path2);
void deleteFile(const char * path);
void appendFile(const char * path, const char * message, size_t messageSize);
void writeFile(const char * path, const char * message, size_t messageSize);
std::string readFile(const char * path);
void readCharsFromFile(const char * path);
void listDir(std::list<std::string> *plist=NULL);
void printFileTest();

void test();
void testFileIO(const char * path);