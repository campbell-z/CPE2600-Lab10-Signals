/********************************************************************************
* @file signal_segfault.c
* @brief Program that dereferences a null pointer causing a segmentation fault
*
* Modified by: Zach Campbell
* Assignment: CPE2600 Lab 10 - Signals 
* Section: 121
* Brief summary of modifications:
* Added a SIGSEGV signal handler that prints a message when a segmentation fault
* occurs. The handler returns to demonstrate that execution resumes at the 
* instruction causing fault [works]
*
* Compile with:
*   gcc -Wall -Wextra -o signal_segfault signal_segfault.c
* Run with:
*   ./signal_segfault
**********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/**
* @brief Signal handler for SIGSEGV - prints a message
*/
void handle_segfault(int signum)
{
    (void)signum;
    printf("Recieved a segmentation fault\n");
    fflush(stdout);
}
int main (void) {
    // Register for SIGSEGV
    if (signal(SIGSEGV, handle_segfault) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }

    // Declare a null pointer
    int* i = NULL;
    // Dereference the null pointer
    printf("The value of i is: %d\n", *i);
    // Return to exit the program
    return EXIT_SUCCESS;
}