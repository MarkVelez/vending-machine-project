#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "temperatureController.h"

// Data pin from the sensor
uint8_t dataPin = 4;

// Time between requests in minutes
unsigned char requestInterval = 15;

unsigned long tempStartTime = 0;

// Library setups
OneWire data(dataPin);
DallasTemperature sensor(&data);

void temperatureSetup(){
    // Initialize the sensors
    sensor.begin();
}

void readTemperature(){
    // Variable to keep track of the time since boot in milliseconds
    unsigned long currentTime = millis();

    // Print the temperature every interval
    if (currentTime - tempStartTime >= requestInterval * 60000){
        tempStartTime = currentTime;

        // Get the temperatures
        sensor.requestTemperatures();
        float temperatureC = sensor.getTempCByIndex(0);

        // Print the temperature to serial
        Serial.print("TEMPERATURE>");
        Serial.println(temperatureC);
    }
}