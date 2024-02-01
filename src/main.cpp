#include <Arduino.h>
#include "motorController.h"
#include "temperatureController.h"
#include "serialHandler.h"
#include "paymentHandler.h"
#include "lcdController.h"

// Machine state
machineStates currentMachineState = IDLE;

// For knowing if a serial connection is established
bool serialConnected = false;

void setup(){
  // Setups for included modules
  serialSetup();
  paymentSetup();
  lcdSetup();
  temperatureSetup();
  motorSetup();
  lcdPrint("INITIALIZING");
}

void loop(){
  // Attempt to establish serial connection
  if (establishConnection() && !serialConnected){
    serialConnected = true;
    lcdPrint("Insert Coin");
    lcdPrint("To Begin", true, 1);
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