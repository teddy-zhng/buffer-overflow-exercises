#ifndef __HELPER_H__
# define __HELPER_H__

#define log_verbose printf

void uses_assumed_sizes();

void respond_str_to_client(int fd, char* str);

void stub_exit(char * str);

#endif