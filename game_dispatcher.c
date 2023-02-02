
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#include "globals.h"
#include "helper.h"
#include "game_dispatcher.h"

void handle_get_version(int client_fd, char* client_str) {
    char version_str[0x100] = {0};
    sprintf(version_str, "Stanford Zero TicTacToe vesion %s", TICTACTOE_VERSION_STR);
    respond_str_to_client(client_fd, version_str);
}

void handle_client(int client_fd, char* client_str) {
    char pkt_type;
    int bytes_read;

    printf("%s connected\n", client_str);

    while (1) {

        // read type
        bytes_read = recv(client_fd, &pkt_type, sizeof(pkt_type), 0);

        if (bytes_read == -1) {
            perror("recv from handle_client");
            goto end;
        }

        if (bytes_read == 0) {
            printf("%s disconnected\n", client_str);
            goto end;
        }
 
        if (bytes_read != sizeof(pkt_type)) {
            printf("only read %d for peer %s. Disconnecting.\n", bytes_read, client_str);
            goto end;
        }

        switch(pkt_type) {
            case 0:
                log_verbose("%s doing handle_get_version\n", client_str);
                handle_get_version(client_fd, client_str);
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

