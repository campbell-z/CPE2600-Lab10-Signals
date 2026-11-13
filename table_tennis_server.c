/*********************************************************
* Filename: table_tennis_server.c
* Author: Zach Campbell
* Assignment: CPE2600 Lab 10 - Signals
* Section: 121
* Decription: Server for a signal-based table tennis game
*********************************************************/

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

static int server_score = 0;
static int client_score = 0;

static pid_t get_pid(const char *text)
{
    char *endptr;
    long value;
    
    errno = 0;
    value = strtol(text, &endptr, 10);

    if (errno != 0 || *text == '\0' || *endptr != '\0' ||
        value <= 0 || value > INT_MAX)
    {
        return -1;
    }

    return (pid_t)value;
}

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

static void print_score(void)
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

static int award_point(int server_won)
{
    int *winner = server_won ? &server_score : &client_score;
    int *loser = server_won ? &client_score : &server_score;

    if (*winner == 3 && *loser < 3)
    {
        *winner = 5;
        return 1;
    }

    if (*winner == 4)
    {
        *winner = 5;
        return 1;
    }

    if (*winner == 3 && *loser == 3)
    {
        *winner = 4;
        return 0;
    }

    if (*loser == 4)
    {
        *loser = 3;
        return 0;
    }

    (*winner)++;
    return 0;
}

static void send_score(pid_t client_pid)
{
    int message = SCORE_BASE + server_score * 10 + client_score;

    send_message(client_pid, SIGRTMIN, message);
}

static void wait_for_ack(pid_t client_pid, const sigset_t *signals)
{
    siginfo_t info;
    int signal_number;

    do 
    {
        signal_number = sigwaitinfo(signals, &info);
    } 
    while (signal_number == -1 && errno == EINTR);
    {
        if (signal_number == -1)
        {
            perror("sigwaitinfo");
            exit(EXIT_FAILURE);
        }

        if (signal_number != SIGRTMIN ||
            info.si_pid != client_pid ||
            info.si_value.sival_int != MSG_ACK)
        {
            fprintf(stderr, "Invalid failure acknowledgement\n");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[])
{
    pid_t client_pid;
    sigset_t signals;
    siginfo_t info;
    int signal_number;
    int game_over = 0;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <client_PID>\n", argv[0]);
        return EXIT_FAILURE;
    }

    client_pid = get_pid(argv[1]);

    if (client_pid == -1)
    {
        fprintf(stderr, "Invalid client PID\n");
        return EXIT_FAILURE;
    }

    sigemptyset(&signals);
    sigaddset(&signals, SIGRTMIN);
    sigaddset(&signals, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &signals, NULL) == -1)
    {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    srand((unsigned int)time(NULL) ^ (unsigned int)getpid());

    printf("Table Tennis Server PID: %ld\n", (long)getpid());
    printf("Client PID: %ld\n", (long)client_pid);
    printf("Failure probability: %d%%\n", FAILURE_PERCENT);
    printf("Starting game. Server serves first!\n\n");

    send_message(client_pid, SIGRTMIN, MSG_BALL);

    while (!game_over)
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

            if (info.si_pid != client_pid)
            {
                continue;
            }

            if (signal_number == SIGUSR2 &&
                info.si_value.sival_int == MSG_FAILURE)
            {
                printf("Client failed to return the ball!\n");
                printf("Point awarded to the server\n");

                game_over = award_point(1);
            }
            else if (signal_number == SIGRTMIN &&
                    info.si_value.sival_int == MSG_BALL)
            {
                    printf("Server received the returned ball\n");

                    wait_random_delay();

                    if (missed_return())
                    {
                        printf("Server failed to return the ball!\n");
                        printf("Point awarded to the client\n");

                        send_message(client_pid, SIGUSR2, MSG_FAILURE);
                        wait_for_ack(client_pid, &signals);

                        game_over = award_point(0);
                    }
                    else
                    {
                        printf("Server successfully returned the ball!\n");

                        send_message(client_pid, SIGRTMIN, MSG_BALL);
                        continue;
                    }
            }
            else
            {
                continue;
            }

            print_score();
            send_score(client_pid);

            if (game_over)
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
            }
            else
            {
                printf("Server serves again!\n\n");
                send_message(client_pid, SIGRTMIN, MSG_BALL);
            }
        }

    return EXIT_SUCCESS;
}
