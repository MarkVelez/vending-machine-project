#ifndef lcdController_h
#define lcdController_h

// Function declarations
void lcdSetup();
void lcdPrint(char text[], uint8_t line = 0);
void lcdPrint(char prefix[], char text[], uint8_t line = 0);
void lcdPrint(int number, uint8_t line = 0);
void lcdPrint(char prefix[], int number, uint8_t line = 0);

#endif