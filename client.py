import sys
import socket
import time
import random

names = ['Mav', 'Adam', 'Teddy', 'Ari', 'Donovan', 'Diana', 
'Eduardo', 'Emanuel', 'Nourya', 'Ron', 'Tobias', 'Vrushank']

handlers = {
	"handle_get_version":0,
	"handle_get_currently_logged_in_uname":1,
	"handle_login":2,
	"handle_login_admin":3,
	"handle_logout":4,
	"handle_create_user":5,
	"handle_admin_run_cmd":6,
	"handle_place":7, 
	"handle_read_board":8,
	"handle_get_winner":9,
	"handle_add_winner":10,
	"handle_reset_winner_data":11, 
    "handle_set_outro_message":12, 
    "handle_report_winners":13
}

def client_error_wrapper(error_msg):
	print("*"*len(str(error_msg)))
	print(error_msg)
	print("*"*len(str(error_msg)))

NET_ORDER = "big"

class TicTacToe(object):
	"""interact with Stanford ZERO fun/vulnerable tic tac toe server"""
	GET_VERSION_PKT = b"\x00"*4
	def __init__(self, connect_tuple, debug = True):
		super(TicTacToe, self).__init__()
		self.connect_tuple = connect_tuple
		self.server_conn = None
		self.debug = debug

	def read_response(self):
		resp_len = self.server_conn.recv(4)
		if len(resp_len) < 4:
			client_error_wrapper("unable to read resposne length")
			raise Exception("need 4 bytes in length resposne, you have " + str(len(resp_len)))
		elif len(resp_len) > 4:
			client_error_wrapper("unable to read resposne length")
			raise Exception("need 4 bytes in length resposne, you have " + str(len(resp_len)))
		resp_len = int.from_bytes(resp_len, NET_ORDER)
		if self.debug:
			print("getting msg resposne len:", resp_len)

		resp_data = self.server_conn.recv(resp_len)
		return resp_data

	def connect(self):
		try:
			self.server_conn = socket.socket() # default to tcp/ip
			self.server_conn.connect(self.connect_tuple)
		except Exception as e:
			client_error_wrapper(f"error connecting to {self.connect_tuple}")
			raise

	def get_version(self):
		self.server_conn.sendall(TicTacToe.GET_VERSION_PKT)
		return self.read_response()

    #caller guarantees turn is a single char, X or O
	def place(self, turn):
		user_coords = input("Please enter the coordinates you want to mark, eg: 0 0").split()
		while len(user_coords) != 2:
			input = ("You made me almost seg fault! Give me two coordinates!")

		turn_enum = 1
		if turn == 'O':
			turn_enum = 2
		packet = int.to_bytes(turn_enum, 4, NET_ORDER) + user_coords[0].encode("utf-8") + user_coords[1].encode("utf-8")
		self.server_conn.sendall(int.to_bytes(handlers["handle_get_winner"], 4, NET_ORDER))
		self.server_conn.sendall(packet) #send which player, and coordinates

	#expects a response that is a 9 byte string representing the board
	def read_board(self):
		self.server_conn.sendall(int.to_bytes(8, 4, NET_ORDER))
		response = self.read_response()
		print(response)

		#print the board //TODO convert ascii code from stuff
		print('     0     1     2')
		print(f'0    {response[0]}  |  {response[1]}  |  {response[2]}')
		print('     ----------------')
		print(f'1    {response[3]}  |  {response[4]}  |  {response[5]}')
		print('     ----------------')
		print(f'2    {response[6]}  |  {response[7]}  |  {response[8]}')
		
	#expects 4 bytes repr. winner, 0 = no one, 1 = X, 2 = O
	def get_winner(self):
		self.server_conn.sendall(int.to_bytes(handlers["handle_get_winner"], 4, NET_ORDER))
		response = self.read_response()
		return int.from_bytes(response, NET_ORDER)

def play(connect_tuple):
	game = TicTacToe(connect_tuple)
	print("connecting")
	game.connect()
	print("connected")
	print(game.get_version())

	print("WELCOME TO ZERO TICTACTOE. 100% VULNERABILITY-FREE CODE GUARANTEED")
	game.read_board()

	time.sleep(1)
	#gameplay
	turn = 'X'
	while game.get_winner == 0:
		print('Player %s\'s')
		game.place(game, turn)
		print('Good move. I think...')
		time.sleep(1)
		game.read_board()
		turn = turn == 'X' if turn == 'Y' else 'Y'

	#game ended
	name = names[random(0, len(names))] # fingers crossed
	print('CONGRATS %s YOU WIN!', name) 


def main():
	if len(sys.argv) != 3:
		print("Need <tictactoe host ip> <tictactoe host port>")
		return
	
	play(tuple([sys.argv[1], int(sys.argv[2])]))

if __name__ == '__main__':
	main()