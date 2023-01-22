#pragma once

//int analogWriteChannel(uint8_t pin){};

//void analogWriteFrequency(double frequency){};
//void analogWriteFrequency(uint8_t pin, double frequency){};

//void analogWriteResolution(uint8_t resolution){};
//void analogWriteResolution(uint8_t pin, uint8_t resolution){};

void analogWrite(int pin, long value, long valueMax = 255);