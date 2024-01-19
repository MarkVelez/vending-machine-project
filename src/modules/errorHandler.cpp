#include <Arduino.h>
#include "../headers/errorHandler.h"
#include "../headers/lcdController.h"

int errorCode;
int errorCodeLength;

int generateErrorCode(bool variableValues[]){
    errorCode = 0;
    errorCodeLength = sizeof(variableValues) / sizeof(variableValues[0]);

    for (int i = 0; i < errorCodeLength; i++){
        errorCode |= (variableValues[i] << i);
    }

    return errorCode;
}

void sendErrorCode(){
    Serial.println("Error: ");
    for (int i = errorCodeLength; i > 0; i--){
        Serial.print((errorCode >> i) & 1);
    }
    Serial.println();
}

void displayErrorCode(){
    char error[errorCodeLength + 1];

    for (int i = errorCodeLength; i > 0; i--){
        error[errorCodeLength - 1 - i] = ((errorCode >> i) & 1) + '0';
    }
    error[errorCodeLength] = '\0';

    lcdPrint("Error: ", error);
}