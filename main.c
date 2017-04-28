#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "configReader.h"
#include "programReader.h"
#include "structures.h"
#include "helperFunctions.h"
#include "pcbUtils.h"

#define STRING_SMALL 32
#define STRING_LARGE 128
#define STD_TIMESTRING_SIZE 12
#define STD_TIMESTRING_LINE_SIZE 70
#define BASE_LOGSTRING_SIZE 400

/*
Changes: This file contains the revisions related to the error checking. Before Sim4, the readability and modularity of the program was sorely lacking in quality. We rewrote it to be more readable to the user.

Improvement: This is a strict improvement over the previous version. It makes the code more legible to anyone reading over it and makes updating it much easier.

Justification: This change needed to be made because updates to the program that may occur in the future is much easier when the code is structured properly.

*/






/*
 *  Name: main
 *  Description: Checks for the correct number of arguments, reads the first
 *               argument as a configuration file and the second argument as a
 *               program file. Prints out any errors that are reported by the
 *               file reading functions.
 */
int main(int argc, char *argv[])
{

    int configMessage;
    int numActions;
    int programMessage;
    int pcbCount;
    int numPCBs;

    struct Config data;
    struct Action * actions;
    struct ProcessControlBlock* pcbArray;

    char* logString = NULL;

    // Check if there are less than or more than two arguments passed
    if (argc != 3)
    {

        // Print arguments error
        printf("\n");
        printf("    Error: Config file and program file arguments expected\n");
        printf("    Example: ./sim01 config1.cnf testfile.mdf\n");
        printf("    Program aborted\n");
        printf("\n");
        return 0;

    }

    // Run the readConfig function and save result
    configMessage = readConfig(argv[1], &data);

    // No problems were detected in the config file
    if (configMessage == 0)
    {
        // Allocate memory for the program actions and retrieve them
        actions = malloc(200 * sizeof(struct Action));
        numActions = 0;
        programMessage = readProgram(argv[2], actions, &numActions);

        // No errors detected in program file
        if (programMessage == 0)
        {
            // Initialize logString
            logString = (char*) malloc(BASE_LOGSTRING_SIZE + (numActions * STD_TIMESTRING_LINE_SIZE));

            // Output is saved to a string in any case, but only used if logTo is File or Both
            // For each print statement, check the value of config file logTo, print to monitor if its Monitor or Both
            if (strcmp(data.logTo, "Monitor") == 0 || strcmp(data.logTo, "Both") == 0)
            {
               printf("\n    Program %s has been read successfully\n\n", argv[2]);
            }
            sprintf(logString, "\n    Program %s has been read successfully\n\n", argv[2]);

            // All data has been gathered, system start
            char* displayTime = malloc(STD_TIMESTRING_SIZE);

            // Start the clock!
            getTime(0, displayTime);
            if (strcmp(data.logTo, "Monitor") == 0 || strcmp(data.logTo, "Both") == 0)
            {
               printf("    Begin Simulation\n\nTime:  0000.000000, System start . . .\n");
            }
            sprintf(logString + strlen(logString), "    Begin Simulation\n\nTime:  0000.000000, System start . . .\n");

            // Get the time (lap)
            getTime(1, displayTime);
            if (strcmp(data.logTo, "Monitor") == 0 || strcmp(data.logTo, "Both") == 0)
            {
               printf("Time:  %s, OS: Begin PCB Creation\n", displayTime);
            }
            sprintf(logString + strlen(logString), "Time:  %s, OS: Begin PCB Creation\n", displayTime);

            // Get the number of PCBs
            numPCBs = getNumberOfPCBs(actions, numActions);

            // Allocate PCB array based on number of PCBs
            pcbArray = malloc(numPCBs * sizeof(struct ProcessControlBlock));

            // OS Creates PCB(s)
            initializePCBArray(numPCBs, pcbArray, numActions, actions, data);

            // Finished creating PCBs
            getTime(1, displayTime);
            if (strcmp(data.logTo, "Monitor") == 0 || strcmp(data.logTo, "Both") == 0)
            {
               printf("Time:  %s, OS: All processes initialized in New state\n", displayTime);
            }
            sprintf(logString + strlen(logString), "Time:  %s, OS: All processes initialized in New state\n", displayTime);

            // Set PCBs in Ready state
            pcbCount = 0;
            for (; pcbCount < numPCBs; pcbCount++)
            {
               // Ready = 1
               pcbArray[pcbCount].pState = 1;
            }
            getTime(1, displayTime);
            if (strcmp(data.logTo, "Monitor") == 0 || strcmp(data.logTo, "Both") == 0)
            {
               printf("Time:  %s, OS: All processes set in Ready state\n", displayTime);
            }
            sprintf(logString + strlen(logString), "Time:  %s, OS: All processes set in Ready state\n", displayTime);

            // Start running PCBs
            // Check scheduling code
            if (strcmp(data.schCode, "FCFS-N") == 0 || strcmp(data.schCode, "SJF-N") == 0)
            {
              // Run pcbs using a nonpreemptive routine
              runProgramByNSchedule(pcbArray, numPCBs, data, actions, logString);
            }
            else // Run using a preemptive routine
            {
              //runProgramByPSchedule(pcbArray, numPCBs, data, actions, logString);
            }

            // Done with PCBs, print last system message
            getTime(1,displayTime);
            if (strcmp(data.logTo, "Monitor") == 0 || strcmp(data.logTo, "Both") == 0)
            {
               getTime(1, displayTime);
               printf("Time:  %s, system stop\n", displayTime);
            }
            sprintf(logString + strlen(logString), "Time:  %s, system stop\n", displayTime);

            // Write output to file if logTo is File or Both
            if (strcmp(data.logTo, "Monitor") == 0 || strcmp(data.logTo, "Both") == 0)
            {
               printf("\n    End Simulation\n");
            }
            sprintf(logString + strlen(logString), "\n    End Simulation\n");
            if (strcmp(data.logTo, "File") == 0 || strcmp(data.logTo, "Both") == 0)
            {
               FILE* logFilePointer;
               logFilePointer = fopen(data.logPath, "a");
               fprintf(logFilePointer, "%s", logString);
               fclose(logFilePointer);
            }

        }

        ///////////////////////////////
        //    Program File Errors    //
        ///////////////////////////////

        // File could not be located
        else if (programMessage == 1)
        {

            printf("\n");
            printf("    Error: File %s could not be located\n", argv[2]);
            printf("    Program loading aborted\n");
            printf("\n");

        }

        // Invalid start to file
        else if (programMessage == 2)
        {

            printf("\n");
            printf("    Error: Invalid start to program file\n");
            printf("    Program loading aborted\n");
            printf("\n");

        }

        // Invalid end to file
        else if (programMessage == 3)
        {

            printf("\n");
            printf("    Error: Invalid end to program file\n");
            printf("    Program loading aborted\n");
            printf("\n");

        }

        // Invalid action syntax
        else if (programMessage == 4)
        {

            printf("\n");
            printf("    Error: Invalid action found\n");
            printf("    Program loading aborted\n");
            printf("\n");

        }

    }

    ////////////////////////////////
    //     Config File Errors     //
    ////////////////////////////////

    // File could not be located
    else if (configMessage == 1)
    {

        printf("\n");
        printf("    Error: File %s could not be located\n", argv[1]);
        printf("    Program aborted\n");
        printf("\n");

    }

    // Invalid version number
    else if (configMessage == 2)
    {

        printf("\n");
        printf("    Error: Invalid version/phase number detected\n");
        printf("    Value should be between or equal to 0 through 10\n");
        printf("    Program aborted\n");
        printf("\n");

    }

    // Invalid CPU scheduling code
    else if (configMessage == 3)
    {

        printf("\n");
        printf("    Error: Invalid CPU scheduling code detected\n");
        printf("    Value should be one of the following:\n");
        printf("        NONE,   FCFS-N\n");
        printf("        SJF-N,  SRTF-P\n");
        printf("        FCFS-P, RR-P\n");
        printf("    Program aborted\n");
        printf("\n");

    }

    // Invalid quantum time
    else if (configMessage == 4)
    {

        printf("\n");
        printf("    Error: Invalid quantum time detected\n");
        printf("    Value should be between or equal to 0 through 100\n");
        printf("    Program aborted\n");
        printf("\n");
    }

    // Invalid Memory amount
    else if (configMessage == 5)
    {

        printf("\n");
        printf("    Error: Invalid memory amount detected");
        printf("    Value should be between or equal to 0 through 1,048,576\n");
        printf("    Program aborted\n");
        printf("\n");

    }

    // Invalid processor cycle time
    else if (configMessage == 6)
    {

        printf("\n");
        printf("    Error: Invalid processor cycle time detected\n");
        printf("    Value should be between or equal to 1 through 1,000\n");
        printf("    Program aborted\n");
        printf("\n");
    }

    // Invalid I/O cycle time
    else if (configMessage == 7)
    {

        printf("\n");
        printf("    Error: Invalid I/O cycle time detected\n");
        printf("    Value should be between or equal to 1 through 10,000");
        printf("    Program aborted\n");
        printf("\n");

    }

    // Invalid Log To
    else if (configMessage == 8)
    {

        printf("\n");
        printf("    Error: Invalid log location\n");
        printf("    Log location should be either Monitor, File, or Both\n");
        printf("    Program aborted\n");

    }

    // End program
    return 0;
}
