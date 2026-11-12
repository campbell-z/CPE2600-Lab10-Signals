/*******************************************************************************
* @file signal_handler.c
* @brief Program that sets a signal handler for SIGINT. Handler prints a message
* and returns it without terminating the program.
*/
/**
* Modified by: Zach Campbell
* Assignment: CPE2600 Lab 10 - Signals 
* Section: 121
* Brief summary of modifications:
* Removed exit() from the signal handler so the program continues running after
* receiving SIGINT [works]
*
*  Compile with:
*   gcc -Wall -Wextra -o signal_handler signal_handler.c
* Run with:
*   ./signal_handler
*******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/**
* @brief Signal handler for SIGINT - prints a message and returns
*/
void handle_signal(int signum)
{
    (void)signum;
    printf("Recieved a signal\n");
    fflush(stdout);
}

int main (void) {
    // Register for the signal
    if (signal(SIGINT, handle_signal) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }
    // Wait until a signal is recieved
    while (1)
    {
        printf("Sleeping\n");
        fflush(stdout);
        sleep(1);
    }

    return EXIT_SUCCESS;
}
