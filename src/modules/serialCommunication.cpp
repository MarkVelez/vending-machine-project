#include <Arduino.h>
#include "serialHandler.h"
#include "errorHandler.h"

// Error variables
int connectionAttempts = 0;
bool requestFailed = false;
bool connectionFailed = false;

// Setup for serial connection
void serialSetup(){
    Serial.begin(115200);
}

// Process for establishing serial connection
bool establishConnection(){
    // Send a connection attempt request to serial
    Serial.println("CONNECTION_ATTEMPT");
    // Wait one second for response
    delay(1000);
// Keep trying until 3 attempts have been performed
if (connectionAttempts < 3){
    // If there was a response
    if (Serial.available() > 0 && 
    Serial.readStringUntil('\n') == "CONNECTION_ATTEMPT_SUCCESSFUL"){
        Serial.println("CONNECTION_ESTABLISHED");
        connectionAttempts = 0;
        return true;
    }else{
        // Wait 10 seconds before trying again
        connectionAttempts++;
        return false;
        delay(10000);
    }
    }else{
        // Disable the machine after 3 failed attempts
        connectionFailed = true;
        generateErrorCode();
        return false;
    }
}

// Function to request data from RPI
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