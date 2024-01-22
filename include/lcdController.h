#ifndef lcdController_h
#define lcdController_h

// Function declarations
void lcdSetup();
void lcdPrint(const char* text, uint8_t line = 0, bool noClear = false);
void lcdPrint(const char* prefix, char* hex, uint8_t line = 0, bool noClear = false);
void lcdPrint(int number, uint8_t line = 0, bool noClear = false);
void lcdPrint(const char* prefix, int number, uint8_t line = 0, bool noClear = false);
void lcdPrint(const char* prefix, float number, uint8_t line = 0, bool noClear = false);

#endif