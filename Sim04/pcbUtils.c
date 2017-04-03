#include "structures.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// This function returns the number of PCBs that will be created from one file
int getNumberOfPCBs(struct Action* theActions, int actionCount)
{
   // Initialize values for looping through actions
   int actionIndex = 0;
   int numPCBs = 0;
   for (; actionIndex < actionCount; actionIndex++)
   {
      // Check for process start
      if (theActions[actionIndex].commandLetter == 'A' && strcmp(theActions[actionIndex].operationString, "start") == 0)
      {
         // Increment numProcesses
         numPCBs++;
      }
   }
   return numPCBs;
}

// This function takes in an action struct and returns the cycle time in ms based on the type of action
int calculateCycleTime(struct Action theAction, int pCycleTime, int ioCycleTime)
{
   int calculatedCycleTime = 0;
   switch(theAction.commandLetter)
   {
      case 'M' :
         // Arbitrary value for memory actions
         calculatedCycleTime = 10;
         break;
      case 'I' :
         // Input
         calculatedCycleTime = theAction.cycleTime * ioCycleTime;
         break;
      case 'O' :
         // Output
         calculatedCycleTime = theAction.cycleTime * ioCycleTime;
         break;
      case 'P' :
         // CPU cycle time
         calculatedCycleTime = theAction.cycleTime * pCycleTime;
         break;
      case 'A' :
         // Arbitrary value for application start/end
         calculatedCycleTime = 0;
         break;
      default:
         return -1;
   }
   return calculatedCycleTime;
}

/* This function takes in the number of PCBs to create, an empty PCB array,
 * the number of total actions for all processes, an array of all actions for all processes,
 * and the config structure for the file
 *
 * It fills each PCB in the array with its inital values
*/
void initializePCBArray(int numProcesses, struct ProcessControlBlock* pcbArray, int numActions, struct Action* theActions, struct Config theData)
{
   // Initialize pcbIndex
   int pcbIndex = 0;
   int actionIndex = 1;
   int currentPCBActions;
   struct ProcessControlBlock newPCB;
   // Initialize each pcb in the array
   for (; pcbIndex < numProcesses; pcbIndex++)
   {
      // Initialize with new state
      newPCB.pState = 0;
      // Assign a process number
      newPCB.pNum = pcbIndex;
      // Initialize msToCompletion
      newPCB.msToCompletion = 0;
      // Set/Reset currentPCBActions
      currentPCBActions = 0;
      // Check that the next action is the start of a new process
      if (theActions[actionIndex].commandLetter == 'A' && strcmp(theActions[actionIndex].operationString, "start") == 0)
      {
         // Set the startActionIndex to the index of the next A(start)0 action
         newPCB.startActionIndex = actionIndex;
         // Add to current actions for the start action
         currentPCBActions++;
         // No need to add time for the start action because it's time = 0
         // Increment the Action Index
         actionIndex++;
         // Loop until the index of the next A(end)0 action
         while (theActions[actionIndex].commandLetter != 'A' && strcmp(theActions[actionIndex].operationString, "end") != 0)
         {
            // Increment the Action index

            // Add to current actions for the intermediary actions
            currentPCBActions++;
            // Add the ms for that action
            newPCB.msToCompletion += calculateCycleTime(theActions[actionIndex], theData.pTime, theData.ioTime);
            actionIndex++;
         }
         // Set the endActionIndex to the index of the next A(end)0 action
         newPCB.endActionIndex = actionIndex;
         // Add to current actions for the end action
         currentPCBActions++;
         // No need to add time for the end action because it's time = 0
         // Set numAssociatedActions for current PCB
         newPCB.numAssociatedActions = currentPCBActions;
      }
      // Increment to set index to next A(start)0 action for next loop
      actionIndex++;
      // Store new PCB in array
      pcbArray[pcbIndex] = newPCB;
   }
}

// This function takes the pcb array, the config data, and an integer array wth size = number of PCBs
// and sorts the ids of each pcb based on the scheduling code
void organizeBySchedule(struct Config theData, int numProcesses, struct ProcessControlBlock* pcbDestination, int* orderDestination)
{
   // initialize the array
   int sortArrayCount = 0;

   if (strcmp(theData.schCode, "SJF-N") == 0)
   {
      // Sort by shortest job first, nonpreemptive
      // Bubble sort
      // Store the time values in an array for sorting
      for (; sortArrayCount < numProcesses; sortArrayCount++)
      {
         orderDestination[sortArrayCount] = pcbDestination[sortArrayCount].msToCompletion;
      }
      // Bubble sort
      int leftSortCount = 0;
      int rightSortCount = 0;
      int temp = 0;
      // left sort count represents the current value to be sorted, left sort count represents the values it is compared to
      for(; leftSortCount < (numProcesses - 1); leftSortCount++)
      {
         rightSortCount = 0;
         for(; rightSortCount < (numProcesses - leftSortCount - 1); rightSortCount++)
         {
            //check if left value is larger than right value
            if (orderDestination[rightSortCount] > orderDestination[rightSortCount + 1])
            {
               //swap the ids of the two processes in the orderDestination array
               temp = orderDestination[rightSortCount];
               orderDestination[rightSortCount] = orderDestination[rightSortCount + 1];
               orderDestination[rightSortCount + 1] = temp;
            }
         }
      }
      // Replace the time values in orderDestination with their respective process id's
      leftSortCount = 0;

      for(; leftSortCount < numProcesses; leftSortCount++)
      {
         rightSortCount = 0;
         // Store the ms value in temp for comparison
         temp = orderDestination[leftSortCount];
         // Compare with pcb array msToCompletion values until they match
         while (temp != pcbDestination[rightSortCount].msToCompletion)
         {
            rightSortCount++;
         }
         // Replace the ms value with the matching pcb id
         orderDestination[leftSortCount] = pcbDestination[rightSortCount].pNum;
      }
   }
   else
   {
      // Sort by first come first serve, nonpreemptive
      for (; sortArrayCount < numProcesses; sortArrayCount++)
      {
         orderDestination[sortArrayCount] = sortArrayCount;
      }
   }
}
