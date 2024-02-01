#include <Arduino.h>
#include "motorController.h"
#include "errorHandler.h"
#include "lcdController.h"
#include "flashController.h"

// Motor variables
const uint8_t motorA1Pin = 25;
const uint8_t motorA2Pin = 26;
uint8_t motorSpeed;
uint8_t returnSpeed = 50;
motorStates currentMotorState = STOPPED;

// Input pins
const uint8_t topSensor = 13;
const uint8_t exitSensor = 12;
const uint8_t bottomSensor = 14;

// Service mode pins
const uint8_t maintenanceModeButton = 18;
const uint8_t upButton = 4;
const uint8_t downButton = 5;
const uint8_t increaseStorage = 32;
const uint8_t decreaseStorage = 33;

// Sensor logging variables
bool topSensorTriggered = false;
bool exitSensorTriggered = false;
bool bottomSensorTriggered = false;

// Time variables
unsigned long startTime;
int sensorTime = 1000;
int motorTimeout = 5000;

// Storage variables
int currentStorage;
int maxStorage = 10;
bool emptyStorage = false;

void motorSetup(){
  currentStorage = getValueFromFlash("currentStorage");

  // Motor pin definitions
  pinMode(motorA1Pin, OUTPUT);
  pinMode(motorA2Pin, OUTPUT);

  // Input definitions
  pinMode(topSensor, INPUT_PULLUP);
  pinMode(exitSensor, INPUT_PULLUP);
  pinMode(bottomSensor, INPUT_PULLUP);
  pinMode(maintenanceModeButton, INPUT_PULLUP);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(increaseStorage, INPUT_PULLUP);
  pinMode(decreaseStorage, INPUT_PULLUP);

  // Stopping the motor just to make sure
  motorStop();
}

// Makes the motor spin backwards
void motorUp(){
  digitalWrite(motorA2Pin, LOW);
  analogWrite(motorA1Pin, motorSpeed);
  currentMotorState = UP;
}

// Makes the motor spin forwards
void motorDown(){
  digitalWrite(motorA1Pin, LOW);
  analogWrite(motorA2Pin, returnSpeed);
  currentMotorState = DOWN;
}

// Makes the motor stop
void motorStop(){
  analogWrite(motorA1Pin, 0);
  analogWrite(motorA2Pin, 0);
  currentMotorState = STOPPED;
}

// Idle process
void idleProcess(bool successfulPayment){
  // Checks for successful payments
  // If there was one start the dispensing process
  if (successfulPayment){
    startDispensing();
  }

  // Checks if storage is empty
  // If yes generates an error code which disables the machine
  if (currentStorage <= 0){
    emptyStorage = true;
    generateErrorCode();
  }

  // If the maintenance button is pressed the machine switches to maintenance mode
  if (digitalRead(maintenanceModeButton) == LOW){
    currentMachineState = MAINTENANCE;
    // Display the current storage
    lcdPrint("Storage: ", currentStorage);
  }
}

// Starts the dispensing process
void startDispensing(){
  currentMachineState = DISPENSING;
  // Setting the motor speed based on how many jars are currently stored
  motorSpeed = map(currentStorage, 1, maxStorage, 50, 240);
  startTime = millis();
  exitSensorTriggered = false;
  topSensorTriggered = false;
  bottomSensorTriggered = false;
  motorUp();
  lcdPrint("Dispensing");
}

// Dispensing process
void dispensingProcess(){
  // Checking for sensor inputs until timout or the exit sensor is triggered
  if (millis() - startTime < sensorTime + motorTimeout && !(topSensorTriggered && exitSensorTriggered)){
    // Checking if the sensor at the top of the shaft has been triggered
    // If yes it stops the motor
    if (digitalRead(topSensor) == LOW){
      // Log to the serial port
      Serial.print("LOG>");
      Serial.println("Top sensor triggered");
      motorStop();
      topSensorTriggered = true;
      delay(50);
    }

    // Checking if the sensor at the exit hole of the vending machine is triggered
    // If yes it initiates the stopping of the dispensing process
    if (digitalRead(exitSensor) == LOW){
      // Log to the serial port
      Serial.print("LOG>");
      Serial.println("Exit sensor triggered");
      exitSensorTriggered = true;
      delay(50);
    }
  }else{
    // Stopping the motor and checking if the dispensing was successful or not
    motorStop();
    delay(50);
    stopDispensing();
  }
}

// Stops the dispensing process
void stopDispensing(){
  // Log to serial port
  Serial.print("LOG>");
  Serial.println("Dispensing finished returning jars");
  motorDown();
  currentMachineState = RETURNING;
  // If the dispensing process was successful
  if (topSensorTriggered && exitSensorTriggered){
    // Log to the serial port
    Serial.print("LOG>");
    Serial.println("Successfully dispensed");
    // Reducing the current storage by one and saving it to flash
    currentStorage--;
    writeValueToFlash("currentStorage", currentStorage);
    lcdPrint("Insert Coin");
    lcdPrint("To Begin", true, 1);
  }else{
    // If the dispensing process was not successful
    Serial.print("LOG>");
    Serial.println("Failed to dispense");
    // Generate error code and send it to the server which disables the machine
    generateErrorCode();
  }
}

// Starts the returning process
void returningProcess(){
  // Checking if the jars have reached the bottom of the shaft
  // If yes stop the motor
  if (digitalRead(bottomSensor) == LOW){
    Serial.print("LOG>");
    Serial.println("Bottom sensor triggered");
    bottomSensorTriggered = true;
    motorStop();
    // If the machine wasn't disabled it sets the state to idle
    if (currentMachineState != DISABLE){
      currentMachineState = IDLE;
    }
    delay(100);
  }
}


// Starts the maintenance process
void maintenanceProcess(){
  // Manual control of the jars
  // If the up button is being held move the jars up
  if (digitalRead(upButton) == LOW && currentMotorState != UP){
    motorUp();
    delay(100);
  // If the down button is being held move the jars down
  }else if (digitalRead(downButton) == LOW && currentMotorState != DOWN){
    motorDown();
    delay(100);
  // If neither are being held stop the motor
  }else if (currentMotorState != STOPPED){
    motorStop();
  }

  // Manual storage control
  // If the increase button is pressed add one to current storage
  if (digitalRead(increaseStorage) == LOW && currentStorage < maxStorage){
    currentStorage++;
    delay(100);
  }
  // If the decrease button is pressed remove one from current storage
  if (digitalRead(decreaseStorage) == LOW && currentStorage > 0){
    currentStorage--;
    delay(100);
  }

  // If the maintenance button is pressed in maintenance mode it exits maintenance mode
  if (digitalRead(maintenanceModeButton) == LOW){
    currentMachineState = IDLE;

    // Reset all error bools
    connectionFailed = false;
    requestFailed = false;
    topSensorTriggered = false;
    exitSensorTriggered = false;
    bottomSensorTriggered = false;
    emptyStorage = false;

    // Save the storage value to flash
    writeValueToFlash("currentStorage", currentStorage);

    // Reset the LCD display text
    lcdPrint("Insert Coin");
    lcdPrint("To Begin", true, 1);
  }
}

// Disabled process
void disableProcess(){
  // If the maintenance button is pressed the machine switches to maintenance mode
  if (digitalRead(maintenanceModeButton) == LOW){
    currentMachineState = MAINTENANCE;
    // Display the current storage as well as the error code
    lcdPrint("Storage: ", currentStorage);
    lcdPrint("Error: ", errorHex, true, 1);
    delay(100);
  }
  
  // Stop the motor if its at the bottom of the shaft
  if (!bottomSensorTriggered){
    returningProcess();
  }
}