#ifndef motorController_h
#define motorController_h

extern unsigned char currentStorage;

// Machine states
enum machineStates{
  IDLE,
  DISPENSING,
  RETURNING,
  MAINTENANCE,
  DISABLED
};
extern machineStates currentMachineState;

// Motor states
enum motorStates{
  UP,
  DOWN,
  STOPPED
};
extern motorStates currentMotorState;

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