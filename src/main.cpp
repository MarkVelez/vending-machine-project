#include <Arduino.h>
#include "headers/motorController.h"
#include "headers/temperatureController.h"
#include "headers/serialHandler.h"
#include "headers/paymentHandler.h"

// Machine state
machineStates currentMachineState = IDLE;

// Error bool
bool serialConnected = false;

void setup(){
  // Setups for included modules
  serialSetup();
  paymentSetup();
  temperatureSetup();
  motorSetup();
}

void loop(){
  // Attempt to establish serial connection
  if (establishConnection && !serialConnected){
    serialConnected = true;
  }

  // If connection was succesful operate normally
  if (serialConnected){
    temperatureRead();
    switch (currentMachineState){
      // The idle state of the machine
      case IDLE:{
        idleProcess(paymentProcessing());
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

      // Runs if there was an error and the machine was disabled
      case DISABLE:{
        disableProcess();
        break;
      }
    }
  }
}