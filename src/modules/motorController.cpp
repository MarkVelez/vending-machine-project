#include "../headers/motorController.h"

// Motor variables
const unsigned char motorA1 = 10;
const unsigned char motorA2 = 11;
unsigned char motorSpeed = 50;

// Input pins
const unsigned char button = 2;
const unsigned char topSensor = 3;
const unsigned char exitSensor = 4;
const unsigned char bottomSensor = 5;

// Output variables
bool dispensing = false;
bool topSensorTriggered = false;
bool exitSensorTriggered = false;
bool returning = false;
bool hasProblem = false;

// Time variables
unsigned long startTime;
int sensorTime = 1000;
int motorTimeout = 5000;
int timeTaken;

// Service mode variables
bool serviceMode = false;
const unsigned char serviceModeButton = 6;
const unsigned char  = 7;

// Storage variables
unsigned char currentStorage = 10;
unsigned char maxStorage = 10;

void motorSetup(){
  // Motor pin definitions
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);

  // Input definitions
  pinMode(button, INPUT_PULLUP);
  pinMode(topSensor, INPUT_PULLUP);
  pinMode(exitSensor, INPUT_PULLUP);
  pinMode(bottomSensor, INPUT_PULLUP);

  // Stopping the motor just to make sure
  motorStop();

  // Connecting to serial monitor
  Serial.begin(9600);
}

void motorLoop(){
  // While the dispensing process is ongoing
  if (dispensing && !serviceMode){
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
  }else{
    // Checking for requests if dispensing is not in process and there are no issues
    if (digitalRead(button) == LOW && !hasProblem){
      startDispensing();
    }
  }
  
  // Runs if the jars are being sent back down the shaft
  if (returning){
    // Checking if the jars have reached the bottom of the shaft
    // If yes stop the motor
    if (digitalRead(bottomSensor) == LOW){
      Serial.println("Bottom sensor triggered");
      motorStop();
      returning = false;
      delay(50);
    }
  }
}

// Makes the motor spin backwards
void motorUp(){
  digitalWrite(motorA2, LOW);
  analogWrite(motorA1, motorSpeed);
}

// Makes the motor spin forwards
void motorDown(){
  digitalWrite(motorA1, LOW);
  analogWrite(motorA2, 50);
  returning = true;
  Serial.println("Returning");
}

// Makes the motor stop
void motorStop(){
  analogWrite(motorA1, 0);
  analogWrite(motorA2, 0);
}

// Starts the dispensing process
void startDispensing(){
  dispensing = true;
  // Setting the motor speed based on how many jars are currently stored
  motorSpeed = map(currentStorage, 1, maxStorage, 50, 240);
  startTime = millis();
  exitSensorTriggered = false;
  topSensorTriggered = false;
  motorUp();
}

// Stops the dispensing process
void stopDispensing(){
  // If the dispensing process was successful
  if (topSensorTriggered && exitSensorTriggered){
    Serial.println("Successfully dispensed");

    dispensing = false;

    // Reducing the current storage by one and lowering the jars back down the shaft
    currentStorage--;
    motorDown();
  }else{
    // If the dispensing process was not successful
    Serial.println("Failed to dispense");
    dispensing = false;
    // Disables the machine
    hasProblem = true;
    // Sending the jars back down without removing from the current storage
    motorDown();
}