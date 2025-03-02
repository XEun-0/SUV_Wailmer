#ifndef PPRINTF_H
#define PPRINTF_H

#include <stdarg.h>
#if defined(__AVR_ATmega2560__)
#include <Arduino.h>                // Serial
#include "string.h"
#else
#include <cstring>          // strlen
#include <iostream>                 // Use standard C++ output for Qt
#include "serialib.h"
#define Serial std::cout
#endif
#include "Interface/ttcSohResp.h"   // Structs and Enums

void pPrintf(const char* msg, ...);
void pWrite(uint8_t* msg);
void pRead();

#endif // PPRINTF_H