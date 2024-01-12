#include <Arduino.h>
#include "headers/motorController.h"

// MDB USB relay pin
uint8_t relayPin = 2;

// Machine state
states currentState = IDLE;

void setup(){
    motorSetup();

    pinMode(relayPin, INPUT_PULLUP);
}

void loop(){
  switch (currentState){
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