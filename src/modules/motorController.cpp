#include <Arduino.h>
#include "../headers/motorController.h"
#include "../headers/errorHandler.h"
#include "../headers/lcdController.h"
#include "../headers/flashController.h"

// Motor variables
const uint8_t motorA1Pin = 26;
const uint8_t motorA2Pin = 27;
uint8_t motorSpeed = 50;
motorStates currentMotorState = STOPPED;

// Input pins
const uint8_t topSensor = 12;
const uint8_t exitSensor = 14;
const uint8_t bottomSensor = 27;

// Sensor logging variables
bool topSensorTriggered = false;
bool exitSensorTriggered = false;
bool bottomSensorTriggered = false;

// Time variables
unsigned long startTime;
int sensorTime = 1000;
int motorTimeout = 5000;

// Service mode pins
const uint8_t maintenanceModeButton = 6;
const uint8_t upButton = 7;
const uint8_t downButton = 8;
const uint8_t increaseStorage = 9;
const uint8_t decreaseStorage = 10;


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
  analogWrite(motorA2Pin, 50);
  currentMotorState = DOWN;
}

// Makes the motor stop
void motorStop(){
  analogWrite(motorA1Pin, 0);
  analogWrite(motorA2Pin, 0);
  currentMotorState = STOPPED;
}

// Starts the dispensing process
void startDispensing(){
  currentMachineState = DISPENSING;
  // Setting the motor speed based on how many jars are currently stored
  motorSpeed = map(currentStorage, 1, maxStorage, 50, 240);
  startTime = millis();
  exitSensorTriggered = false;
  topSensorTriggered = false;
  motorUp();
}

void dispensingProcess(){
  unsigned long currentTime = millis();

  // Checking for sensor inputs until timout or the exit sensor is triggered
  if (currentTime - startTime < sensorTime + motorTimeout || !exitSensorTriggered){
    // Checking if the sensor at the top of the shaft has been triggered
    // If yes it stops the motor
    if (digitalRead(topSensor) == LOW){
      Serial.println("Top sensor triggered");
      motorStop();
      topSensorTriggered = true;
      delay(50);
    }

    // Checking if the sensor at the exit hole of the vending machine is triggered
    // If yes it initiates the stopping of the dispensing process
    if (digitalRead(exitSensor) == LOW){
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
  // If the dispensing process was successful
  if (topSensorTriggered && exitSensorTriggered){
    Serial.println("Successfully dispensed");
    currentMachineState = IDLE;
    // Reducing the current storage by one and lowering the jars back down the shaft
    currentStorage--;
    writeValueToFlash("currentStorage", currentStorage);
    motorDown();
  }else{
    // If the dispensing process was not successful
    Serial.println("Failed to dispense");
    // Generate error code and send it to the server which disables the machine
    generateErrorCode();
    // Sending the jars back down without removing from the current storage
    motorDown();
  }
  currentMachineState = RETURNING;
  Serial.println("Returning");
}

// Starts the returning process
void returningProcess(){
  // Checking if the jars have reached the bottom of the shaft
  // If yes stop the motor
  if (digitalRead(bottomSensor) == LOW){
    Serial.println("Bottom sensor triggered");
    motorStop();
    currentMachineState = IDLE;
    delay(50);
  }
}


// Starts the maintenance process
void maintenanceProcess(){
  // If the maintenance button is pressed in maintenance mode it exits maintenance mode
  if (digitalRead(maintenanceModeButton) == LOW){
    currentMachineState == IDLE;
    bool connectionFailed = false;
    bool requestFailed = false;
    bool topSensorTriggered = false;
    bool exitSensorTriggered = false;
    bool bottomSensorTriggered = false;
    bool emptyStorage = false;
    writeValueToFlash("currentStorage", currentStorage);
  }

  if (digitalRead(upButton) == LOW && currentMotorState != UP){
    motorUp();
  }else if (digitalRead(downButton) == LOW && currentMotorState != DOWN){
    motorDown();
  }else if (currentMotorState != STOPPED){
    motorStop();
  }

  if (digitalRead(increaseStorage) == LOW && currentStorage < maxStorage){
    currentStorage++;
  }

  if (digitalRead(decreaseStorage) == LOW){
    currentStorage--;
  }
  
  delay(50);
}

void idleProcess(bool successfulPayment){
  // Checks for successful payments
  // If there was one start the dispensing process
  if (successfulPayment){
    startDispensing();
  }

  // Checks if storage is empty
  // If yes generates an error code and sends it to the server which disables the machine
  if (currentStorage <= 0){
    emptyStorage = true;
    generateErrorCode();
  }

  // If the maintenance button is pressed the machine switches to maintenance mode
  if (digitalRead(maintenanceModeButton) == LOW){
    currentMachineState = MAINTENANCE;
    lcdPrint("Storage: ", currentStorage);
  }
}