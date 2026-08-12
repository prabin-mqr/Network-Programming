#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#define PORT 8080

int server_fd;

void signal_handler(int sig) {
    printf("\nSIGINT received. Closing server...\n");
    close(server_fd);
    exit(0);
}

int main() {
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_socket;
    char buffer[1024];

    signal(SIGINT, signal_handler);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);

    printf("Signal Handling Server is waiting...\n");

    client_socket = accept(server_fd,
                           (struct sockaddr *)&client_addr,
                           &addr_len);

    read(client_socket, buffer, sizeof(buffer));

    printf("Client: %s\n", buffer);

    char reply[] = "Message received successfully";

    send(client_socket, reply, strlen(reply), 0);

    close(client_socket);

    pause();

    return 0;
}
