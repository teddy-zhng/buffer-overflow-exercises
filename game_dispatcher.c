
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "globals.h"
#include "helper.h"
#include "game_dispatcher.h"

// includes for dispatchers
#include "tictactoe.h"
#include "account_login.h"
#include "high_score.h"

typedef bool (*pkt_handler)(int client_fd, char* client_str);

pkt_handler handlers[] = {
    /* general */
    handle_get_version, 

    /* login */
    handle_get_currently_logged_in_uname,
    handle_login,
    handle_login_admin,
    handle_logout,
    handle_create_user,
    handle_admin_run_cmd,

	/* gameplay stuff */
	handle_place, 
	handle_read_board,
	handle_get_winner,

    /* high scores */
    handle_add_winner, 

    handle_reset_winner_data, 
    handle_set_outro_message, 
    handle_report_winners,
};

void handle_client(int client_fd, char* client_str) {
    int pkt_type;
    int bytes_read;

    printf("%s connected in handle_client\n", client_str);

    while (1) {
        // read packet type
		printf("reading packet type\n");
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
        pkt_type = ntohl(pkt_type);

        // handle packet type
        printf("dispatching\n");
        bool dispatch_status = handlers[pkt_type](client_fd, client_str);
        printf("dispatch_status: %d\n", dispatch_status);
    }
end:
    free(client_str);
    shutdown(client_fd, SHUT_RDWR);
    close(client_fd);
}

