#ifndef __HELPER_H__
# define __HELPER_H__

#include <stdbool.h>

#define log_verbose printf

// client communication helpers
bool handle_get_version(int client_fd, char* client_str);
void respond_str_to_client(int fd, char* str);
void respond_buff_to_client(int fd, char* buff, int buff_len);

int get_int_from_client(int fd);
bool get_str_from_client(int fd, char* str);
bool get_buffer_from_client(int client_fd, char* output, int output_size);

// C heleprs
void uses_assumed_sizes();
void stub_exit(char * str);

#endif