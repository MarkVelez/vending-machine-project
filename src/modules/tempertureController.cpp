#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "temperatureController.h"

// Data pin from the sensor
const uint8_t dataPin = 19;
// Library setups
OneWire data(dataPin);
DallasTemperature sensor(&data);

// Time between requests in minutes
unsigned long requestInterval = 15;
unsigned long tempStartTime = 0;

void temperatureSetup(){
    // Initialize the sensors
    sensor.begin();
}

void readTemperature(){
    // Print the temperature every interval
    if (millis() - tempStartTime >= requestInterval * 60000){
        tempStartTime = millis();

        // Get the temperatures
        sensor.requestTemperatures();
        float temperatureC = sensor.getTempCByIndex(0);

        // Print the temperature to serial
        Serial.print("TEMPERATURE>");
        Serial.println(temperatureC);
    }
}