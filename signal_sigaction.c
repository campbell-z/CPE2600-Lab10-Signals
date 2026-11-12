/**********************************************************************
* File: signal_sigaction.c
* Modified by: Zach Campbell
* Assignment: CPE2600 Lab 10 - Signals 
* Section: 121
* Brief summary of program: Uses sigaction() to recieve SIGUSR1 and 
* display the PID of the process that sent the signal [works]
*
* Command to send to SIGUSR1
* kill -SIGUSR1 <PID>
* Compile with:
*   gcc -Wall -Wextra -o signal_sigaction signal_sigaction.c
* Run with:
*   ./signal_sigaction
***********************************************************************/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
// Handles SIGUSR1 and retrives the sender's PID from siginfo_t
void usr1_handler(int signum, siginfo_t *info, void *context)
{
    (void)signum;
    (void)context;

    printf("SIGUSR1 recieved from PID %ld\n", (long)info->si_pid);
    fflush(stdout);
}

int main(void)
{
    // Configure handler to recieved additional signal information
    struct sigaction sa = {0};

    sa.sa_sigaction = usr1_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    printf("Process PID: %ld\n", (long)getpid());
    printf("Waiting for SIGUSR1...\n");
    // Wait for incoming SIGUSR1 signals
    while (1)
    {
        pause();
    }

    return EXIT_SUCCESS;
}