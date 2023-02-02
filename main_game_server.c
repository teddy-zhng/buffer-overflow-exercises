#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdint.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include "helper.h"


typedef struct dirent dirent;

#define PORT 5555							//Defines the Port Address
#define BACKLOG 10							//Defines the Queue size for Listen


//Global Var
int sockfd;	//This is the file descriptor for the servers socket connection
fd_set connections;	//Creates a list which manages all incoming requests
//char client_path[BUFSIZ];
int dirs_to_ignore;

void new()
{
	//This 'Paragraph' is mundane, merely Establishes a connection.
	struct sockaddr_in client_addr;
	int sin_size = sizeof(struct sockaddr_in);							//This is need because only a reference can be passed (accept takes a non-int parameter)
	int curfd = error(accept(sockfd, (struct sockaddr *) &client_addr, (socklen_t *) &sin_size), "accept");		//Set current fd to the accept client connection
	printf("Connection Made\n");
}


//Socket Settings and select loop below
void setup(struct sockaddr_in * addr )
{
    //Sets the socket type to Internet
    addr->sin_family = AF_INET;

    //Sets the port to Network Standard Endianess value of PORT
    addr->sin_port = htons(PORT);

    //Sets the server IP to the current machine's address
    addr->sin_addr.s_addr = INADDR_ANY;

    //Zeroes the rest of the struct 
    memset(addr->sin_zero, 0, 8);
}

void socketoptions()
{
    int flag = 1;
    error(setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)), "setsockopt");     //Make sure to put before bind
}

void establish(struct sockaddr_in * addr)
{
	//Create a Internet Domain Stream Socket
	sockfd = error(socket(AF_INET, SOCK_STREAM, 0), "socket");
	
	//printf("%d\n",addr->sin_port );

	//Puts this computer's address and the specified PORT number into the struct, sets it to AF_INET, and zeroes sin_zero 
	setup(addr);

	//Set the Options for the socket, particularly Re-Use of the PORT
	socketoptions();

	//printf("%d\n",addr->sin_port );
	
	//Register the socket with the OS
    error(bind(sockfd, (struct sockaddr *) addr, sizeof(struct sockaddr)), "bind");
    
    //Start waiting for incoming signals, and cap the queue at BACKLOG
    error(listen(sockfd, BACKLOG), "listen" );
}

void build_select_list()
{
    FD_ZERO(&connections);					//Zero all values/clear

    FD_SET(sockfd, &connections);			//Ensures that the Set will listen to the socket.
    
    FD_SET(0, &connections);				//Allows Listening on stdin
}

void initialize()
{
	struct sockaddr_in my_addr;
	
	//Creates all the settings for the socket    
    establish(&my_addr);

    printf("The Server is now active on Port: %d \n", PORT);

    char intro[] = "The Server is now Running, to quit at any time, type in 'QUIT' or 'Q'/'q'.\nAs of now all other input will be ignored.\n";
    error( write(1, intro, strlen(intro)), "write");
}

int terminate()
{
	char temp[BUFSIZ];
	char * asses = fgets( temp, BUFSIZ, stdin);
	lower(temp);
    return ( !asses || !strcmp(temp, "q\n") || !strcmp(temp,"quit\n") );
    //	return 1;
    //return 0;
}

int check()
{
	if (FD_ISSET(sockfd, &connections))
	{
			new();
	}
	else if (FD_ISSET(0, &connections))
	{
		printf("Terminating\n");
		return terminate();
	}
	return 0;
}

void run()
{
	build_select_list();	//Can be added to main for line count if needed
	while( error(select(FD_SETSIZE, &connections, NULL, NULL, NULL), "select") > 0 )		//error(accept(sockfd, (struct sockaddr *) &client_addr, (socklen_t *) &sin_size), "accept")
	{
		printf("Slected\n");
		if( check() )
			return;
		build_select_list();	//This may need to come before check
	}
}

void disengage()
{
	int pid, status;
	//Shutsdown the socket
    shutdown(sockfd, SHUT_RDWR);
    
    //Terminates/Closes the Socket
    close(sockfd);

    printf("Waiting for Children\n");
    //Prevent Zombification
    while ( ( pid = wait(&status) ) > 0)
    {
    	printf("%d completed with status : %d\n",pid, status );
    }

}

int main()
{
	//Prints Inital message and intializes connections
	initialize();

	setbuf(stdout,0);	//For Debug Purposes

    //Loops through and accepts connections iteratively
    run();

	//Cleanup
	disengage();

	return 0;
}
