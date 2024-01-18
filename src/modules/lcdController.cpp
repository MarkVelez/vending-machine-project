#include <LiquidCrystal.h>

// LCD pins
const uint8_t rs = 12;
const uint8_t en = 11;
const uint8_t d4 = 4;
const uint8_t d5 = 5;
const uint8_t d6 = 6;
const uint8_t d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void lcdSetup(){
    lcd.begin(16, 2);
    lcd.noCursor();
}

void lcdPrint(char text[], uint8_t line = 0){
    const uint8_t maxLine = 1;

    if (line > maxLine){
        line = maxLine;
    }

    lcd.clear();
    lcd.setCursor(0, line);
    delay(100);
    lcd.print(text);
}