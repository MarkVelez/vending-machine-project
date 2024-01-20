#include <Arduino.h>
#include "../headers/serialHandler.h"

bool connectionFailed = false;
bool requestFailed = false;
int connectionAttempts = 0;

void serialSetup(){
    Serial.begin(9600);
}

bool establishConnection(){
    Serial.println("CONNECTION_ATTEMPT");
    delay(1000);
    if (connectionAttempts < 3){
        if (Serial.available() > 0 && Serial.readStringUntil('\n') == "CONNECTION_ATTEMPT_SUCCESSFUL"){
            Serial.println("CONNECTION_ESTABLISHED");
            connectionAttempts = 0;
            return true;
        }else{
            connectionAttempts++;
            return false;
            delay(10000);
        }
    }else{
        connectionFailed = true;
        generateErrorCode();
    }
}

String requestData(requestTypes requestType){
    String data;
    Serial.println(requestType);
    delay(1000);
    if (Serial.available() > 0){
      data = Serial.readStringUntil('\n');
      return data;
    }else{
       data = "ERROR";
       requestFailed = true;
       generateErrorCode();
       return data;
    }
}