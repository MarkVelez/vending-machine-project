#ifndef serialHandler_h
#define serialHandler_h

// Request types for data retrival
enum requestTypes{
    REQUEST_STORAGE
};
extern requestTypes requestType;

// Function declarations
void serialSetup();
bool establishConnection();
String requestData(requestTypes requestType);

#endif