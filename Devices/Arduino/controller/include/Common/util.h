#ifndef UTIL_H
#define UTIL_H

#if defined(__AVR_ATmega2560__)
#include <Arduino.h>                // Serial
#else
#include <iostream>                 // Use standard C++ output for Qt
#define Serial std::cout
#endif
#include "Interface/ttcSohResp.h"   // Structs and Enums

void compareSizes(char * name, int actualSize, int expectedSize);
void checkStructSizes(void);
void hexDump(uint8_t *data, int length);
void pPrint(const char* msg, ...);


#endif // UTIL_H