#ifndef __HIGH_SCORE__
#define __HIGH_SCORE__

#include <stdbool.h>
#include "tictactoe.h"

//void add_winner(int client_fd, enum Player winner); //use a username or cookie or session id instead?
void init_msgs();

bool handle_add_winner(int client_fd, char* client_str); // Collect information about the winner from the user
bool handle_set_intro(int client_fd, char* client_str);
bool handle_set_outro(int client_fd, char* client_str);
bool handle_report_winners(int client_fd, char* client_str);

#endif
