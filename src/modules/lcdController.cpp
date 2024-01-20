#include <LiquidCrystal.h>

// LCD pins
const uint8_t rs = 12;
const uint8_t en = 11;
const uint8_t d4 = 4;
const uint8_t d5 = 5;
const uint8_t d6 = 6;
const uint8_t d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const uint8_t maxLine = 1;

void lcdSetup(){
    // Define lcd display size
    lcd.begin(16, 2);
    lcd.noCursor();
}


// Custom print functions with overloads
void lcdPrint(const char* text, uint8_t line = 0){
    if (line > maxLine){
        line = maxLine;
    }

    lcd.clear();
    lcd.setCursor(0, line);
    lcd.print(text);
}

void lcdPrint(const char* prefix, char* hex, uint8_t line = 0){
    if (line > maxLine){
        line = maxLine;
    }

    lcd.clear();
    lcd.setCursor(0, line);
    lcd.print(prefix);
    lcd.print(hex);
}

void lcdPrint(int number, uint8_t line = 0){
    if (line > maxLine){
        line = maxLine;
    }

    lcd.clear();
    lcd.setCursor(0, line);
    lcd.print(number);
}

void lcdPrint(const char* prefix, int number, uint8_t line = 0){
    if (line > maxLine){
        line = maxLine;
    }

    lcd.clear();
    lcd.setCursor(0, line);
    lcd.print(prefix);
    lcd.print(number);
}