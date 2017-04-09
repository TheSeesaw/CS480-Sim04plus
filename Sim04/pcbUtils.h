#ifndef PCBUTILS_H
#define PCBUTILS_H


// Function prototypes
void organizeByNSchedule(struct Config theData, int numProcesses, struct ProcessControlBlock* pcbDestination, int* orderDestination);
int getNumberOfPCBs(struct Action* theActions, int actionCount);
int calculateCycleTime(struct Action theAction, int pCycleTime, int ioCycleTime);
void initializePCBArray(int numProcesses, struct ProcessControlBlock* pcbDestination, int numActions, struct Action* theActions, struct Config theData);
void runProgramByNSchedule(struct ProcessControlBlock* pcbArray, int numPCBs, struct Config data, struct Action* actions, char* logString);
void ioThreadRoutine(void* actionMSptr);


#endif
