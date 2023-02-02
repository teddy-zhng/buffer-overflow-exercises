
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "tictactoe.h"

//TODO make sure each game has their own board, client_fd etc
//this is just a visualization of what it'd look like for each player

enum Player board[][] = { //(0, 0) top left, (2, 2) bottom right
    {E, E, E}, 
    {E, E, E}, 
    {E, E, E}  
};

bool handle_place(int client_fd, char* client_str) {
    enum Player player = (int)(*client_str); //first four bytes of packet are the player
    int x_coord = (int)( *( client_str + sizeof(int)) );
    int y_coord = (int)( *( client_str + sizeof(int) * 2) );
    board[x_coord][y_coord] = player;
}

//should we send the whole board as a (9 * 4) byte string instead? modify client/how we send this
bool handle_read_board(int client_fd, char* client_str) {
    for (int x = 0; x < __BOARD_SIZE__; x++) {
        for (int y = 0; y < __BOARD_SIZE__; y++) {
            //send the info at each location
        }
    }
}

bool handle_get_winner(int client_fd) {
    //implement a more effecient check if needed 

    enum Player winner = E;
    
    for (int i = 0; i < __BOARD_SIZE__; i++) {
        //check vertical lines
        if ((board[i][0] != E) && 
            (board[i][0] == board[i][1]) && 
            (board[i][0] == board[i][2]))

            winner = board[i][0];

        // check horizontal lines
        if ((board[0][i] != E) &&
            (board[0][i] == board[1][i]) && 
            (board[0][i] == board[2][i]))

            return(board[0][i];
    }
    // check diagonals
    if ((board[0][0] != E) && 
        (board[0][0] == board[1][1]) && 
        (board[0][0] == board[2][2]))

        winner = board[0][0];

    if ((board[2][0] != E) && 
        (board[2][0] == board[1][1]) &&
        (board[0][0] == board[0][2]))

        winner = board[2][0];

    //send winner bac

    return 1;//what to return for status?

}