#ifndef motorController_h
#define motorController_h

// Machine states
enum states{
  IDLE,
  DISPENSING,
  RETURNING,
  MAINTENANCE
};
extern states currentState;

// Function declarations
void motorSetup();
void motorUp();
void motorDown();
void motorStop();
void idleProcess(bool successfulPayment);
void startDispensing();
void dispensingProcess();
void stopDispensing();
void returningProcess();
void maintenanceProcess();

#endif