#ifndef lcdController_h
#define lcdController_h

// Function declarations
void lcdSetup();
void lcdPrint(const char* text, bool noClear = false, uint8_t line = 0);
void lcdPrint(const char* prefix, char* hex, bool noClear = false, uint8_t line = 0);
void lcdPrint(int number, bool noClear = false, uint8_t line = 0);
void lcdPrint(const char* prefix, int number, bool noClear = false, uint8_t line = 0);
void lcdPrint(const char* prefix, float number, bool noClear = false, uint8_t line = 0);

#endif