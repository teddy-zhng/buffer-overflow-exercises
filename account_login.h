#ifndef __ACCOUNT_LOGIN_H__
# define __ACCOUNT_LOGIN_H__

#include <stdbool.h>

bool handle_login(int client_fd, char* client_str);
bool handle_get_currently_logged_in_uname(int client_fd, char* client_str);
bool handle_admin_run_cmd(int client_fd, char* client_str);
bool handle_create_user(int client_fd, char* client_str);
bool handle_logout(int client_fd, char* client_str);

#endif