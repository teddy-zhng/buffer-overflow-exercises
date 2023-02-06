#ifndef __TICTACTOE__
#define __TICTACTOE__
#define __BOARD_SIZE__ 3

#include <stdbool.h>

enum Player {
    E, X, O //E is empty and happens to be null too, 0, 1, 2 default values
};

bool handle_place(int client_fd, char* client_str);

bool handle_read_board(int client_fd, char* client_str);

bool handle_get_winner(int client_fd, char* client_str);

#endif