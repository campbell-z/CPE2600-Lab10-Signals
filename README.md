# CPE2600 Lab 10 -Signals
**Name:** Zach Campbell
**Course:** CPE2600
**Section:** 121
**Assignment:** Lab 10 - Signals

## Description

This lab explores Linux signal handling and demonstrates how C programs
can generate, recieve, and respond to signals using POSIX system calls
and signal handling functions,

The assignment consists of four programs:

### 1. signal_handler.c

Demonstrates basic signal handling using signal() and SIGINT:

- Registers a custom handler for SIGINT
- Displays a message when the signal is recieved
- Returns from the signal handler without terminating the program
- Demonstrates how a process continues executing after recieving SIGINT

### 2. signal_alarm.c

Demonstrates the use of alarm() and SIGALRM

- Registers custom handler for SIGALRM
- Schedules an alarm to expire after 5 seconds
- Returns from the signal handler without terminanting the program
- Terminates after the alarm handler finishes executing

### 3. signal_segfault.c

Demonstrates segmentation fault handling using SIGSEGV

- Registers a custome handler for SIGSEGV
- Intentionally dereferences a null pointer to generate a segmentation fault
- Displays a message when the signal handler executes
- Demonstrates the behavior of returning a segmentation fault handler

### 4. signal_sigaction.c

Demonstrates advanced signal handling using sigaction() and SIGUSR1

- Registers a custom signal handler using sigaction()
- Uses the SA_SIGINFO flag to access additional signal information
- Retrives and displays the PID of the process that sent the signal
- Allows another process to send SIGUSR1 using the kill command

### 5. recv_signal.c

Recieves signals containing additional data using sigaction() and SIGUSR1

- Registers a custom signal handler using sigaction()
- Uses the SA_SIGINFO flag to access additional signal information
- Retrives and displays an integer transmitted through sigqueue()
- Waits for incoming signals from another process

### 6. send_signal.c

Transmits signals containing additional data using sigqueue()

- Accepts the receiver's PID as a command line argument
- Generates a random integer using rand()
- Stores the integer in a union sigval structure
- Sends SIGUSR1 and the integer payload to the receiving process using sigqueue()

## Compilation

Compile all programs using the provided Makefile:

```bash
make
```

To remove compiled executables and object files:

```bash
make clean
```

## Execution

Run each program using the following commands:

### Demonstrate basic SIGINT handling
```bash
./signal_handler
```
Press Ctrl+C to send SIGINT and observe that the program continues executing

### Demonstrate SIGALRM
```bash
./signal_alarm
```

### Demonstrate SIGSEGV handling
```bash
./signal_segfault
```
The program intentionally dereferences a null pointer to generate SIGSEGV

Note: Returning from the handler does not correct invalid memory access. 
On Linux, the segmentation fault will repeatadly occure. Terminate the 
program using Ctrl+C.

### Demonstrate SIGUSR1 using sigaction()
Start the receiving process:
```bash
./signal_sigaction
```
From another terminal, send SIGUSR1 using the reciever's PID:
```bash
kill -SIGUSR1 <PID>
```
The receiving process displays the PID of the program that sent the signal

### Receive an integer using sigqueue()
Start the receiving process:
```bash
./recv_signal
```
Record the PID displayed in the program

### Send an integer using sigqueue()
From another terminal, execute:
```bash
./send_signal <PID>
```
The sending process generates a random integer and transmits it to the receiver
using sigqueue()

The receiving process displays the transmitted integer

## Requirements

- Linux operating system
- GCC compiler
- GNU Make
- POSIX-compliant signal handling functionality
- Two terminal sessions for demonstrating interprocess signal communication