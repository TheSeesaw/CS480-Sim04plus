#ifndef PCBUTILS_H
#define PCBUTILS_H


// Function prototypes
void organizeBySchedule(struct Config theData, int numProcesses, struct ProcessControlBlock* pcbDestination, int* orderDestination);
int getNumberOfPCBs(struct Action* theActions, int actionCount);
int calculateCycleTime(struct Action theAction, int pCycleTime, int ioCycleTime);
void initializePCBArray(int numProcesses, struct ProcessControlBlock* pcbDestination, int numActions, struct Action* theActions, struct Config theData);


#endif
