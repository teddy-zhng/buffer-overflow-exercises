//Helpful Header .c
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include "helper.h"

int error(int x, char * caller)		//The parameter caller is used as a way to pass perror relevant msg information.
{
    if ( x == -1)
    {
        perror(caller);
        exit(1);
    }
    return x;
}

struct hostent * hostent_error(struct hostent * x, char * caller)		//The parameter caller is used as a way to pass perror relevant msg information.
{
    if ( x == NULL)
    {
        perror(caller);
        exit(1);
    }
    return x;
}

char * lower(char * temp)
{
	for (int i = 0; temp[i] ; ++i)
	{
		temp[i] = tolower(temp[i]);
	}
	return temp;
}


