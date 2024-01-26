#include <LiquidCrystal.h>

// LCD pins
const uint8_t rs = 12;
const uint8_t en = 11;
const uint8_t d4 = 4;
const uint8_t d5 = 5;
const uint8_t d6 = 6;
const uint8_t d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// LCD size variables
const uint8_t lcdColumns = 16;
const uint8_t lcdRows = 2;

// Max lines possible
const uint8_t maxLines = lcdRows - 1;

void lcdSetup(){
    // Define LCD display size
    lcd.begin(lcdColumns, lcdRows);
    lcd.noCursor();
}


// Custom print functions with overloads
// For text only
void lcdPrint(const char* text, bool noClear = false, uint8_t line = 0){
    // Caps the selected line
    if (line > maxLines){
        line = maxLines;
    }

    // Clears the LCD if not set to true
    if (!noClear){
        lcd.clear();
    }

    // Set the cursor to the selected line and print the text
    lcd.setCursor(0, line);
    lcd.print(text);
}

// For a text prefix + hex
void lcdPrint(const char* prefix, char* hex, bool noClear = false, uint8_t line = 0){
    // Caps the selected line
    if (line > maxLines){
        line = maxLines;
    }

    // Clears the LCD if not set to true
    if (!noClear){
        lcd.clear();
    }

    // Set the cursor to the selected line and print the hex and its prefix
    lcd.setCursor(0, line);
    lcd.print(prefix);
    lcd.print(hex);
}

// For integer only
void lcdPrint(int number, bool noClear = false, uint8_t line = 0){
    // Caps the selected line
    if (line > maxLines){
        line = maxLines;
    }

    // Clears the LCD if not set to true
    if (!noClear){
        lcd.clear();
    }

    // Set the cursor to the selected line and print the number
    lcd.setCursor(0, line);
    lcd.print(number);
}

// For a text prefix + integer
void lcdPrint(const char* prefix, int number, bool noClear = false, uint8_t line = 0){
    // Caps the selected line
    if (line > maxLines){
        line = maxLines;
    }

    // Clears the LCD if not set to true
    if (!noClear){
        lcd.clear();
    }

    // Set the cursor to the selected line and print the number with its prefix
    lcd.setCursor(0, line);
    lcd.print(prefix);
    lcd.print(number);
}

// For a text prefix + float
void lcdPrint(const char* prefix, float number, bool noClear = false, uint8_t line = 0){
    // Caps the selected line
    if (line > maxLines){
        line = maxLines;
    }

    // Clears the LCD if not set to true
    if (!noClear){
        lcd.clear();
    }

    // Set the cursor to the selected line and print the number with its prefix
    lcd.setCursor(0, line);
    lcd.print(prefix);
    lcd.print(number);
}