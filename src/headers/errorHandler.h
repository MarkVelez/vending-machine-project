#ifndef errorHandler_h
#define errorHandler_h

extern bool connectionFailed;
extern bool requestFailed;
extern bool topSensorTriggered;
extern bool exitSensorTriggered;
extern bool bottomSensorTriggered;
extern bool emptyStorage;

// Function declarations
char* generateErrorCode();

#endif