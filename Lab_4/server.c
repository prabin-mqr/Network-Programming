#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8080
#define MAX_CLIENTS 5

int main() {
    int server_fd, new_socket, client_socket[MAX_CLIENTS];
    struct sockaddr_in address;
    fd_set readfds;
    int max_sd, activity, valread;
    int addrlen = sizeof(address);
    char buffer[1024];

    for (int i = 0; i < MAX_CLIENTS; i++)
        client_socket[i] = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Select Server is waiting...\n");

    while (1) {

        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_socket[i];

            if (sd > 0)
                FD_SET(sd, &readfds);

            if (sd > max_sd)
                max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &readfds)) {

            new_socket = accept(server_fd,
                                (struct sockaddr *)&address,
                                (socklen_t *)&addrlen);

            printf("New Client Connected\n");

            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {

            int sd = client_socket[i];

            if (FD_ISSET(sd, &readfds)) {

                valread = read(sd, buffer, sizeof(buffer));

                if (valread == 0) {

                    close(sd);
                    client_socket[i] = 0;

                } else {

                    buffer[valread] = '\0';

                    printf("Client: %s\n", buffer);

                    char reply[] = "Message received";

                    send(sd, reply, strlen(reply), 0);
                }
            }
        }
    }

    return 0;
}
