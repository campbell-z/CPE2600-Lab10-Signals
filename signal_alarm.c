/**********************************************************************
* File: signal_alarm.c
* Modified by: Zach Campbell
* Assignment: CPE2600 Lab 10 - Signals 
* Section: 121
* Brief summary of program: Schedules a SIGALRM signal after 5 seconds
* and prints a message when the signal is received [works]
*
*  Compile with:
*   gcc -Wall -Wextra -o signal_alarm signal_alarm.c
* Run with:
*   ./signal_alarm
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
// Handles SIGALRM when scheduled alarm expires
void alarm_handler(int signum)
{
    (void)signum;
    printf("SIGALRM received; The alarm has expired\n");
    fflush(stdout);
}

int main(void)
{
    // Registers custom handler for SIGALRM
    if (signal(SIGALRM, alarm_handler) == SIG_ERR)
    {
        perror("signal");
        return EXIT_FAILURE;
    }

    printf("Alarm scheduled for 5 seconds\n");
    // Schedule SIGALRM to be delivered after 5 seconds
    alarm(5);
    // Wait for alarm signal to be received 
    pause();

    printf("Program finished.\n");

    return EXIT_SUCCESS;
}