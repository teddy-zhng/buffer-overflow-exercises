
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#include "helper.h"
#include "game_dispatcher.h"

void handle_client(int client_fd, char* client_str) {
    int pkt_type;
    int bytes_read;

    printf("client %s connected\n", client_str);

    while (1) {
        bytes_read = recv(client_fd, &pkt_type, 4, 0);

        if (bytes_read == -1) {
            perror("recv from handle_client");
        }

        if (bytes_read == 0) {
            printf("client %s disconnected\n", client_str);
            goto end;
        }

        if (bytes_read != 4) {
            perror("didn't read enough bytes for type :(");
        }

        switch(pkt_type) {
        case 0:
            // handle pkt type 0
            printf("stub code will handle pkt type 0\n");
            break;
        case 1:
            printf("stub code will handle pkt type 1\n");
            break;

        default:
            printf("stub code will handle unknown pkt type!\n");
            break;        
        }
    }
end:
    free(client_str);
    shutdown(client_fd, SHUT_RDWR);
    close(client_fd);
}
