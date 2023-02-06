#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include "helper.h"
#include "high_score.h"
#include "game_dispatcher.h"

#define MAX_CONNECTIONS (10)

int main (int argc, char *argv[]) {
    uses_assumed_sizes();
    init_msgs();

    int server_fd, client_fd, err;
    struct sockaddr_in server, client;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Could not create socket\n");
    }

    server.sin_family = AF_INET;
    server.sin_port = 0; // bind will choose random port
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt_val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);

    err = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
    if (err < 0) {
        perror("Could not bind socket\n");
    }

    err = listen(server_fd, MAX_CONNECTIONS);
    if (err < 0) {
        perror("Could not listen on socket\n");
    }

    socklen_t len = sizeof(server);
    if (getsockname(server_fd, (struct sockaddr *)&server, &len) == -1) {
        perror("Couldn't get binded socket port");
    }
    else {
        printf("Server is listening on port %d\n", ntohs(server.sin_port));
    }

    while (1) {
        socklen_t client_len = sizeof(client);
        client_fd = accept(server_fd, (struct sockaddr *) &client, &client_len);

        if (client_fd < 0) {
            perror("Could not establish new connection\n");
        }

        char client_conn_str_temp[0x100] = {0};
        snprintf(client_conn_str_temp, sizeof(client_conn_str_temp), \
            "%s:%d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        handle_client(client_fd, strdup(client_conn_str_temp)); // inside game_dispatcher
    }

    return 0;
}
