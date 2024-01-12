#include <LiquidCrystal.h>

// LCD pins
const unsigned char rs = 12;
const unsigned char en = 11;
const unsigned char d4 = 4;
const unsigned char d5 = 5;
const unsigned char d6 = 6;
const unsigned char d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void lcdSetup(){
    lcd.begin(16, 2);
    lcd.noCursor();
}

void lcdPrint(char text[]){
    lcd.clear();
    delay(100);
    lcd.print(text);
}