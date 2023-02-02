#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>

#include "helper.h"
#include "globals.h"

bool handle_get_version(int client_fd, char* client_str) {
    char version_str[0x100] = {0};
    sprintf(version_str, "Stanford Zero TicTacToe vesion %s", TICTACTOE_VERSION_STR);
    respond_str_to_client(client_fd, version_str);
    return true;
}

void respond_str_to_client(int fd, char* str) {
	int len_htonl = htonl(strlen(str));

	int err = send(fd, &len_htonl, sizeof(len_htonl), 0);
	if (err < 0) {
		perror("send len in respond_str_to_client");
	}
	err = send(fd, str, strlen(str), 0);
	if (err < 0) {
		perror("send str in respond_str_to_client");
	}
}

bool get_str_from_client(int client_fd, char* str) {
	int str_len;
    int bytes_read = recv(client_fd, &str_len, sizeof(str_len), 0);
    if (bytes_read == -1) {
        perror("recv from get_str_from_client in len");
    }
    if (bytes_read == 0) {
        printf("client disconnected in str len in get_str_from_client\n");
        return false;
    }
    if (bytes_read != sizeof(str_len)) {
    	printf("client didn't send enough bytes for string length in get_str_from_client\n");
    	return false;
    }

    str_len = ntohl(str_len);
    log_verbose("will read str len %d\n", str_len);

    recv(client_fd, str, str_len, 0);
    if (bytes_read == -1) {
        perror("recv from get_str_from_client in data");
    }
    return true;
}

void uses_assumed_sizes() {
	if (sizeof(int) != 4) {
		printf("oh no! expected sizeof(int) to be size 4\n");
		exit(1);
	}
}

void stub_exit(char * str) {
	printf("%s\n", str);	
	exit(1);
}

