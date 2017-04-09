// Make sure not to compile structures more than once
#ifndef STRUCTURES_H
#define STRUCTURES_H

#define STD_I_QUEUE_LEN 10
// Structure used to hold the data of an individual action from a program
struct Action
{

    char commandLetter;
    char operationString[16];
    int cycleTime;

};

// Structure used to hold all necessary data read in from a configuration file
struct Config
{

    int      version;
    char     filePath[64];
    char     schCode[16];
    int      quanTime;
    long int memAvail;
    int      pTime;
    int      ioTime;
    char     logTo[16];
    char     logPath[64];

};

// Structure used to hold all the context for a process
struct ProcessControlBlock
{
    int pState;
    int pNum;
    int pPriority;
    int msToCompletion;
    int numAssociatedActions;
    int startActionIndex;
    int endActionIndex;
};

// Queue for holding interrupts
struct IQueue
{
  void* queue[STD_I_QUEUE_LEN];
};

#endif
