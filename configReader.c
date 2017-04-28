#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "helperFunctions.h"

#define STRING_SMALL 32
#define STRING_LARGE 128

 /*
  *  Name:        readConfig
  *  Description: Looks for a file with the fileName parameter and then scans it
  *               looking for data to assign to the Config structure. Returns
  *               specific ints for types of errors encountered.
  *               0: No errors
  *               1: File not found
  *               2: Invalid version number
  *               3: Invalid CPU Scheduling Code
  *               4: Invalid Quantum Time
  *               5: Invalid Memory Available
  *               6: Invalid Processor Cycle Time
  *               7: Invalid I/O Cycle Time
  *               8: Invalid Log To
  */
int readConfig(char * fileName, struct Config * data)
{

    char line[STRING_LARGE];
    int lineNum;
    int end;
    int value;

    // Open the file in read mode
    FILE * file = fopen(fileName, "r");

    // If the file does not exist, return 1
    if (!file)
    {
        return 1;
    }

    // Set the line number to the first line
    lineNum = 1;

    // Loop through each line of the file
    while (fgets(line, sizeof(line), file))
    {



        /*
         *   Version / Phase
         */
        if (lineNum == 2)
        {

            // Make sure the version is between 0 and 9
            value = line[15] - '0';

            if (value >= 0 && value <= 9)
            {

                // Make sure version is not greater than 10
                if (line[16] == '.' || line[16] == '\n')
                {
                    data -> version = value;
                }
                else if (value == 1 && line[16] == '0' && line[17] == '\n')
                {
                    data -> version = 10;
                }
                else
                {
                    fclose(file);
                    return 2;
                }

            }
            // If not in range, close file
            else
            {
                fclose(file);
                return 2;
            }

        }



        /*
         *   File Path
         */
        else if (lineNum == 3)
        {
            strcpy(data -> filePath, substring(line, 11, findChar(line, '\n')));
        }




        /*
         *   CPU Scheduling Code
         */
        else if (lineNum == 4)
        {

            // Check if a valid option is present
            if (strcmp(line, "CPU Scheduling Code: NONE\n") == 0)
            {
                strcpy(data -> schCode, "FCFS-N");
            }
            else if (strcmp(line, "CPU Scheduling Code: FCFS-N\n") == 0)
            {
                strcpy(data -> schCode, "FCFS-N");
            }
            else if (strcmp(line, "CPU Scheduling Code: SJF-N\n") == 0)
            {
                strcpy(data -> schCode, "SJF-N");
            }
            else if (strcmp(line, "CPU Scheduling Code: SRTF-P\n") == 0)
            {
                strcpy(data -> schCode, "SRTF-P");
            }
            else if (strcmp(line, "CPU Scheduling Code: FCFS-P\n") == 0)
            {
                strcpy(data -> schCode, "FCFS-P");
            }
            else if (strcmp(line, "CPU Scheduling Code: RR-P\n") == 0)
            {
                strcpy(data -> schCode, "RR-P");
            }

            // If not valid option is present, close file
            else
            {
                fclose(file);
                return 3;
            }

        }



        /*
         *   Quantum Time
         */
        else if (lineNum == 5)
        {

            // Find the end of the line, and the integer value in the line
            end = findChar(line, '\n');
            value = stringToInt(substring(line, 23, end));

            // Check if value is in acceptable range
            if (value < 0 || value > 100)
            {
                fclose(file);
                return 4;
            }

            // Assign value to data
            data -> quanTime = value;

        }



        /*
         *   Memory Available
         */
        else if (lineNum == 6)
        {

            // Find the end of the line, and the integer value in the line
            end = findChar(line, '\n');
            value = stringToInt(substring(line, 23, end));

            // Check if value is in acceptable range
            if (value < 0 || value > 1048576)
            {
                fclose(file);
                return 5;
            }

            // Assign value to data
            data -> memAvail = value;

        }



        /*
         *   Processor Cycle Time
         */
        else if (lineNum == 7)
        {

            // Find the end of the line, and integer value in the line
            end = findChar(line, '\n');
            value = stringToInt(substring(line, 29, end));

            // Check if the value is in acceptable range
            if (value < 0 || value > 1000)
            {
                fclose(file);
                return 6;
            }

            // Assign the value to data
            data -> pTime = value;

        }



        /*
         *   I/O Cycle Time
         */
        else if (lineNum == 8)
        {

            // Find the end of the line, and the integer value in the line
            end = findChar(line, '\n');
            value = stringToInt(substring(line, 23, end));

            // Check if the value is in acceptable range
            if (value < 0 || value > 10000)
            {
                fclose(file);
                return 7;
            }

            // Assign the value to data
            data -> ioTime = value;

        }



        /*
         *   Log To
         */
        else if (lineNum == 9)
        {

            // Check if the value is Monitor, File, or Both
            if (strcmp(line, "Log To: Monitor\n") == 0)
            {
                strcpy(data -> logTo, "Monitor");
            }
            else if (strcmp(line, "Log To: File\n") == 0)
            {
                strcpy(data -> logTo, "File");
            }
            else if (strcmp(line, "Log To: Both\n") == 0)
            {
                strcpy(data -> logTo, "Both");
            }

            // If it is none of those close file
            else
            {
                fclose(file);
                return 8;
            }

        }



        /*
         *   Log File Path
         */
        else if (lineNum == 10)
        {
            strcpy(data -> logPath, substring(line, 15, findChar(line, '\n')));
        }



        // Increment the line number
        lineNum++;

    }

    // If all tasks finish successfully, close file, return 0
    fclose(file);
    return 0;

}
