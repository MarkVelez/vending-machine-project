#include <Arduino.h>
#include "errorHandler.h"
#include "motorController.h"
#include "lcdController.h"

// Definition of the global variable
char errorHex[3];

void generateErrorCode(){
    // Resetting the unformated error code
    int errorCode = 0;
    // Getting all the error bool values
    bool errorBools[] = {
    connectionFailed, 
    requestFailed, 
    emptyStorage, 
    topSensorTriggered, 
    exitSensorTriggered, 
    bottomSensorTriggered
    };
    int errorCodeLength = sizeof(errorBools) / sizeof(errorBools[0]);

    // Add all the error bool values to the unformated error code
    for (int i = 0; i < errorCodeLength; i++){
        errorCode |= (errorBools[i] << i);
    }

    // Format the error code to HEX and print it to serial
    sprintf(errorHex, "%02X", errorCode);
    Serial.print("ERROR>");
    Serial.println(errorHex);

    // Disable the machine
    currentMachineState = DISABLE;
    lcdPrint("MACHINE DISABLED");
}