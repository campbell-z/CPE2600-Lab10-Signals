/**********************************************************************
* File: recv_signal.c
* Modified by: Zach Campbell
* Assignment: CPE2600 Lab 10 - Signals 
* Section: 121
* Brief summary of program: Recieves SIGUSR1 using sigaction() and 
* prints the integer value transmitted by sigqueue() [works]
*
*  Compile with:
*   gcc -Wall -Wextra -o recv_signal recv_signal.c
* Run with:
*   ./recv_signal
***********************************************************************/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Handles SIGUSR1 and retrives interger payload from sigqueue()
void usr1_handler(int signum, siginfo_t *info, void *context)
{
    (void)signum;
    (void)context;

    printf("Recived integer: %d\n", info->si_value.sival_int);
    fflush(stdout);
}

int main(void)
{
    // Configure the signal handler and enable additional signal information
    struct sigaction sa = {0};

    sa.sa_sigaction = usr1_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction");
        return EXIT_FAILURE;
    }
    // Display the PID so another process can send this process a signal
    printf("Process PID: %ld\n", (long)getpid());
    printf("Waiting for SIGUSR1...\n");
    fflush(stdout);
    // Wait for incoming signals
    while (1)
    {
        pause();
    }

    return EXIT_SUCCESS;

}