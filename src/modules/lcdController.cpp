#include <LiquidCrystal.h>
#include <string>

// LCD pins
const uint8_t rs = 12;
const uint8_t en = 11;
const uint8_t d4 = 4;
const uint8_t d5 = 5;
const uint8_t d6 = 6;
const uint8_t d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void lcdSetup(){
    // Define lcd display size
    lcd.begin(16, 2);
    lcd.noCursor();
}


// Custom print functions with overloads
void lcdPrint(char text[], uint8_t line = 0){
    const uint8_t maxLine = 1;

    if (line > maxLine){
        line = maxLine;
    }

    lcd.clear();
    lcd.setCursor(0, line);
    lcd.print(text);
}

void lcdPrint(char prefix[], char text[], uint8_t line = 0){
    const uint8_t maxLine = 1;

    if (line > maxLine){
        line = maxLine;
    }

    std::string fullMessage = std::string(prefix) + std::string(text);

    lcd.clear();
    lcd.setCursor(0, line);
    lcd.print(fullMessage.c_str());
}

void lcdPrint(int number, uint8_t line = 0){
    const uint8_t maxLine = 1;

    if (line > maxLine){
        line = maxLine;
    }

    lcd.clear();
    lcd.setCursor(0, line);
    lcd.print(number);
}

void lcdPrint(char prefix[], int number, uint8_t line = 0){
    const uint8_t maxLine = 1;

    if (line > maxLine){
        line = maxLine;
    }

    std::string fullMessage = std::string(prefix) + std::to_string(number);

    lcd.clear();
    lcd.setCursor(0, line);
    lcd.print(fullMessage.c_str());
}