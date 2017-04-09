#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include "structures.h"

#define STRING_SMALL 32
#define STRING_LARGE 128
#define A_MILLION 1000000
#define STD_TIMESTRING_SIZE 12

/*
 *  Name:        findChar
 *  Description: Loops through a char array searching for a specific target
 *               char. Returns the index of the char if it is found, and -1 if
 *               it is not found.
 */
int findChar(char * string, char target)
{

    // Counter for loop
    int count;

    // Loop through every char in the string
    for (count = 0; count < strlen(string); count++)
    {

        // If the char is equal to our target, return count
        if (string[count] == target)
        {
            return count;
        }

    }

    // If the char is not found, return -1
    return -1;

}

/*
 *  Name: stringToInt
 *  Description: Converts a char array to an integer. Example: {'7', '1', '4'}
 *               would be converted into an integer 714.
 */
int stringToInt(char * string)
{

    // Variables for loop, digit place, and final answer
    int count;
    int digit = 1;
    int answer = 0;

    // Loop through array
    for (count = strlen(string) - 1; count >= 0; count--)
    {

        // Convert char int, multiple it by the digit place, add to total
        answer += (string[count] - '0') * digit;

        // Increment digit to the next place
        digit *= 10;
    }

    // Return the final total
    return answer;

}

/*
 *  Name:        substring
 *  Description: Returns a subsection of a character array derived from the
 *               start and end integer parameters passed to it.
 */
char * substring(char * string, int start, int end)
{

    // Initialize loop counter, char array index, and char array
    int count;
    int index = 0;
    char * newString = malloc(STRING_LARGE);

    // Loop through input array, assign values to it
    for (count = start; count < end; count++)
    {
        newString[index] = string[count];
        index++;
    }

    // Mark the end of the string in the array
    newString[index] = '\0';

    // Return the final new array
    return newString;

}

// This function operates a timer that saves a string representation of the timer value in timeString
void getTime(int timerOperation, char* timeString)
{
   // s_time is the time in seconds and us_time is the time in microseconds
   static struct timeval sysTime;
   static int isValidTime = 0;

   static int startSeconds = 0;
   static int startUSeconds = 0;
   static int currentSeconds = 0;
   static int currentUSeconds = 0;




   switch(timerOperation)
   {
      // 0 case = reset
      case 0 :
         gettimeofday(&sysTime, NULL);
         startSeconds = sysTime.tv_sec;
         startUSeconds = sysTime.tv_usec;
         isValidTime = 1;
         break;
      // 1 case = lap
      case 1 :
         // Check if timer has been started
         if (isValidTime == 1)
         {
            gettimeofday(&sysTime, NULL);
            currentSeconds = sysTime.tv_sec;
            currentUSeconds = sysTime.tv_usec;
            currentSeconds -= startSeconds;
            currentUSeconds -= startUSeconds;
            // Check for edge case
            if (currentUSeconds < 0)
            {
               currentSeconds -= 1;
               currentUSeconds += A_MILLION;
            }
            // Else, save information into time_string
            snprintf(timeString, STD_TIMESTRING_SIZE, "%04d.%06d", currentSeconds, currentUSeconds);
         }
         else
         {
            // Timer has not been started
            snprintf(timeString, STD_TIMESTRING_SIZE, "ERROR: 0");

         }
         break;
      // 2 case = stop
      case 2 :
         // Check if timer is started
         if (isValidTime == 1)
         {
            // Reset variables
            startSeconds = 0;
            startUSeconds = 0;
            currentSeconds = 0;
            currentUSeconds = 0;

            isValidTime = 0;
         }
         break;
   }
}

// Spin and do nothing for a certain amount of time
void busyCycle(int targetTime)
{
   struct timeval startCycleTime;
   struct timeval busyCycleTime;
   int targetTimeInMS = targetTime;
   int currentTimeInMS = 0;

   int cycleStartS;
   int cycleStartUS;
   int cycleCurrentS;
   int cycleCurrentUS;

   //char* cycleTargetString;

   gettimeofday(&startCycleTime, NULL);
   cycleStartS = startCycleTime.tv_sec;
   cycleStartUS = startCycleTime.tv_usec;

   //Check for edge case
   if (cycleStartUS < 0)
   {
      cycleStartS -= 1;
      cycleStartUS += A_MILLION;
   }

   // Cycle while waiting for clock to match target time
   while (currentTimeInMS <= targetTimeInMS)
   {
      gettimeofday(&busyCycleTime, NULL);
      cycleCurrentS = busyCycleTime.tv_sec;
      cycleCurrentUS = busyCycleTime.tv_usec;
      cycleCurrentS = cycleCurrentS - cycleStartS;
      cycleCurrentUS = cycleCurrentUS - cycleStartUS;
      // Check for edge case
      if (cycleCurrentUS < 0)
      {
         cycleCurrentS -= 1;
         cycleCurrentUS += A_MILLION;
      }
      currentTimeInMS = (cycleCurrentS * 1000) + (cycleCurrentUS / 1000);
   }
   // Clock has reached appropriate time
}

char* getOperationString(struct Action theAction)
{
  char* theString;
  switch(theAction.commandLetter)
  {
     case 'M' :
        // Memory
        theString = "memory";
        break;
     case 'I' :
        // Input
        theString = "input";
        break;
     case 'O' :
        // Output
        theString = "output";
        break;
     case 'P' :
        // Run Process
        theString = "process";
        break;
     case 'A' :
        // Run/End application
        theString = "application";
        break;
     default:
        printf("Glitch in the matrix.");
  }
  return theString;
}
