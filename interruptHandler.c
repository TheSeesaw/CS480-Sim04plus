#include "interruptHandler.h"


* File for all functionality related to handling interrupts


//****************************************************************

//Changes: This file adds an inerrupt handler to the program. This was listed among the revisions we aimed to make to our previous version of Sim4.

//Improvement: This is an improvement over the previous version since it is now capable of using interrupts with the threads. This allows for preemptive scheduling, which is required by the project guidelines.

//Justification: We justified making this change by examining the overall requirements of the project and the most elegant solutions available. An interrupt handler was by far the most clean and concise solution that we could realize realistically.

//****************************************************************



// Creates a new interrupt queue
struct IQueue* createInterruptQueue()
{
  // Allocate memory for queue
  IQueue* queue = malloc(sizeof(IQueue));
  // Set pointer values
  // Return pointer to new queue
  return queue;
}
// Signal function creates an interrupt and adds it to the queue
void signalInterrupt(struct ThreadControlBlock* currentTCB)
{

}

// For now, assume that ThreadControlBlock structures are added to the queue
void addToIQueue(void* currentTCB)
{

}
