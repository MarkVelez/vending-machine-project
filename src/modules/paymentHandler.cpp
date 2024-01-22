#include <Arduino.h>
#include "../headers/lcdController.h"

// Where the data pin of the coin acceptor is connected
uint8_t coinPin = 19;

// The price of the product
float productPrice = 8.50;

// Coin values
float coinValues[] = {0, 2, 1, 0.5, 0.2, 0.1};

// Process variables
int i = 0;
int impulseCount = 0;
float totalAmount = 0;
bool coinInserted = false;

// Called whenever the coin acceptor sends an impulse
void incomingImpulse(){
    impulseCount++;
    coinInserted = true;
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
        while (coinInserted){
            i++;
            // After 30 intervals check the amount of impulses
            if (i >= 30){
                // Add an amount based on the amount of impulses
                totalAmount += coinValues[impulseCount];
                // Tell the customer their total amount inserted
                lcdPrint("Total: ", totalAmount);
                i = 0;
                impulseCount = 0;
                coinInserted = false;
            }
        }
        return false;
    }else{
        // If the full price has been paid start the vending process
        totalAmount = 0;
        return true;
    }
}