#ifndef errorHandler_h
#define errorHandler_h

// Function declarations
int generateErrorCode(bool variableValues[], int errorCodeLength);
void sendErrorCode(int errorCode, int errorCodeLength);
void displayErrorCode(int errorCode, int errorCodeLength);

#endif