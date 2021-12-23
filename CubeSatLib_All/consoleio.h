#pragma once
#include "msg.h"
#include <iostream>
#include <list>



void writeconsole(CMsg &m);

void writeconsole(const std::string s);

void writeconsole(const char* s);

void writeconsole(int s);

void writeconsole( long s);

void writeconsole( unsigned long s);

void writeconsole(char s);

void writeconsole(float s);

void writeconsoleln();

void writeconsoleln(const std::string s);

void writeconsoleln(const char* s);

void writeconsoleln(int s);

void writeconsoleln( long s);

void writeconsoleln( unsigned long s);

void writeconsoleln(float s);

void writeOut();


//Encoding formulas
//y=2x+<seed>
//x=(y-<seed>)/2

int charEncode(int x, int seed);

int charDecode(int y, int seed);

std::string stringEncode(std::string str,int seed);

std::string stringDecode(std::string str,int seed);