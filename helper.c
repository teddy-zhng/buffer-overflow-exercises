#include <stdlib.h>
#include <stdio.h>

#include "helper.h"

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