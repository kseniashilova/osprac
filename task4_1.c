#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 

void send(pid_t pid, char bit) {
    int signal;
    if (bit == 0) {
        signal = SIGUSR1;
    } else {
        signal = SIGUSR2;
    }

    if (kill(pid, signal) < 0) {
        printf("cannot send signal\n");
        exit(-1);
    }
}

char connected = 0;
char lastBit = 1;
char received = 0;

void my_handler(int nsig) {
    char bit;
    if (nsig == SIGUSR1) {
        bit = 0;
    } else if (nsig == SIGUSR2) {
        bit = 1;
    }

    if (!connected && bit == 0) {
        connected = 1;
        return;
    }
    if (connected && !received) {
        lastBit = bit;
        received = 1;
        return;
    }

    printf("unexpected signal\n");
}

int main(void) {
    (void) signal(SIGUSR1, my_handler);
    (void) signal(SIGUSR2, my_handler);

    printf("receiver PID = %d\n", getpid());
    printf("input transmitter PID: ");
    pid_t transmitter_pid;
    scanf("%d", &transmitter_pid);

    send(transmitter_pid, 0);
    printf("waiting for transmitter\n");
    while(!connected);

    printf("receiving message\n");
    int message = 0;
    for (int i = 0; i < 32; i++) {
        received = 0;
        send(transmitter_pid, 1);
        while (!received);
        message |= (lastBit << i);
    }

    printf("message: %d\n", message);
    return 0;
}