CC=gcc
CFLAGS= -Wall -Wextra -Werror

TARGETS = signal_handler signal_alarm signal_segfault signal_sigaction recv_signal send_signal

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
clean:
	rm -f $(TARGETS)
