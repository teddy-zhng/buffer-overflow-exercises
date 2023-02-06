#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

// #include "tictactoe.h"
#include "high_score.h"
#include "helper.h"

//void add_winner(int client_fd, enum Player winner) {
//    //TODO
//}

#define MAX_BUF_SIZE (0x100)
char winners_list[MAX_BUF_SIZE] = {0};
char outro_message[MAX_BUF_SIZE] = {0};

void reset_winner_data()
{
    memset(winners_list, sizeof(winners_list), 0);
    memset(outro_message, sizeof(outro_message), 0);
    strcpy(outro_message, "\nBetter Luck to our other participants next time!\n" );
}

bool handle_add_winner(int client_fd, char* client_str) {
    char winner_name[0x80];
    if (!get_buffer_from_client(client_fd, winner_name, sizeof(winner_name))) {
        printf("handle_add_winner error: get_buffer_from_client failed\n");
        return false;
    }
    strncpy(winners_list, winner_name, sizeof(winners_list));
    printf("handle_add_winner success!\n");
    return true;
}

bool handle_set_outro_message(int client_fd, char* client_str) {
    if (!get_buffer_from_client(client_fd, outro_message, sizeof(outro_message))) {
        printf("handle_set_outro_message error: get_buffer_from_client failed\n");
        return false;
    }
    printf("handle_set_outro_message success!\n");
    return true;
}

bool handle_report_winners(int client_fd, char* client_str) {
    char msg_to_user[MAX_BUF_SIZE];

    int bytes_written = snprintf(msg_to_user, sizeof(msg_to_user),  "**WINNER LIST**\n%s\n*** :) ***", winners_list);
    snprintf(msg_to_user + bytes_written, sizeof(msg_to_user) - bytes_written, "%s", outro_message);
    respond_str_to_client(client_fd, msg_to_user);
    return true;
}
