#include <Preferences.h>

Preferences localData;

int getValueFromFlash(const char* valueName){
    localData.begin("storage", true);
    int value = localData.getInt(valueName, 0);
    localData.end();
    return value;
}

void writeValueToFlash(const char* valueName, int value){
    localData.begin("storage", false);
    localData.putInt(valueName, value);
    localData.end();
}