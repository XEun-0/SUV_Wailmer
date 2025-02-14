#include "Common/util.h"

// Variables
#ifdef __AVR_ATmega2560__
char avr_buffer[300];
#endif

/*********************************************************
 * 
 * Name:  compareSizes
 * Notes: See util.h
 * 
 *********************************************************/
void compareSizes(char * name, int actualSize, int expectedSize) {
    if (actualSize != expectedSize) {
        #ifdef __AVR_ATmega2560__
        sprintf(avr_buffer, "(Error) %s expected size: %d bytes, is actually %d bytes", name, expectedSize, actualSize);
        Serial.println(avr_buffer);
        #else
        printf("(Error) %s expected size: %d bytes, is actually %d bytes\n", name, expectedSize, actualSize);
        #endif
    } else {
        #ifdef CONTROL_PANEL
        printf("%s is expected size: %d bytes\n", name, actualSize);
        #else
        sprintf(avr_buffer, "%s is expected size: %d bytes", name, actualSize);
        Serial.println(avr_buffer);
        #endif
    }
}

/*********************************************************
 * 
 * Name:  checkStructSizes
 * Notes: See util.h
 * 
 *********************************************************/
void checkStructSizes(void) {
    compareSizes((char *)"SensorInfoType",                  sizeof(SensorInfoType),                 36);
    compareSizes((char *)"ThrusterIndividualTestProgress",  sizeof(ThrusterIndividualTestProgress), 2);
    compareSizes((char *)"ThrusterSpeedsType",              sizeof(ThrusterSpeedsType),             12);
    compareSizes((char *)"ThrusterInfoType",                sizeof(ThrusterInfoType),               16);
    compareSizes((char *)"TTCSohRespType",                  sizeof(TTCSohRespType),                 52);
    compareSizes((char *)"ThrusterCommandsType",            sizeof(ThrusterCommandsType),           12);

}

/*********************************************************
 * 
 * Name:  hexDump
 * Notes: See util.h
 * 
 *********************************************************/
void hexDump(uint8_t *data, int length) {
    uint8_t *c = data;
    int s;

    for(s = 0; s < length; s++) {
        #ifdef CONTROL_PANEL
        printf("%02X, ", *c++);
        #else
        sprintf(avr_buffer, "%02X, ", *c++);
        Serial.print(avr_buffer);
        #endif
    }
    #ifdef CONTROL_PANEL
    printf("\n");
    #else
    Serial.print("\n");
    #endif
}

/*********************************************************
 * 
 * Name:  pPrint
 * Notes: See util.h
 * 
 *********************************************************/
void pPrintf(const char* msg, ...) {
    // TODO
}