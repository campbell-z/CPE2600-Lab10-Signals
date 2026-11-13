/***********************************************************
* Filename: table_tennis_client.c
* Author: Zach Campbell
* Assignment: CPE2600 Lab 10 - Signals
* Section: 121
* Description: Client for a signal-based table tennis game
***********************************************************/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <time.h>

#define MSG_BALL 1
#define MSG_ACK 2
#define MSG_FAILURE 3
#define SCORE_BASE 100
#define FAILURE_PERCENT 20

static void wait_random_delay(void)
{
     double delay = 1.0 + (double)rand() / RAND_MAX;
   struct timespec request;

   request.tv_sec = (time_t)delay;
   request.tv_nsec = (long)((delay - (double)request.tv_sec) * 1000000000L);

    printf("Waiting %.2f second(s)...\n", delay);

    while (nanosleep(&request, &request) == -1 && errno == EINTR)
    {
    }
}

static int missed_return(void)
{
    return rand() % 100 < FAILURE_PERCENT;
}

static void send_message(pid_t recipient, int signal_number, int message)
{
    union sigval value;

    value.sival_int = message;

    if (sigqueue(recipient, signal_number, value) == -1)
    {
        perror("sigqueue");
        exit(EXIT_FAILURE);
    }
}

static const char *score_name(int score)
{
    switch (score)
    {
        case 0: return "Love";
        case 1: return "15";
        case 2: return "30";
        case 3: return "40";
        case 4: return "Advantage";
        case 5: return "Game";
        default: return "Unknown";
    }
}

static void print_score(int server_score, int client_score)
{
    printf("\n---------------SCORE---------------\n");
    printf("Server: %s\n", score_name(server_score));
    printf("Client: %s\n", score_name(client_score));

    if (server_score == 3 && client_score == 3)
    {
        printf("DEUCE!\n");
    } 
    else if (server_score == 4)
    {
        printf("Server has Advantage!\n");
    }
    else if (client_score == 4)
    {
        printf("Client has Advantage!\n");
    }

    printf("----------------------------------\n\n");
}

int main(void)
{
    pid_t server_pid = 0;
    sigset_t signals;
    siginfo_t info;
    int signal_number;
    int server_score;
    int client_score;
    int score;

    sigemptyset(&signals);
    sigaddset(&signals, SIGRTMIN);
    sigaddset(&signals, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &signals, NULL) == -1)
    {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    srand((unsigned int)time(NULL) ^ (unsigned int)getpid());

    printf("Table Tennis Client PID: %ld\n", (long)getpid());
    printf("Server PID: %ld\n", (long)server_pid);
    printf("Failure probability: %d%%\n", FAILURE_PERCENT);
    printf("Waiting for the server to serve...\n\n");


    while (1)
    {
        do
        {
            signal_number = sigwaitinfo(&signals, &info);
        }

        while (signal_number == -1 && errno == EINTR);

        
        if (signal_number == -1)
        {
            perror("sigwaitinfo");
            return EXIT_FAILURE;
        }

        if (server_pid == 0)
        {
            if (signal_number != SIGRTMIN ||
                info.si_value.sival_int != MSG_BALL)
            {
                continue;
            }

            server_pid = info.si_pid;

            printf("Connected to the server PID: %ld\n\n", (long)server_pid);
        }
        else if (info.si_pid != server_pid)
        {
            continue;
        }

        if (signal_number == SIGUSR2 &&
            info.si_value.sival_int == MSG_FAILURE)
        {
            printf("Server failed to return the ball!\n");
            printf("Acknowledging failed return\n\n");

            send_message(server_pid, SIGRTMIN, MSG_ACK);
            continue;
        }

        if (signal_number != SIGRTMIN)
        {
            continue;
        }

        if (info.si_value.sival_int == MSG_BALL)
        {
            printf("Client receieved the ball!\n");

            wait_random_delay();
        

            if (missed_return())
            {
                printf("Client failed to return the ball!\n");
                printf("Sending failure notification to server\n\n");

                send_message(server_pid, SIGUSR2, MSG_FAILURE);
            }
            else
            {
                printf("Client successfully returned the ball!\n\n");
                send_message(server_pid, SIGRTMIN, MSG_BALL);
            }
        }
        else if (info.si_value.sival_int >= SCORE_BASE)
        {
            score = info.si_value.sival_int - SCORE_BASE;
            server_score = score / 10;
            client_score = score % 10;

            print_score(server_score, client_score);

            if (server_score == 5 || client_score == 5)
            {
                printf("GAME OVER! ");

                if (server_score == 5)
                {
                    printf("Server wins!\n");
                }
                else
                {
                    printf("Client wins!\n");
                }

                break;
            }
        }

    }

    return EXIT_SUCCESS;
}
