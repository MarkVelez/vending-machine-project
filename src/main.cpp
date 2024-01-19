#include <Arduino.h>
#include "headers/motorController.h"
#include "headers/temperatureController.h"

// MDB USB relay pin
const uint8_t relayPin = 2;

// Machine state
machineStates currentMachineState = IDLE;

void setup(){
  temperatureSetup();
  motorSetup();

  pinMode(relayPin, INPUT_PULLUP);

  // Connecting to serial monitor
  Serial.begin(9600);
}

void loop(){
  temperatureRead();
  switch (currentMachineState){
    case IDLE:{
      idleProcess(!digitalRead(relayPin));
      break;
    }

    case DISPENSING:{
      dispensingProcess();
      break;
    }

    case MAINTENANCE:{
      maintenanceProcess();
      break;
    }
  
    // Runs if the jars are being sent back down the shaft
    case RETURNING:{
      returningProcess();
      break;
    }
  }
}