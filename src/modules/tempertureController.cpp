#include <Arduino.h>
#include "../headers/temperatureController.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Data pin from the sensor
const unsigned char dataPin = 4;

// Time between requests in minutes
unsigned char requestInterval = 15;

unsigned long startTime = 0;

// Library setups
OneWire data(dataPin);
DallasTemperature sensors(&data);

void temperatureSetup(){
    sensors.begin();
}

void readTemperature(){
    unsigned long currentTime = millis();

    // Print the temperature every interval
    if (currentTime - startTime >= requestInterval * 60000){
        startTime = currentTime;
        sensors.requestTemperatures();
        float temperatureC = sensors.getTempCByIndex(0);
        Serial.print("Temperatures: ");
        Serial.print(temperatureC);
        Serial.print(" C");
    }
}