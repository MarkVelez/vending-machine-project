#include <Arduino.h>
#include "lcdController.h"

// Where the data pin of the coin acceptor is connected
uint8_t coinPin = 19;

// The price of the product
float productPrice = 8.50;

// Coin values
float coinValues[] = {0, 2, 1, 0.5, 0.2, 0.1};

// Process variables
int impulseCount = 0;
float totalAmount = 0;

// Time variables
unsigned long coinInsertTime;
const int impulseTime = 100;

// Called whenever the coin acceptor sends an impulse
void incomingImpulse(){
    impulseCount++;
    coinInsertTime = millis();
}

// Setup for the payment handler
void paymentSetup(){
    // Call the function when there is an impulse sent
    attachInterrupt(coinPin, incomingImpulse, FALLING);
}

// Payment handler loop
bool paymentProcessing(){
    // If the full price has not been paid yet
    if (totalAmount < productPrice){
        // If there is a coin insertion
        if (impulseCount > 0 && millis() - coinInsertTime > impulseTime){
            // Add an amount based on the amount of impulses
            totalAmount += coinValues[impulseCount];
            // Tell the customer their total amount inserted
            lcdPrint("Total: ", totalAmount);
            impulseCount = 0;
        }
        return false;
    }else{
        // If the full price has been paid start the vending process
        totalAmount = 0;
        return true;
    }
}