#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

#include "helper.h"

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