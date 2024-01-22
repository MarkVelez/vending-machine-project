#ifndef errorHandler_h
#define errorHandler_h

// Global variable to hold the error code in HEX
extern char errorHex[3];

// Global error bools
extern bool connectionFailed;
extern bool requestFailed;
extern bool topSensorTriggered;
extern bool exitSensorTriggered;
extern bool bottomSensorTriggered;
extern bool emptyStorage;

// Function declarations
void generateErrorCode();

#endif