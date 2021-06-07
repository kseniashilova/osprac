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
char need_to_send = 0;

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
    if (!need_to_send && bit == 1) {
        need_to_send = 1;
        return;
    }

    printf("unexpected signal\n");
}
 
int main(void) {
    (void) signal(SIGUSR1, my_handler);
    (void) signal(SIGUSR2, my_handler);

    printf("Transmitter pid: %d\n", getpid());
    printf("Please input receiver pid: ");
    pid_t receiver_pid;
    scanf("%d", &receiver_pid);

    int message;
    printf("input number to transmit: ");
    scanf("%d", &message);

    send(receiver_pid, 0);
    printf("waiting for receiver\n");
    while(!connected);

    printf("transmitting message\n");
    for (int i = 0; i < 32; i++) {
        while (!need_to_send);
        need_to_send = 0;
        send(receiver_pid, (message >> i) & 1);
    }

    printf("transmission finished\n");

    return 0;
}