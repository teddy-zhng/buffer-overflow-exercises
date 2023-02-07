

all: server

server: globals.c account_login.c high_score.c main_game_server.c tictactoe.c helper.c game_dispatcher.c 
	gcc -O0 -fno-stack-protector -Wall -g globals.c helper.c account_login.c high_score.c tictactoe.c game_dispatcher.c main_game_server.c -o server

clean:
	rm server