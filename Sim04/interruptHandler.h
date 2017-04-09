/*
* Header File for all functionality related to handling interrupts
*/

#include "structures.h"

// Creates a new interrupt queue
IQueue* create_interrupt_queue();
// Signal function creates an interrupt and adds it to the queue
void signal();
