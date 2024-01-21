#include <Arduino.h>
#include "headers/motorController.h"
#include "headers/temperatureController.h"
#include "headers/serialHandler.h"

// MDB USB relay pin
const uint8_t relayPin = 26;

// Machine state
machineStates currentMachineState = IDLE;

bool serialConnected = false;

void setup(){
  // Setups for included modules
  serialSetup();
  temperatureSetup();
  motorSetup();

  pinMode(relayPin, INPUT_PULLUP);
}

void loop(){
  if (establishConnection && !serialConnected){
    serialConnected = true;
  }

  if (serialConnected){
    temperatureRead();
    switch (currentMachineState){
      // The idle state of the machine
      case IDLE:{
        idleProcess(!digitalRead(relayPin));
        break;
      }
      // Runs once a successful payment has gone through
      case DISPENSING:{
        dispensingProcess();
        break;
      }

      // Runs when maintenance mode is toggled on
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
}