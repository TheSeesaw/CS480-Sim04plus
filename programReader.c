#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "helperFunctions.h"

#define STRING_SMALL 32
#define STRING_LARGE 128

/*
 *  Name:        getNextAction
 *  Description: Returns the next 'action' from the line parameter while also
 *               shortening the line to no longer include it.
 */
char * getNextAction(char * line)
{

    char * newStringAction;
    int count;
    int index;

    // Allocate memory for the action string and start its index at zero
    newStringAction = malloc(STRING_SMALL);
    index = 0;

    // Loop through the line and search for the semicolon
    for (count = 0; count < strlen(line); count++)
    {

        // If semicolon is found, end the new string, remove it from the line
        if (line[count] == ';' || line[count] == '.')
        {

            newStringAction[count++] = '\0';
            strcpy(line, substring(line, ++count, strlen(line)));
            return newStringAction;

        }

        // Add the current line character to the new string
        else
        {

            newStringAction[index++] = line[count];

        }

    }

    // If no semicolon is found, return an empty string
    return '\0';

}

/*
 *  Name:        createAction
 *  Description: Creates a new Action structure from the char array parameter
 *               while also checking the stucture for valid values. Returns an
 *               one if an error is found, and a zero if no errors are found.
 */
int createAction(char * string, struct Action * data)
{

    int firstPar;
    int secondPar;
    int cycleTime;
    char commandLetter;
    char * operationString;

    // Set the first character as the command letter
    commandLetter = string[0];

    // Make sure the command letter is acceptable
    if (commandLetter != 'S' && commandLetter != 'A' && commandLetter != 'P' &&
        commandLetter != 'M' && commandLetter != 'I' && commandLetter != 'O')
        {

            return 1;

        }

    // Locate the open and closing parenthesies
    firstPar = findChar(string, '(');
    secondPar = findChar(string, ')');

    // Get the text inbetween the first and second parenthesies
    operationString = substring(string, firstPar + 1, secondPar);

    // Make sure the text is an acceptable value
    if (strcmp(operationString, "access") != 0 &&
        strcmp(operationString, "allocate") != 0 &&
        strcmp(operationString, "end") != 0 &&
        strcmp(operationString, "hard drive") != 0 &&
        strcmp(operationString, "keyboard") != 0 &&
        strcmp(operationString, "printer") != 0 &&
        strcmp(operationString, "monitor") != 0 &&
        strcmp(operationString, "run") != 0 &&
        strcmp(operationString, "start") != 0)
        {

            return 1;

        }

    // If string ends after the parenthesies, then it is missing a cycle time
    if (string[secondPar + 1] == '\0')
    {
        return 1;
    }

    // Get the cycle time from everything after the close parenthesies
    cycleTime = stringToInt(substring(string, secondPar + 1, strlen(string)));

    // Make sure the cycle time is an acceptable value
    if (cycleTime < 0)
    {
        return 1;
    }

    // Copy all the information into the Action structure
    data -> commandLetter = commandLetter;
    strcpy(data -> operationString, operationString);
    data -> cycleTime = cycleTime;

    // No Errors were found
    return 0;

}

/*
 *  Name:        readProgram
 *  Description: Reads in a program using the fileName char array and parses all
 *               of the program actions into a Action struct array while also
 *               counting the number of actions in the program. The int return
 *               value determines if any errors were encountered in the file.
 *               0: Successful, no errors
 *               1: File not found
 *               2: Invalid start to file
 *               3: Invalid end to file
 *               4: Invalid action syntax
 */
int readProgram(char * fileName, struct Action * actions, int * numActions)
{

    int index;
    int actionMessage;
    char line[STRING_LARGE];
    struct Action newAction;

    // Open the file in read mode
    FILE * file = fopen(fileName, "r");

    // If the file does not exist, return 1
    if (!file)
    {
        return 1;
    }

    // Set the index of the Action array to zero
    index = 0;

    // Verify the first line is the start of the program
    fgets(line, sizeof(line), file);

    if (strcmp(line, "Start Program Meta-Data Code:\n") != 0)
    {
        fclose(file);
        return 2;
    }

    // Loop through all remaining lines
    while (fgets(line, sizeof(line), file))
    {

        // If it is the end of the program, close the file and return no errors
        if (strcmp(line, "End Program Meta-Data Code.\n") == 0)
        {
            fclose(file);
            return 0;
        }

        // While actions are left in the line...
        while (strlen(line) > 1)
        {

            // Create a new action from the next available
            actionMessage = createAction(getNextAction(line), &newAction);

            // If there is a non valid action, return error
            if (actionMessage == 1)
            {
                return 4;
            }

            // If no error is found, add it to the actions array and increment
            actions[index++] = newAction;
            *numActions = *numActions + 1;

        }

    }

    // Loop ended and no program end was found, so return error
    fclose(file);
    return 3;
}
