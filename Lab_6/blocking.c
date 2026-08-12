#include <stdio.h>
#include <unistd.h>

int main() {
    char buffer[100];

    printf("Blocking I/O\n");
    printf("Enter text: ");

    read(STDIN_FILENO, buffer, sizeof(buffer));

    printf("You entered: %s\n", buffer);

    return 0;
}
