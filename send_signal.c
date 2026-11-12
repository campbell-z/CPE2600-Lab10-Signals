/**********************************************************************
* File: send_signal.c
* Modified by: Zach Campbell
* Assignment: CPE2600 Lab 10 - Signals 
* Section: 121
* Brief summary of program: Generates a random integer and transmits
* it to another process using sigqueue() and SIGUSR1 [works]
*
*  Compile with:
*   gcc -Wall -Wextra -o send_signal send_signal.c
* Run with:
*   ./send_signal
***********************************************************************/
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // Requires the receiver PID as only command-line argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <receiver_pid>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr;
    errno = 0;
    // Convert receiver PID from string to long integer
    long pid_value = strtol(argv[1], &endptr, 10);
    // Validate PID before continuing
    if (errno != 0 || endptr == argv[1] ||
        *endptr != '\0' || pid_value <= 0 || 
        pid_value > INT_MAX)
    {
        fprintf(stderr, "Invalid receiver PID\n");
        return EXIT_FAILURE;
    }

    pid_t receiver_pid = (pid_t)pid_value;
    // Ensure PID was not changed during conversion to pid_t
    if ((long)receiver_pid != pid_value)
    {
        fprintf(stderr, "Invalid receiver PID\n");
        return EXIT_FAILURE;
    }
    // Seed the random number generator using current time
    srand((unsigned int)time(NULL));

    int random_value = rand();

    printf("Sending integer %d\n", random_value);
    // sigval stores additional data transmitted with SIGUSR1
    // Receiver retrives this integer through info->si_value.sival_int
    union sigval value;
    value.sival_int = random_value;
    // Send SIGUSR1 and integer payload to receiver process
    if (sigqueue(receiver_pid, SIGUSR1, value) == -1)
    {
        perror("sigqueue");
        return EXIT_FAILURE;
    }

    printf("Signal sent successfully\n");

    return EXIT_SUCCESS;
}
