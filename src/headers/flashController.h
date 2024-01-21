#ifndef flashController_h
#define flashController_h

// Function declarations
int getValueFromFlash(const char* valueName);
void writeValueToFlash(const char* valueName, int value);

#endif