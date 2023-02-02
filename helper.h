//Helpful Header
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>

#ifndef HELPFUL_H
#define HELPFUL_H
	int error(int x, char * caller);
	struct hostent * hostent_error(struct hostent * x, char * caller);
	char * lower(char * temp);
#endif
