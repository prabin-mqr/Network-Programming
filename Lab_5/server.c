#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

void *client_handler(void *socket_desc) {
    int sock = *(int *)socket_desc;
    char buffer[1024];
    char reply[] = "Hello from Multithreaded Server";

    int read_size = read(sock, buffer, sizeof(buffer));

    if (read_size > 0) {
        buffer[read_size] = '\0';
        printf("Client: %s\n", buffer);

        send(sock, reply, strlen(reply), 0);
    }

    close(sock);
    free(socket_desc);

    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);

    printf("Multithreaded Server is waiting...\n");

    while (1) {
        new_socket = accept(server_fd,
                            (struct sockaddr *)&client_addr,
                            &client_len);

        int *new_sock = malloc(sizeof(int));
        *new_sock = new_socket;

        pthread_t thread_id;

        pthread_create(&thread_id,
                       NULL,
                       client_handler,
                       (void *)new_sock);

        pthread_detach(thread_id);
    }

    close(server_fd);

    return 0;
}
