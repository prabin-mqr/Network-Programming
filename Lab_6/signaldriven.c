#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("\nSignal received: %d\n", sig);
}

int main() {

    signal(SIGINT, handler);

    printf("Press Ctrl+C to generate SIGINT.\n");

    while (1)
        pause();

    return 0;
}
