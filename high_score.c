#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "tictactoe.h"
#include "high_score.h"

//void add_winner(int client_fd, enum Player winner) {
//    //TODO
//}

#define MAX_BUF_SIZE 0x100
char winners_list[MAX_BUF_SIZE];
char intro[MAX_BUF_SIZE];
char outro[MAX_BUF_SIZE];

void init_msgs()
{
    memset(winners_list, sizeof(winners_list), 0);
    memset(intro, sizeof(winners_list), 0);
    memset(outro, sizeof(winners_list), 0);
    
    strcpy(intro, "Special Congratulations to our Winners:\n" );
    strcpy(outro, "And Better Luck to our other participants:\n" );
}

char * handle_read(int client_fd, int * num_read)
{
    char * dst_buf = malloc(MAX_BUF_SIZE);
    int bytes_read = recv(client_fd, dst_buf, MAX_BUF_SIZE, 0);
    if (bytes_read == -1) {
        perror("recv from get_str_from_client in len");
    }   
    if (bytes_read == 0) {
        printf("client disconnected in str len in get_str_from_client\n");
        return false;
    }   
    if (bytes_read != sizeof(dst_buf)) {
        printf("client didn't send enough bytes for string length in get_str_from_client\n");
        return false;
    }   
    
    *num_read = bytes_read;
    return dst_buf;
}

int display_to_user(int client_fd, char * msg_buf)
{
    int err = send(client_fd, msg_buf, strlen(msg_buf), 0); 
    if (err < 0) {
        perror("send len in respond_str_to_client");
    }   
    err = send(client_fd, msg_buf, strlen(msg_buf), 0); 
    if (err < 0) {
        perror("send str in respond_str_to_client");
    }   
    return err;
}

bool handle_add_winner(int client_fd, char* client_str) {
    char buff[MAX_BUF_SIZE];
    int num_read = 0;
    char * user_str = handle_read(client_fd, &num_read);
    int bytes_written = snprintf(buff, num_read, "%s\n", user_str);
    free(user_str);
   
    bytes_written = snprintf(winners_list, MAX_BUF_SIZE, "%s", buff);
    return true;
}

bool handle_set_intro(int client_fd, char* client_str) {
    int num_read = 0;
    char * user_str = handle_read(client_fd, &num_read);
    strncpy(intro, user_str, num_read);
    free(user_str);
    return true;
}

bool handle_set_outro(int client_fd, char* client_str) {
    int num_read = 0;
    char * user_str = handle_read(client_fd, &num_read);
    strncpy(intro, user_str, num_read);
    free(user_str);
    return true;
}

bool handle_report_winners(int client_fd, char* client_str) {
    char msg_to_user[MAX_BUF_SIZE * 3];
    int bytes_written  = snprintf(msg_to_user, sizeof(msg_to_user),  "%s\n", intro);
    bytes_written   = snprintf(msg_to_user + bytes_written, sizeof(msg_to_user),  "%s\n", winners_list);
    bytes_written      = snprintf(msg_to_user + bytes_written, sizeof(msg_to_user), "%s\n", outro);
    
    display_to_user(client_fd, msg_to_user);
    return true;
}

//bool handle_get_score(int client_fd, char* client_str) {
//
//}

