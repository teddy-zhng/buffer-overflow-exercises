import sys
import socket

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
		if len(resp_len) != 4:
			client_error_wrapper("unable to read resposne length")
			raise Exception("need at least 4 bytes in resposne")
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

		packet = turn + user_coords[0] + user_coords[1]
		self.server_conn.send(packet) #send which player, and coordinates

	#expects a response that is a 9 byte string representing the board
	def read_board(self):
		response = self.read_response(self)

		#print the board
		print('     0     1     2')
		print('0    %s  |  %s  |  %s', response[0], response[1], response[2])
		print('     ----------------')
		print('1    %s  |  %s  |  %s', response[4], response[5], response[6])
		print('     ----------------')
		print('2    %s  |  %s  |  %s', response[7], response[8], response[9])
		
	#expects 4 bytes repr. winner
	def get_winner(self):
		response = self.read_response(self)

        

def play(connect_tuple):
	game = TicTacToe(connect_tuple)
	print("connecting")
	game.connect()
	print("connected")
	print(game.get_version())


def main():
	if len(sys.argv) != 3:
		print("Need <tictactoe host ip> <tictactoe host port>")
		return
	
	play(tuple([sys.argv[1], int(sys.argv[2])]))

if __name__ == '__main__':
	main()