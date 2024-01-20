#include <Arduino.h>
#include "../headers/errorHandler.h"
#include "../headers/lcdController.h"

char errorHex[3];
bool errorBools[] = {connectionFailed, requestFailed, emptyStorage, topSensorTriggered, exitSensorTriggered, bottomSensorTriggered};
int errorCodeLength = sizeof(errorBools) / sizeof(errorBools[0]);

char* generateErrorCode(){
    int errorCode = 0;

    for (int i = 0; i < errorCodeLength; i++){
        errorCode |= (errorBools[i] << i);
    }

    Serial.println("ERROR>");
    sprintf(errorHex, "%02X", errorCode);
    Serial.print(errorHex);

    currentMachineState = DISABLED;

    lcdPrint("MACHINE DISABLED");

    return errorHex;
}