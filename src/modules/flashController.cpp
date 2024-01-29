#include <Preferences.h>
#include "flashController.h"

// Definition for Preferences library
Preferences localData;

// Function for getting values from flash memory
int getValueFromFlash(const char* valueName){
    // Open the storage namespace in flash as read only
    localData.begin("storage", true);
    // Get the desired value, close the storage and return the value
    int value = localData.getInt(valueName, 0);
    localData.end();
    return value;
}

// Function to save values to flash memory
void writeValueToFlash(const char* valueName, int value){
    // Open the storage namespace in flash with the ability to write to it
    localData.begin("storage", false);
    // Save the desired value and close the storage
    localData.putInt(valueName, value);
    localData.end();
}