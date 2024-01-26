#ifndef lcdController_h
#define lcdController_h

// Function declarations
void lcdSetup();

// Custom print functions with overloads
// For text only
void lcdPrint(const char* text, bool noClear = false, uint8_t line = 0);
// For a text prefix + hex
void lcdPrint(const char* prefix, char* hex, bool noClear = false, uint8_t line = 0);
// For integer only
void lcdPrint(int number, bool noClear = false, uint8_t line = 0);
// For a text prefix + integer
void lcdPrint(const char* prefix, int number, bool noClear = false, uint8_t line = 0);
// For text prefix + float
void lcdPrint(const char* prefix, float number, bool noClear = false, uint8_t line = 0);

#endif