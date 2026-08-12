#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("Socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 5);

    printf("Concurrent TCP Server is waiting...\n");

    while (1) {
        new_socket = accept(server_fd,
                            (struct sockaddr *)&address,
                            (socklen_t *)&addrlen);

        if (fork() == 0) {
            close(server_fd);

            read(new_socket, buffer, sizeof(buffer));

            printf("Client says: %s\n", buffer);

            char reply[] = "Hello from Child Process";

            send(new_socket, reply, strlen(reply), 0);

            close(new_socket);

            exit(0);
        }

        close(new_socket);
    }

    return 0;
}