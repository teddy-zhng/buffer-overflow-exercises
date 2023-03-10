
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "tictactoe.h"
#include "high_score.h"
#include "helper.h"

// from tictactoe.h:
// enum Player {
//     E, //E means empty and happens to be null too
//	   X, 
//     O 
// };
enum Player board[__BOARD_SIZE__][__BOARD_SIZE__] = { //(0, 0) top left, (2, 2) bottom right
    {E, E, E}, 
    {E, E, E}, 
    {E, E, E} 
};

void reset_board(){
	for (int x = 0; x < __BOARD_SIZE__; x++) {
        for (int y = 0; y < __BOARD_SIZE__; y++) {
			board[x][y] = E;
        }
    }
}

//expects 3 * 4 bytes = 12
bool handle_place(int client_fd, char* client_str) {
	enum Player player; //first four bytes of packet are the player

	//flipped because of array indexing vs x-y order
	int y_coord; //next four bytes are the y
	int x_coord; //last 4 are the x

	while ( (player = get_int_from_client(client_fd)) == -1) { }
	while ( (y_coord = get_int_from_client(client_fd)) == -1) { }
	while ( (x_coord = get_int_from_client(client_fd)) == -1) { }

	board[x_coord][y_coord] = player;
	return true;
}

//should we send the whole board as a (9 * 4) byte string instead? modify client/how we send this
// bool handle_read_board(int client_fd, char* client_str) {
//     for (int x = 0; x < __BOARD_SIZE__; x++) {
//         for (int y = 0; y < __BOARD_SIZE__; y++) {
//             //send the info at each location to client
//         }
//     }

//     return 0;
// }

// sends the board as a 9-byte char*
bool handle_read_board(int client_fd, char* client_str) {
    char board_pkt[10] = {0};
    int i = 0;
    for (int x = 0; x < __BOARD_SIZE__; x++) {
        for (int y = 0; y < __BOARD_SIZE__; y++) {
			char player = board[x][y] == X ? 'X' : board[x][y] == O ? 'O': ' ';
            board_pkt[i] = player;
            i++;
        }
    }

    respond_str_to_client(client_fd, board_pkt);

    return true;
}


//sends 4 bytes
bool handle_get_winner(int client_fd, char* client_str) {
    //implement a more effecient check if needed 

    enum Player winner = E;

    for (int i = 0; i < 3; i++) {
        //check vertical lines
        if ((board[i][0] != E) && (board[i][0] == board[i][1]) && (board[i][0] == board[i][2]))
            winner = board[i][0];
        // check horizontal lines
        if ((board[0][i] != E) && (board[0][i] == board[1][i]) && (board[0][i] == board[2][i]))
            winner = board[0][i];
    }

    // check diagonals
    if ((board[0][0] != E) && (board[0][0] == board[1][1]) && (board[0][0] == board[2][2]))
        winner = board[0][0];

    if ((board[2][0] != E) && (board[2][0] == board[1][1]) && (board[0][0] == board[0][2]))
        winner = board[2][0];

    //clear board and send winner to client
	if (winner != E) {
		reset_board();
	}
    respond_buff_to_client(client_fd, (char*)(&winner), sizeof(E));

    return true;//what to return for status?

}
