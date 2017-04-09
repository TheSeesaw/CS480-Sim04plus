/*
* File for all functionality related to handling interrupts
*/

// Creates a new interrupt queue
IQueue* create_interrupt_queue()
{
  // Allocate memory for queue
  IQueue* queue = malloc(sizeof(IQueue));
  // Set pointer values
  // Return pointer to new queue
  return queue;
}
// Signal function creates an interrupt and adds it to the queue
void signal()
{
  
}
