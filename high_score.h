#ifndef __HIGH_SCORE__
#define __HIGH_SCORE__

#include <stdbool.h>
#include "tictactoe.h"

bool handle_reset_winner_data(int client_fd, char* client_str);

bool handle_add_winner(int client_fd, char* client_str); // Collect information about the winner from the user
bool handle_set_outro_message(int client_fd, char* client_str);
bool handle_report_winners(int client_fd, char* client_str);

#endif
