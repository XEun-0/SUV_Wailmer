#include "pPrintf.h"

#define STX 0x7F
#define ETX 0x7E

/*********************************************************
 * 
 * Name:  pPrint
 * Notes: See pPrintf.h
 *       
 *********************************************************/
void pPrintf(const char* msg, ...) {
    // If arduino then you want to use Serial library.
    // goal of this file is to rework it such that i can send both a 
    // print debug msg as well as an actual msg of bytes.
    
    va_list args;
    va_start(args, msg);
    
    // Arduino Compilation
    #if defined(__AVR_ATmega2560__)
    size_t msgSize = strlen(msg) + 20; // buffer size
    printf("strlen %d\n", msgSize);
    char avr_buffer[msgSize];

    // [stx][identifier][msg][etx]
    
    vsprintf(avr_buffer, msg, args);
    Serial.print(avr_buffer);
    #else
    // Non-Arduino Compilation
    vprintf(msg, args);
    #endif

    va_end(args);
}

/*********************************************************
 * 
 * Name:  pWrite
 * Notes: See pPrintf.h
 * 
 *********************************************************/
void pWrite(uint8_t* msg) {
    #if defined(__AVR_ATmega2560__)
    // Serial.write();
    #endif
}

/*********************************************************
 * 
 * Name:  pRead
 * Notes: See pPrintf.h
 * 
 *********************************************************/
// most likely used outside Arduino
void pRead() {

}