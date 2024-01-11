#include <Arduino.h>
#include "../headers/errorHandler.h"
#include "../headers/lcdController.h"

int generateErrorCode(bool variableValues[], int errorCodeLength){
    int errorCode = 0;

    for (int i = 0; i < errorCodeLength; i++){
        errorCode |= (variableValues[i] << i);
    }
}

void sendErrorCode(int errorCode, int errorCodeLength){
    for (int i = errorCodeLength; i > 0; i--){
        Serial.println("Error: ");
        Serial.print((errorCode >> i) & 1);
    }
}

void displayErrorCode(int errorCode, int errorCodeLength){
    char error[errorCodeLength + 1];

    for (int i = errorCodeLength; i > 0; i--){
        error[errorCodeLength - 1 - i] = ((errorCode >> i) & 1) + '0';
    }
    error[errorCodeLength] = '\0';

    lcdPrint("Error: ");
    lcdPrint(error);
}