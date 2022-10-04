#pragma once

#include <string>
long getTime();
std::string getTimeString();
std::string tostring(int val);
std::string tostring(unsigned int val);
std::string tostring(long val);
std::string tostring(unsigned long val);
std::string tostring(float val);
std::string tostring(double val);
std::string replaceall(std::string str, std::string from, std::string to) ;
std::string strReplace(std::string original, const char * search, std::string val);
