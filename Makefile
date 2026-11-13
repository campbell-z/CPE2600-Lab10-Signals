CC=gcc
CFLAGS= -Wall -Wextra -Werror

TARGETS = signal_handler signal_alarm signal_segfault signal_sigaction recv_signal send_signal table_tennis_server table_tennis_client

all: $(TARGETS)

signal_handler: signal_handler.c
	$(CC) $(CFLAGS) signal_handler.c -o signal_handler

signal_alarm: signal_alarm.c
	$(CC) $(CFLAGS) signal_alarm.c -o signal_alarm

signal_segfault: signal_segfault.c
	$(CC) $(CFLAGS) signal_segfault.c -o signal_segfault

signal_sigaction: signal_sigaction.c
	$(CC) $(CFLAGS) signal_sigaction.c -o signal_sigaction

recv_signal: recv_signal.c
	$(CC) $(CFLAGS) recv_signal.c -o recv_signal

send_signal: send_signal.c 
	$(CC) $(CFLAGS) send_signal.c -o send_signal

table_tennis_server: table_tennis_server.c
	$(CC) $(CFLAGS) table_tennis_server.c -o table_tennis_server

table_tennis_client: table_tennis_client.c
	$(CC) $(CFLAGS) table_tennis_client.c -o table_tennis_client
clean:
	rm -f $(TARGETS)
