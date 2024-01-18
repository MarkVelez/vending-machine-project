#include <Arduino.h>
#include "headers/motorController.h"

// MDB USB relay pin
const uint8_t relayPin = 2;

// Machine state
machineStates currentMachineState = IDLE;

void setup(){
    motorSetup();

    pinMode(relayPin, INPUT_PULLUP);
}

void loop(){
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