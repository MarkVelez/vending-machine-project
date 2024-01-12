#include <Arduino.h>
#include "../headers/motorController.h"
#include "../headers/errorHandler.h"
#include "../headers/lcdController.h"

// Motor variables
uint8_t motorA1Pin = 26;
uint8_t motorA2Pin = 27;
unsigned char motorSpeed = 50;

// Input pins
uint8_t topSensor = 3;
uint8_t exitSensor = 4;
uint8_t bottomSensor = 5;

// Sensor logging variables
bool topSensorTriggered = false;
bool exitSensorTriggered = false;
bool bottomSensorTriggered = false;

// Time variables
unsigned long startTime;
int sensorTime = 1000;
int motorTimeout = 5000;

// Service mode pins
uint8_t maintenanceModeButton = 6;
uint8_t actionButton = 7;

// Storage variables
unsigned char currentStorage = 10;
unsigned char maxStorage = 10;
bool emptyStorage = false;

// Error variables
int errorCode = 0;
const int errorLength = 4;
bool errorBools[errorLength] = {topSensorTriggered, exitSensorTriggered, bottomSensorTriggered, emptyStorage};

void motorSetup(){
  // Motor pin definitions
  pinMode(motorA1Pin, OUTPUT);
  pinMode(motorA2Pin, OUTPUT);

  // Input definitions
  pinMode(topSensor, INPUT_PULLUP);
  pinMode(exitSensor, INPUT_PULLUP);
  pinMode(bottomSensor, INPUT_PULLUP);

  // Stopping the motor just to make sure
  motorStop();

  // Connecting to serial monitor
  Serial.begin(9600);
}

// Makes the motor spin backwards
void motorUp(){
  digitalWrite(motorA2Pin, LOW);
  analogWrite(motorA1Pin, motorSpeed);
}

// Makes the motor spin forwards
void motorDown(){
  digitalWrite(motorA1Pin, LOW);
  analogWrite(motorA2Pin, 50);
  currentState = RETURNING;
  Serial.println("Returning");
}

// Makes the motor stop
void motorStop(){
  analogWrite(motorA1Pin, 0);
  analogWrite(motorA2Pin, 0);
}

// Starts the dispensing process
void startDispensing(){
  currentState = DISPENSING;
  // Setting the motor speed based on how many jars are currently stored
  motorSpeed = map(currentStorage, 1, maxStorage, 50, 240);
  startTime = millis();
  exitSensorTriggered = false;
  topSensorTriggered = false;
  motorUp();
}

void dispensingProcess(){
  long currentTime = millis();

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
    currentState = IDLE;
    // Reducing the current storage by one and lowering the jars back down the shaft
    currentStorage--;
    motorDown();
  }else{
    // If the dispensing process was not successful
    Serial.println("Failed to dispense");
    // Generate error code and send it to the server which disables the machine
    errorCode = generateErrorCode(errorBools, errorLength);
    sendErrorCode(errorCode, errorLength);
    // Sending the jars back down without removing from the current storage
    motorDown();
  }
}

// Starts the returning process
void returningProcess(){
  // Checking if the jars have reached the bottom of the shaft
  // If yes stop the motor
  if (digitalRead(bottomSensor) == LOW){
    Serial.println("Bottom sensor triggered");
    motorStop();
    currentState = IDLE;
    delay(50);
  }
}


// Starts the maintenance process
void maintenanceProcess(){
  // If the maintenance button is pressed in maintenance mode it exits maintenance mode
  if (digitalRead(maintenanceModeButton) == LOW){
    currentState == IDLE;

    // If there was an error, unset the errorCode and enable the machine
    if (errorCode != -1){
      errorCode = -1;
      Serial.print("Error: ");
      Serial.println(errorCode);
    }
  }

  if 
}

void idleProcess(bool successfulPayment){
  // Checks for successful payments
  // If there was one start the dispensing process
  if (successfulPayment){
    startDispensing();
  }

  // Checks if storage is empty
  // If yes generates an error code and sends it to the server which disables the machine
  if (currentStorage <= 0 && errorCode == -1){
    emptyStorage = true;
    Serial.println("Storage empty");
    lcdPrint("Machine is disabled!");
  }

  // If the maintenance button is pressed the machine switches to maintenance mode
  if (digitalRead(maintenanceModeButton) == LOW){
    currentState = MAINTENANCE;

    // If there was an error, display the error code on the lcd
    if (errorCode != -1){
      displayErrorCode(errorCode, errorLength);
    }
  }
}